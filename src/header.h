#ifndef HEADER_H
#define HEADER_H
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"
#define COMBINED_BUFFER_SIZE 100
#define SALT_HEX_LEN 32
#define MAX_PASSWORD_LEN (COMBINED_BUFFER_SIZE - SALT_HEX_LEN - 1)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sqlite3.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
extern sqlite3 *db; // Declare the global database pointer

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
    char password[COMBINED_BUFFER_SIZE];

};

// authentication functions

/**
 * Opens (or creates) the SQLite database file at data/atm.db and
 * creates the 'users' and 'records' tables if they don't already exist.
 * Must be called once at program startup, before any login/register.
 * Returns 1 on success, 0 on failure.
 */
int initDatabase(void);
/**
 * Loads all records from the SQLite 'records' table into the given array.
 * Returns the number of records loaded.
 */
int loadAllRecords(struct Record records[]);
/**
 * Loads all users from the SQLite 'users' table into the given array.
 * Returns the number of users loaded.
 */
int loadAllUsers(struct User users[]);
/**
 * Inserts a new user into the SQLite 'users' table.
 * The caller is responsible for setting a unique user->id beforehand.
 * Returns 1 on success, 0 on failure.
 */
int insertUser(struct User *user);
/**
 * Updates an existing user's name and password in the SQLite 'users' table,
 * matched by user->id.
 * Returns 1 on success, 0 on failure.
 */
int updateUser(struct User *user);
/**
 * Inserts a new record into the SQLite 'records' table.
 * The caller is responsible for setting a unique record->id beforehand.
 * Returns 1 on success, 0 on failure.
 */
int insertRecord(struct Record *record);
/**
 * Updates an existing record's fields in the SQLite 'records' table,
 * matched by record->id.
 * Returns 1 on success, 0 on failure.
 */
int updateRecord(struct Record *record);
/**
 * Deletes the user with the given id from the SQLite 'users' table.
 * Returns 1 on success, 0 on failure.
 */
int deleteUser(int id);

/**
 * Deletes the record with the given id from the SQLite 'records' table.
 * Returns 1 on success, 0 on failure.
 */
int deleteRecord(int id);

/**
 * One-time migration: copies any users that exist in users.txt but not yet
 * in the SQLite 'users' table into the database. Safe to call on every
 * program startup (idempotent — skips users whose id already exists in db).
 */
void migrateUsersToDb(void);

/**
 * One-time migration: copies any records that exist in records.txt but not
 * yet in the SQLite 'records' table into the database. Safe to call on
 * every program startup (idempotent — skips records whose id already
 * exists in db).
 */
void migrateRecordsToDb(void);

/**
 * Migrates an old plaintext password file to the new salted hash format.
 * Reads from 'stored' and writes to 'newFile', then renames newFile to stored.
 * Returns 1 on success, 0 on failure.
 */
int migration(const char *stored, const char *newFile);

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
int loadAllUsersFromTxt (struct User *out);

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
 * Converts raw bytes into a lowercase hex string.
 * @param bytes   the raw byte array to convert
 * @param len     number of bytes in the array
 * @param output  destination buffer, must be at least (len*2 + 1) chars
 */
void bytesToHex(const unsigned char *bytes, int len, char *output);

/**
 * Generates a cryptographically secure random salt, hex-encoded.
 * @param output_hex  destination buffer, must be at least 33 chars (16*2 + 1)
 */
void generateSalt(char *output_hex);

/**
 * Computes the SHA-256 hash of a string, hex-encoded.
 * @param input       the string to hash (e.g. salt+password concatenated)
 * @param output_hex  destination buffer, must be at least 65 chars (32*2 + 1)
 */
void sha256Hash(const char *input, char *output_hex);

/**
 * Hashes a password with a fresh random salt, producing "salt:hash" in hex.
 * @param password  the plaintext password to hash
 * @param output    destination buffer, must be at least 100 chars (see COMBINED_BUFFER_SIZE)
 */
void hashPassword(const char *password, char *output);

/**
 * Verifies a plaintext password against a stored hash.
 * @param password  the plaintext password to verify
 * @param stored    the stored hash (salt:hash)
 * @return  1 if the password is correct, 0 otherwise
 */
int verifyPassword(const char *password, const char *stored);

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
int loadAllRecordsFromTxt(struct Record *out);

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
 * Child-process listener: reads transfer notifications from the user's
 * FIFO and prints them in real time. Runs until the process exits.
*/
 int startListener(struct User u);

 /**
 * Shared post-action prompt used by every feature function. If notGood is 0
 * (record not found), offers retry/main-menu/exit. Otherwise offers
 * main-menu/exit only. Calls f(u) to retry the same feature if requested.
 */
int stayOrReturn(int notGood, struct User u);

/**
 * Reads an integer from stdin, reprompting on invalid (non-numeric) input.
 * Always clears the input buffer up to the next newline.
 */
int readInt(const char *prompt);

/**
 * Shared success prompt shown after createNewAcc/checkAllAccounts complete.
 * Offers main-menu/exit.
 */
void success(struct User u);
#endif