#include <termios.h>
#include "header.h"
#include <unistd.h>


char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}
int loadAllUsersFromTxt (struct User *out)
{
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    int i = 0;
    while (fscanf(fp, "%d %s %s", &out[i].id, out[i].name, out[i].password) != EOF)
    {
        i++;
    }
    fclose(fp);
    return i;
}
int isSystemLocked()
{
    FILE *fp = fopen("./data/system_locked.txt", "r");
    if (fp == NULL)
    {
        return 0;
    }
    fclose(fp);
    return 1;
}   
int isUserLocked(char *name)
{
    FILE *fp = fopen("./data/locked_users.txt", "r");
    if (fp == NULL)
    return 0;

    char lockedName[50];
    while (fscanf(fp, "%s", lockedName) != EOF)
    {
        if (strcmp(lockedName, name) == 0)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int lockSystem()
{
    FILE *fp = fopen("./data/system_locked.txt", "w");
    if (fp ==NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fp, "locked\n");
    fclose(fp);
    return 1;
}

int lockUser(char *name)
{
    FILE *fp = fopen("./data/locked_users.txt", "a");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fp, "%s\n", name);
    fclose(fp);
    return 1;
}

void bytesToHex(const unsigned char *bytes, int len, char *output){
    for (int i = 0; i < len; i++) {
        sprintf(output + i*2, "%02x", bytes[i]);
    }
    output[len*2] = '\0';
}

void generateSalt(char *output_hex){
    unsigned char raw_salt[16];

    if (RAND_bytes(raw_salt, 16) != 1) {
        printf(RED "Critical error: unable to generate secure random salt.\n" RESET);
        exit(1);
    }
        bytesToHex(raw_salt, 16, output_hex);
}

void sha256Hash(const char *input, char *output_hex){
    unsigned char raw_hash[32];
    unsigned int actual_len;
    EVP_MD_CTX *CTX = EVP_MD_CTX_new();
        if (CTX == NULL) {
        printf(RED "Critical error: unable to create hash context.\n" RESET);
        exit(1);
    }
    if (EVP_DigestInit_ex(CTX, EVP_sha256(), NULL) != 1) {
        printf(RED "Critical error: unable to initialize hash context.\n" RESET);
        exit(1);
    }
    if (EVP_DigestUpdate(CTX, input, strlen(input)) != 1) {
        printf(RED "Critical error: unable to update hash context.\n" RESET);
        exit(1);
    }
    if (EVP_DigestFinal_ex(CTX, raw_hash, &actual_len) != 1) {
        printf(RED "Critical error: unable to finalize hash.\n" RESET);
        exit(1);
    }
    EVP_MD_CTX_free(CTX);
    bytesToHex(raw_hash, actual_len, output_hex);
}

void hashPassword(const char *password, char *output) {
    char salt_hex[33];
    char combined[COMBINED_BUFFER_SIZE];
    char hash_hex[65];
    generateSalt(salt_hex);
        strcpy(combined, salt_hex);
        strcat(combined, password);

        sha256Hash(combined, hash_hex);
        strcpy(output, salt_hex);
        strcat(output, ":");
        strcat(output, hash_hex);
}

int verifyPassword(const char *password, const char *stored) {
    char salt_hex[33];
    char stored_hash_hex[65];
    char combined[COMBINED_BUFFER_SIZE];
    char computed_hash_hex[65];

    sscanf(stored, "%32[^:]:%64s", salt_hex, stored_hash_hex);
    strcpy(combined, salt_hex);
    strcat(combined, password);

    sha256Hash(combined, computed_hash_hex);
    return (strcmp(computed_hash_hex, stored_hash_hex) == 0);
}


int migration(const char *stored, const char *newFile) {
    FILE *old_fp = fopen(stored, "r");
    if (old_fp == NULL) {
        printf(RED "Error! opening old file: %s\n" RESET, stored);
        return 0;
    }

    FILE *new_fp = fopen(newFile, "w");
    if (new_fp == NULL) {
        printf(RED "Error! opening new file: %s\n" RESET, newFile);
        fclose(old_fp);
        return 0;
    }
    struct User user;
    while (fscanf(old_fp, "%d %49s %99s", &user.id, user.name, user.password) != EOF) {
        char hashed[COMBINED_BUFFER_SIZE];
        if (strchr(user.password, ':') != NULL) {
            fprintf(new_fp, "%d %s %s\n", user.id, user.name, user.password);
            continue;
        }
        hashPassword(user.password, hashed);
        fprintf(new_fp, "%d %s %s\n", user.id, user.name, hashed);
    }

    fclose(old_fp);
    fclose(new_fp);
    int result = rename(newFile, stored);
    if (result != 0) {
        printf(RED "Error! renaming new file to old file: %s\n" RESET, stored);
        return 0;
    }
    return 1;
}

int loginUser(struct User *u)
{

if (isSystemLocked())
{
    printf("System is locked! Contact the admin.\n");
    exit(1);
}
 // username loop
    struct User users[100];
    int count = loadAllUsersFromTxt(users);
    int found = 0;
    int username_attempts = 0;

    while (username_attempts < 2)
    {
        printf("Enter username: ");
        scanf("%s", u->name);

        for (int i = 0; i < count; i++)
        {
            if (strcmp(users[i].name, u->name) == 0)
            {
                found = 1;
                break;
            }
        }
        if (found) break;

        username_attempts++;
        printf("Username not found! %d attempt(s) remaining.\n", 2 - username_attempts);
    }

    if (!found)
    {
        lockSystem();
        printf("Too many failed attempts! System locked.\n");
        exit(1);
    }

    if (isUserLocked(u->name))
    {
        printf("This user is locked! Contact the admin.\n");
        fflush(stdout);
        sleep(3);
        return 0;
    }

 int password_attempts = 0;

 while (password_attempts < 3)
 {
printf("Enter password: ");
    struct termios oflags, nflags;
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag &= ~ICANON;
    tcsetattr(fileno(stdin), TCSANOW, &nflags);
    while (getchar() != '\n');
    int i = 0;
    char ch;
    while ((ch = getchar()) != '\n')
    {
        u->password[i++] = ch;
        printf("*");
    }
    u->password[i] = '\0';
    tcsetattr(fileno(stdin), TCSANOW, &oflags);
    printf("\n");

    for (int i = 0; i< count; i++)
    {
        if (strcmp(users[i].name, u->name) ==0 && verifyPassword(u->password, users[i].password) == 1)
    {
        u->id = users[i].id;
        return 1;
    }
    }
    password_attempts++;
    printf("Wrong password! %d attempt(s) remaining.\n", 3 - password_attempts);
 }

 lockUser(u->name);
 printf("Too many failed attempts! Account locked.\n");
 fflush(stdout);
 sleep(3);
 return 0;
}

int registerUser (struct User *u)
{
   printf("Enter username: ");
    scanf("%s", u->name);
    printf("Enter password: ");
    scanf("%99s", u->password);
    struct User users[100];
    int count = loadAllUsersFromTxt(users);
    while (strlen(u->password) > MAX_PASSWORD_LEN)
        {
            printf(RED "Password too long! Max length is %d characters.\n" RESET, MAX_PASSWORD_LEN);
            printf("Enter password: ");
            scanf("%99s", u->password);
        }
    for (int i = 0; i < count; i++)    {
        if (strcmp(users[i].name, u->name) == 0)
        {            printf(RED "Username already exists\n" RESET);
            return 0;
        }
        
    }         
   int maxId = -1;
    for (int i = 0; i < count; i++)
    {
        if (users[i].id > maxId)
        {
            maxId = users[i].id;
        }
    }
    u->id = maxId + 1;

    char hashed[COMBINED_BUFFER_SIZE];
    hashPassword(u->password, hashed);
    strcpy(u->password, hashed);

    if(!insertUser(u))
    {
        printf(RED "Error saving user to database!\n" RESET);
        return 0;
    }

    FILE *fp = fopen("./data/users.txt", "a");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", u->id, u->name, hashed);
    fclose(fp);
    return 1;
}   





