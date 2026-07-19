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

/**
 * Prompts for username and password with terminal echo disabled for the password.
 * Fills the given buffers directly (legacy helper, superseded by loginUser).
 */
void loginMenu(char a[50], char pass[50]);
/**
 * Looks up the stored password for the given user by scanning users.txt.
 * Returns "no user found" if no matching username exists.
 */
const char *getPassword(struct User u);
/**
 * Loads every user from users.txt into the given array.
 * Returns the number of users loaded.
 */
int loadAllUsers (struct User *out);
/**
 * Checks whether the whole system is locked (system_locked.txt exists).
 * Returns 1 if locked, 0 otherwise.
 */
int isSystemLocked(void);
/**
 * Checks whether a specific username is locked (present in locked_users.txt).
 * Returns 1 if locked, 0 otherwise.
 */
int isUserLocked(char *name);
/**
 * Creates system_locked.txt, locking the whole system after too many
 * failed username attempts.
 */
int lockSystem(void);
/**
 * Appends the given username to locked_users.txt, locking that account
 * after too many failed password attempts.
 */
int lockUser(char *name);
/**
 * Handles the full login flow: username lookup (2 attempts), lock check,
 * and password verification (3 attempts, hidden input).
 * Fills the User struct on success. Returns 1 on success, 0 on failure.
 */
int loginUser(struct User *u);
/**
 * Handles registration: prompts for username/password, checks for duplicates,
 * and appends the new user to users.txt. Returns 1 on success, 0 if username taken.
 */
int registerUser(struct User *u);

// system function

/**
 * Reads one record from an open file stream into the given struct,
 * skipping blank lines. Copies the parsed username into r->name.
 * Returns 1 on success, 0 on parse failure or end of file.
 */
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);
/**
 * Appends a single record to an already-open file stream, in
 * pipe-delimited format.
 */
void saveAccountToFile(FILE *ptr, struct User u, struct Record r);
/**
 * Creates a new account/record for the logged-in user, preventing duplicate
 * account numbers for the same user. Assigns a globally unique record id.
 */
void createNewAcc(struct User u);
/**
 * Loads every record from records.txt into the given array.
 * Returns the number of records loaded.
 */
int loadAllRecords(struct Record *out);
/**
 * Overwrites records.txt with the given array of records.
 */
void saveAllRecords(struct Record *records, int count);
/**
 * Displays the main menu loop and dispatches to the selected feature.
 */
void mainMenu(struct User u);
/**
 * Displays full details and interest projection for a single account
 * belonging to the logged-in user.
 */
void checkAccountDetails(struct User u);
/**
 * Lists every account belonging to the logged-in user.
 */
void checkAllAccounts(struct User u);
/**
 * Lets the logged-in user update the phone or country of one of their accounts.
 */
void updateAccount(struct User u);
/**
 * Handles deposit/withdraw for one account. Blocks fixed-type accounts
 * and withdrawals exceeding the current balance.
 */
void makeTransaction(struct User u);
/**
 * Deletes one account belonging to the logged-in user, after confirmation.
 */
void removeAccount(struct User u);
/**
 * Transfers ownership of one account to another existing user, after
 * confirmation. Blocks self-transfers and unknown usernames.
*/
 void transferOwnership(struct User u);
 /**
 * Shared post-action prompt used by every feature function. If notGood is 0
 * (record not found), offers retry/main-menu/exit. Otherwise offers
 * main-menu/exit only. Calls f(u) to retry the same feature if requested.
 */
void stayOrReturn(int notGood, void f(struct User u), struct User u);
/**
 * Shared success prompt shown after createNewAcc/checkAllAccounts complete.
 * Offers main-menu/exit.
 */
void success(struct User u);
#endif