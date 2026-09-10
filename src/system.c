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
    fprintf(ptr, "%d|%d|%s|%d|%d/%d/%d|%s|%s|%.2lf|%s\n",
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

int loadAllRecordsFromTxt(struct Record *out)
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
        fprintf(ptr, "%d|%d|%s|%d|%d/%d/%d|%s|%s|%.2lf|%s\n",
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

int stayOrReturn(int notGood, struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf(RED "\n✖ Record not found!!\n" RESET);
    invalid:
        option = readInt("\nEnter 0 to try again, 1 to go to the main menu, and 2 to exit: ");
        if (option == 0)
        {
            return 0;
        }
        else if (option == 1)
        {
            system("clear");  
            mainMenu(u);
        }
        else if (option == 2)
        {
            system("clear");
            exit(0);
        }
        else
        {
            printf(RED "Insert a valid operation!\n" RESET);
            goto invalid;
        }
    }
    else
    {
        option = readInt("\nEnter 1 to go to the main menu and 0 to exit: ");
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
    return -1;
}

int readInt(const char *prompt)
{
    int value;
    int result;

    do
    {
        printf("%s", prompt);
        result = scanf("%d", &value);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (result != 1)
            printf(RED "Invalid input! Please enter a number.\n" RESET);

    } while (result != 1);

    return value;
}

void success(struct User u)
{
    int option;
    printf(GREEN "\n✔ Success!\n\n" RESET);
invalid:
    option = readInt("Enter 1 to go to the main menu and 0 to exit: ");
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
        printf(RED "Insert a valid operation!\n" RESET);
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
    r.accountNbr = readInt("\nEnter the account number:");

    fseek(pf, 0, SEEK_SET);
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf(RED "✖ This Account already exists for this user\n\n" RESET);
            goto noAccount;
        }
    }
    while (getchar() != '\n');

    do
{
    printf("Enter the country:");
    fgets(r.country, sizeof(r.country), stdin);
    r.country[strcspn(r.country, "\n")] = '\0';

    if (strlen(r.country) == 0)
        printf(RED "Country cannot be empty!\n" RESET);

    } while (strlen(r.country) == 0);

    do
    {
    printf("\nEnter the phone number:");
    fgets(r.phone, sizeof(r.phone), stdin);
    r.phone[strcspn(r.phone, "\n")] = '\0';

    if (strlen(r.phone) == 0)
        printf(RED "Phone number cannot be empty!\n" RESET);
    } while (strlen(r.phone) == 0);

    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> savings\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    while (strcmp(r.accountType, "savings") != 0 && strcmp(r.accountType, "current") != 0 &&
           strcmp(r.accountType, "fixed01") != 0 && strcmp(r.accountType, "fixed02") != 0 &&
           strcmp(r.accountType, "fixed03") != 0)
    {
        printf(RED "✖ Invalid account type! Please choose from the given options.\n" RESET);
        printf("\nChoose the type of account:\n\t-> savings\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
        scanf("%s", r.accountType);
    }

    struct Record allRecords[100];
    int count = loadAllRecordsFromTxt(allRecords);
    int maxExistingId = -1;
    for (int i = 0; i <count; i++)
    {
        if (allRecords[i].id > maxExistingId)
        {
            maxExistingId = allRecords[i].id;
        }

    }
    r.id = maxExistingId + 1;
    r.userId = u.id;
    strcpy(r.name, u.name);

    if(!insertRecord(&r))
    {
        printf(RED "Error saving record to database!\n" RESET);
        fclose(pf);
        return;
    }

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
    
    struct Record allRecords[100];
    int count = loadAllRecordsFromTxt(allRecords);
    int userHasAccounts = 0;
    for (int i =0; i < count; i++)
    {
        if (allRecords[i].userId == u.id)
        {
            userHasAccounts = 1;
            break;
        }
    }
    if (userHasAccounts)
    {
        printf(CYAN "%-6s%-10s%-10s%-13s%-17s%-12s%-12s\n" RESET,
               "ID", "Acc.Num", "Type", "Balance", "Country", "Phone", "Deposit Date");
        printf("----  --------  --------  -----------  ---------------  ----------- ------------\n");
    }
    
    while (getAccountFromFile(pf, userName, &r))
    {
        if (r.userId ==u.id)
        {
            found =1;
            char dateStr[15];
            sprintf(dateStr, "%d/%d/%d", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("%-6d%-10d%-10s$%-12.2f%-17s%-12s%-12s\n",
                   r.id, r.accountNbr, r.accountType, r.amount, r.country, r.phone, dateStr);
        }
    }
    if (!found)
        printf(RED "No accounts found!\n" RESET);
    fclose(pf);
    success(u);
}

void checkAccountDetails(struct User u)
{
    int accountNbr;

    while(1)
    {
    system("clear");
    accountNbr = readInt("Enter account number: ");
    system("clear");
   
    struct Record records[100];
    int count = loadAllRecordsFromTxt(records);
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
    if (found)
    {
            stayOrReturn(found,  u);
            return;
        }
        else
        {
            int result = stayOrReturn(found, u);
            if (result == 0)
            continue;
        }
    }
}


void updateAccount(struct User u)
{   
    int accountNbr;

    while (1)
    {
    system ("clear");
    accountNbr = readInt("Enter account number: ");
    system ("clear");

    struct Record records[100];
    int count = loadAllRecordsFromTxt(records);
    int found = 0;

    for (int i =0; i < count; i++)
    {
        if (records[i].userId == u.id && records[i].accountNbr == accountNbr)
        {
            found = 1;
            int choice;
        invalidChoice:
            choice = readInt("Update (1) phone or (2) country? ");

            if (choice ==1)
            {
                do
                {
                    printf("New phone: ");
                    fgets(records[i].phone, sizeof(records[i].phone), stdin);
                    records[i].phone[strcspn(records[i].phone, "\n")] = '\0';

                    if (strlen(records[i].phone) == 0)
                    {
                        printf(RED "Phone number cannot be empty!\n" RESET);
                    }
                    } while (strlen(records[i].phone) == 0);
                    if (!updateRecord(&records[i]))
                    {
                        printf(RED "Error updating record in database!\n" RESET);
                    return;
                    }
            }
            else if (choice ==2)
            {
                do
                {
                printf("Enter the country:");
                fgets(records[i].country, sizeof(records[i].country), stdin);
                records[i].country[strcspn(records[i].country, "\n")] = '\0';

                if (strlen(records[i].country) == 0)
                {
                    printf(RED "Country cannot be empty!\n" RESET);
                }
             } while (strlen(records[i].country) == 0);
              if (!updateRecord(&records[i]))
                    {
                        printf(RED "Error updating record in database!\n" RESET);
                    return;
                    }
            }
             else
            {
                printf(RED "Insert a valid operation!\n" RESET);
                goto invalidChoice;
            }
             
            saveAllRecords(records, count);
            break;
            }
        }
        if (found)
        {
            stayOrReturn(found,  u);
            return;
        }
        else
        {
            int result = stayOrReturn(found, u);
            if (result == 0)
            continue;
        }
    }
}

void makeTransaction(struct User u)
{
    int accountNbr;

    while(1)
    {
    system("clear");
    accountNbr = readInt("Enter account number: ");
    system("clear");

    struct Record records[100];
    int count = loadAllRecordsFromTxt(records);
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
                    printf(RED "Transactions are not allowed for fixed accounts!\n" RESET);
                   break;
                }
            int choice;
            int changed = 0;
        invalidChoice:
            choice = readInt("Do you want to (1) deposit or (2) withdraw? ");

            if (choice == 1)
            {
                double depositAmount;
                printf("Enter amount to deposit: $");
                scanf("%lf", &depositAmount);
               
                if (depositAmount <= 0)
                {
                    printf(RED "Amount must be positive!\n" RESET);
                    goto invalidChoice;
                }
            
                records[i].amount += depositAmount;
                changed = 1;
                printf(GREEN "Deposited $%.2f successfully!\n" RESET, depositAmount);

            }
            else if (choice == 2)
            {
                double withdrawAmount;
                printf("Enter amount to withdraw: $");
                scanf("%lf", &withdrawAmount);
                
                if (withdrawAmount <= 0)
                {
                    printf(RED "Amount must be positive!\n" RESET);
                    goto invalidChoice;
                }

                if (withdrawAmount > records[i].amount + 0.001)
                {
                    printf(RED "Insufficient funds! Current balance: $%.2f\n" RESET, records[i].amount);
                    goto invalidChoice;
                }
                records[i].amount -= withdrawAmount;
                changed = 1;
                printf(GREEN "Withdrew $%.2f successfully!\n" RESET, withdrawAmount);
            }
            else
            {
                printf(RED "Insert a valid operation!\n" RESET);
                goto invalidChoice;
            }
        if (changed)
        {
             if (!updateRecord(&records[i]))
                    {
                        printf(RED "Error updating record in database!\n" RESET);
                    return;
                    }
            saveAllRecords(records, count);
        }
            break;
        }
    }
        if (found)
        {
            stayOrReturn(found,  u);
            return;
        }
        else
        {
            int result = stayOrReturn(found, u);
            if (result == 0)
            continue;
        }
    }
}

void removeAccount(struct User u)
{
    int accountNbr;

    while(1)
    {
    system("clear");
    accountNbr = readInt("Enter account number: ");
    system("clear");

    struct Record records[100];
    int count = loadAllRecordsFromTxt(records);
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (records[i].userId == u.id && records[i].accountNbr == accountNbr)
        {
            found = 1;
            int choice;
        invalidChoise:
            choice = readInt("Are you sure you want to delete this account? (1) Yes (2) No: ");

            if (choice == 1)
            {    
              int idToDelete = records[i].id;       
              for (int j = i; j < count - 1; j++)
            {
                records[j] = records[j + 1];
            }
            count--;
            if (!deleteRecord(idToDelete))
                    {
                        printf(RED "Error deleting record from database!\n" RESET);
                    return;
                    }
            saveAllRecords(records, count);
            printf(GREEN "Account removed successfully!\n" RESET);
        }
        else if (choice == 2)
        {
            printf(CYAN "Account removal canceled.\n" RESET);
        }
        else
        {
            printf(RED "Insert a valid operation!\n" RESET);
                goto invalidChoise;
        }
            break;
        }
    }
    if (found)
    {
    stayOrReturn(found,  u);
    return;
    }
    else
    {
        int result = stayOrReturn(found, u);
        if (result == 0)
        continue;
       } 
    }
} 

void transferOwnership (struct User u)
{
    while(1)
    {
    int accountNbr;
    system("clear");
    accountNbr = readInt("Enter account number: ");
    system("clear");

    struct Record records[100];
    int count = loadAllRecordsFromTxt(records);
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (records[i].userId == u.id && records[i].accountNbr == accountNbr)
        {
            found = 1;
            char newOwnerName[50];
            printf("Enter the name of the new owner: ");
            fgets(newOwnerName, sizeof(newOwnerName), stdin);
            newOwnerName[strcspn(newOwnerName, "\n")] = '\0';

            // Self-transfer check
            if (strcmp(newOwnerName, u.name) == 0)
            {
                printf(CYAN "😊 This account is already yours — no need to transfer it to yourself!\n" RESET);
                break;
            }
            //Lookup the new owner using the exixting loadAllUsers()
            struct User allUsers[100];
            int userCount = loadAllUsersFromTxt(allUsers);
            int ownerIndex = -1;

            for (int j = 0; j< userCount; j++)
            {
                if (strcmp(allUsers[j].name, newOwnerName) == 0)
                {
                    ownerIndex = j;
                    break;
                }
            }
            if (ownerIndex == -1)
            {
                printf(RED "✖ No user found with the username \"%s\"!\n" RESET, newOwnerName);
                break;
            }

            int choice;
            invalidChoice:
            char promptMsg[150];
            sprintf(promptMsg, "Are you sure you want to transfer this account to %s? (1) Yes (2) No: ", newOwnerName);
            choice = readInt(promptMsg);
            if (choice == 1)
            {
                records[i].userId = allUsers[ownerIndex].id;
                strcpy(records[i].name, allUsers[ownerIndex].name);
                if (!updateRecord(&records[i]))
                    {
                        printf(RED "Error updating record in database!\n" RESET);
                    return;
                    }

                saveAllRecords(records, count);
                 printf(GREEN "✔ Ownership of account #%d transferred to %s!\n" RESET, accountNbr, allUsers[ownerIndex].name);
            
            char fifoPath[100]; 
            sprintf (fifoPath, "./data/%s.fifo", newOwnerName);

            int fd = open (fifoPath, O_WRONLY | O_NONBLOCK);
            if (fd == -1)
            {
                printf(CYAN "i %s is not online - they'll see the change when they log in.\n" RESET, newOwnerName);
            }
            else
            {
            char message[256];
            sprintf(message, "Account #%d has been transferred to you by %s.", accountNbr, u.name);
                write(fd, message, strlen(message));
                close(fd);

        }
    }
            else if (choice ==2)
            {
                printf(CYAN "Transfer cancelled - nothing was changed.\n" RESET);
            }
            else
            {
                printf(RED "Insert a valid operation!\n" RESET);
                goto invalidChoice;
            }
            break;
        }
    }
    if (found)
    {
     stayOrReturn(found,  u);
     return;
    }
    else
    {
        int result = stayOrReturn(found, u);
        if (result == 0)
        continue;
    }
    }
}

int startListener(struct User u)
{
    char fifoPath[100]; 
    sprintf (fifoPath, "./data/%s.fifo", u.name);
  
if (mkfifo (fifoPath, 0666) == -1 && errno != EEXIST)
    {
        perror ("mkfifo failed");
        return -1 ;
    }
int fd = open (fifoPath, O_RDONLY);
if (fd == -1)
    {
        perror ("open failed");
        return -1 ;
    }
    char buffer[256];
   
    while (1)
    {
        int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            printf(GREEN "\n🔔 %s\n" RESET, buffer);
        }
        else if (bytesRead ==0)
        {
        close(fd);
        fd = open(fifoPath, O_RDONLY);
        }
    }
}   
