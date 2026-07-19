#include "header.h"
#include <unistd.h>

void mainMenu(struct User u)
{
    int option;
    do
    {
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccount(u);
        break;
    case 3:
       checkAccountDetails(u);
       break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
       removeAccount(u);
        break;
    case 7:
        transferOwnership(u);
        break;
    case 8:
        system("clear");
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
}while (option !=8);
}

void initMenu(struct User *u)
{
    if (isSystemLocked())
    {
        printf("System is locked! Contact the admin.\n");
        exit(1);
    }
    int r = 0;
    int option;
     while (!r)
     {
        system("clear");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
           if (loginUser(u) == 1)
           {
            r = 1;
           }
           else
           {
            sleep(2);
            while(getchar() != '\n');
           }
            break;
        case 2:
            if (registerUser(u) == 1)
            r = 1;
            break;
        case 3:
          system("clear");
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
}

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}
