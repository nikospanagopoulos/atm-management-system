#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    char line[512];
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        if (line[0] == '\n' || line[0] == '\0')
            continue;
             int matched = sscanf(line, "%d|%d|%49[^|]|%d|%d/%d/%d|%99[^|]|%19[^|]|%lf|%9[^|\n]",
                              &r->id,
                              &r->userId,
                              name,
                              &r->accountNbr,
                              &r->deposit.month,
                              &r->deposit.day,
                              &r->deposit.year,
                              r->country,
                              r->phone,
                              &r->amount,
                              r->accountType);

        if (matched == 11)
        {
            strcpy(r->name, name);
            return 1;
        }
        return 0;
    }
    return 0;
}


void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d|%d|%s|%d|%d/%d/%d|%s|%s|%.2lf|%s\n\n",
            r.id,
	    u.id,
	    u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

int loadAllRecords(struct Record *out)
{
    FILE *ptr = fopen(RECORDS, "r");
    if (ptr == NULL)
    {
        printf("Error! opening file");
        exit(1);    
    }
    char name [50];
    int i = 0;
    while (i < 100 &&getAccountFromFile(ptr, name, &out[i]))
    {
        i++;
    }
    fclose(ptr);
    return i;
}

void saveAllRecords(struct Record *records, int count)
{
    FILE *ptr = fopen(RECORDS, "w");
    if (ptr == NULL)
    {
        printf("Error! opening file");
        exit(1);    
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(ptr, "%d|%d|%s|%d|%d/%d/%d|%s|%s|%.2lf|%s\n\n",
                records[i].id,
        records[i].userId,
        records[i].name,
                records[i].accountNbr,
                records[i].deposit.month,
                records[i].deposit.day,
                records[i].deposit.year,
                records[i].country,
                records[i].phone,
                records[i].amount,
                records[i].accountType);
    }
    fclose(ptr);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    fseek(pf, 0, SEEK_SET);
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    while (getchar() != '\n');
    fgets(r.country, sizeof(r.country), stdin);
    r.country[strcspn(r.country, "\n")] = '\0';
    printf("\nEnter the phone number:");
    scanf("%s", r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> savings\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    struct Record allRecords[100];
    int count = loadAllRecords(allRecords);
    r.id = count;
    r.userId = u.id;

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;
    int found = 0;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            found =1;
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    if (!found)
        printf("No accounts found!\n");
    fclose(pf);
    success(u);
}

void checkAccountDetails(struct User u)
{
    int accountNbr;
    printf("Enter account number: ");
    scanf("%d", &accountNbr);
    system("clear");
   
    struct Record records[100];
    int count = loadAllRecords(records);
    int found = 0;

   for (int i = 0; i < count; i++)
   {
        if (records[i].userId == u.id && records[i].accountNbr == accountNbr)
        {
            found = 1;
             printf("Account number: %d\n", records[i].accountNbr);
            printf("Deposit Date: %d/%d/%d\n", records[i].deposit.day, records[i].deposit.month, records[i].deposit.year);
            printf("Country: %s\n", records[i].country);
            printf("Phone: %s\n", records[i].phone);
            printf("Amount: $%.2f\n", records[i].amount);
            printf("Type: %s\n", records[i].accountType);
  
        if (strcmp(records[i].accountType, "savings") == 0)
            printf("You will get $%.2f as interest on day %d of every month\n",
                records[i].amount * 0.07 / 12, records[i].deposit.day);
        else if (strcmp(records[i].accountType, "current") == 0)
            printf("You will not get interests because the account is of type current\n");
        else if (strcmp(records[i].accountType, "fixed01") == 0)
            printf("You will get $%.2f as interest on %d/%d/%d\n",
                records[i].amount * 0.04, records[i].deposit.day, records[i].deposit.month, records[i].deposit.year + 1);
        else if (strcmp(records[i].accountType, "fixed02") == 0)
            printf("You will get $%.2f as interest on %d/%d/%d\n",
                records[i].amount * 0.05 * 2, records[i].deposit.day, records[i].deposit.month, records[i].deposit.year + 2);
        else if (strcmp(records[i].accountType, "fixed03") == 0)
            printf("You will get $%.2f as interest on %d/%d/%d\n",
                records[i].amount * 0.08 * 3, records[i].deposit.day, records[i].deposit.month, records[i].deposit.year + 3);
        break;
        }   
}
    stayOrReturn(found, checkAccountDetails, u);
}


void updateAccount(struct User u)
{
    int accountNbr;
    printf("Enter account number: ");
    scanf("%d", &accountNbr);
    system ("clear");

    struct Record records[100];
    int count = loadAllRecords(records);
    int found = 0;

    for (int i =0; i < count; i++)
    {
        if (records[i].userId == u.id && records[i].accountNbr == accountNbr)
        {
            found = 1;
            int choice;
        invalidChoice:
            printf("Update (1) phone or (2) country? ");
            scanf("%d", &choice);
            while (getchar() != '\n');

            if (choice ==1)
            {
                printf("New phone: ");
                fgets(records[i].phone, sizeof(records[i].phone), stdin);
                records[i].phone[strcspn(records[i].phone, "\n")] = '\0';
            }
            else if (choice ==2)
            {
                printf("New country: ");
                fgets(records[i].country, sizeof(records[i].country), stdin);
                records[i].country[strcspn(records[i].country, "\n")] = '\0';
            }
            else
            {
                printf("Insert a valid operation!\n");
                goto invalidChoice;
            }

            saveAllRecords(records, count);
            break;
            }
        }
    stayOrReturn(found, updateAccount, u);
    }

void makeTransaction(struct User u)
{
    int accountNbr;
    printf("Enter account number: ");
    scanf("%d", &accountNbr);
    system("clear");

    struct Record records[100];
    int count = loadAllRecords(records);
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (records[i].userId == u.id && records[i].accountNbr == accountNbr)
        {
            found = 1;

            if (strcmp(records[i].accountType, "fixed01") == 0 ||
                strcmp(records[i].accountType, "fixed02") == 0 ||
                strcmp(records[i].accountType, "fixed03") == 0)
                {
                    printf("Transactions are not allowed for fixed accounts!\n");
                   break;
                }
            int choice;
            int changed = 0;
        invalidChoice:
            printf("Do you want to (1) deposit or (2) withdraw? ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                double depositAmount;
                printf("Enter amount to deposit: $");
                scanf("%lf", &depositAmount);
               
                if (depositAmount <= 0)
                {
                    printf("Amount must be positive!\n");
                    goto invalidChoice;
                }
            
                records[i].amount += depositAmount;
                changed = 1;
                printf("Deposited $%.2f successfully!\n", depositAmount);

            }
            else if (choice == 2)
            {
                double withdrawAmount;
                printf("Enter amount to withdraw: $");
                scanf("%lf", &withdrawAmount);
                
                if (withdrawAmount <= 0)
                {
                    printf("Amount must be positive!\n");
                    goto invalidChoice;
                }

                if (withdrawAmount > records[i].amount + 0.001)
                {
                    printf("Insufficient funds! Current balance: $%.2f\n", records[i].amount);
                    goto invalidChoice;
                }
                records[i].amount -= withdrawAmount;
                changed = 1;
                printf("Withdrew $%.2f successfully!\n", withdrawAmount);
            }
            else
            {
                printf("Insert a valid operation!\n");
                goto invalidChoice;
            }
        if (changed)
        {
            saveAllRecords(records, count);
        }
            break;
        }
    }
    stayOrReturn(found, makeTransaction, u);
}