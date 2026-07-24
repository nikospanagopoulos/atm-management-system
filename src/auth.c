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
int loadAllUsers (struct User *out)
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

int loginUser(struct User *u)
{

if (isSystemLocked())
{
    printf("System is locked! Contact the admin.\n");
    exit(1);
}
 // username loop
    struct User users[100];
    int count = loadAllUsers(users);
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
        if (strcmp(users[i].name, u->name) ==0 && strcmp(users[i].password, u->password) == 0)
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
    scanf("%s", u->password);
    struct User users[100];
    int count = loadAllUsers(users);
    for (int i = 0; i < count; i++)    {
        if (strcmp(users[i].name, u->name) == 0)
        {            printf(RED "Username already exists\n" RESET);
            return 0;
        }
    }         
   u->id = count;
    FILE *fp = fopen("./data/users.txt", "a");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", u->id, u->name, u->password);
    fclose(fp);
    return 1;
}   






