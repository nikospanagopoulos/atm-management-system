#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    char phone[20];
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];

};

// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);
int loadAllUsers (struct User *out);
int isSystemLocked(void);
int isUserLocked(char *name);
int lockUser(char *name);
int loginUser(struct User *u);
int registerUser(struct User *u);

// system function
void createNewAcc(struct User u);
int loadAllRecords(struct Record *out);
void saveAllRecords(struct Record *records, int count);
void mainMenu(struct User u);
void checkAccountDetails(struct User u);
void checkAllAccounts(struct User u);
#endif