# TASK CARDS — ATM Management System

> Tasks are ordered chronologically: foundations first, features second, bonus last.
> Each card has an ID, estimated difficulty, and a clear done condition.

---

## PHASE 1 — Foundation

---

### TASK-01 · Project Setup
**File:** all files  
**Difficulty:** ⭐  

Set up the project structure exactly as defined in the PRD.

- [ ] Create folders: `data/`, `src/`
- [ ] Create empty files: `main.c`, `auth.c`, `system.c`, `header.h`
- [ ] Create `data/users.txt` and `data/records.txt` with sample data
- [ ] Verify the file tree matches the PRD

**Done when:** `tree` output matches the PRD file system exactly.

---

### TASK-02 · Makefile
**File:** `Makefile`  
**Difficulty:** ⭐⭐  

Write a Makefile that compiles the project with a single `make` command.

- [ ] `make` compiles all `.c` files into a single binary called `atm`
- [ ] `make clean` removes the compiled binary
- [ ] Compiler flags include: `-Wall -Wextra -g`

**Done when:** `make` produces `./atm` with no errors or warnings.

```makefile
# Expected usage
make        # compiles
make clean  # removes binary
./atm       # runs the program
```

---

### TASK-03 · Data Structures & Header
**File:** `header.h`  
**Difficulty:** ⭐  

Define all structs and declare all function signatures.

- [ ] Define `User` struct (id, name, password)
- [ ] Define `Account` struct (id, user_id, username, account_id, date, country, phone, balance, type)
- [ ] Declare all function signatures from PRD section 8
- [ ] Add include guards (`#ifndef HEADER_H`)

**Done when:** all `.c` files can include `header.h` with no compile errors.

```c
#ifndef HEADER_H
#define HEADER_H

typedef struct { ... } User;
typedef struct { ... } Account;

// function declarations here

#endif
```

---

### TASK-04 · File I/O Utilities
**File:** `system.c`  
**Difficulty:** ⭐⭐⭐  

Implement the core file read/write functions. Everything else depends on these.

- [ ] `loadAllUsers(User *out)` — reads all users from `users.txt`, returns count
- [ ] `loadAllRecords(Account *out)` — reads all records from `records.txt`, returns count
- [ ] `saveAllRecords(Account *accounts, int count)` — overwrites `records.txt` with updated data
- [ ] Test manually: load → print → verify output matches file

**Done when:** loading and printing users/records shows correct data from the files.

---

## PHASE 2 — Auth

---

### TASK-05 · Login
**File:** `auth.c`  
**Difficulty:** ⭐⭐  

Implement `loginUser()`.

- [ ] Prompt user for name and password
- [ ] Load users from `users.txt`
- [ ] If match found → fill `User` struct and return 1
- [ ] If no match → display error message, return 0

**Done when:** Alice can login with correct credentials. Wrong password shows error.

---

### TASK-06 · Register
**File:** `auth.c`  
**Difficulty:** ⭐⭐  

Implement `registerUser()`.

- [ ] Prompt user for name and password
- [ ] Load users from `users.txt`
- [ ] If name already exists → display error message
- [ ] If new → assign new id (max existing id + 1), append to `users.txt`

**Done when:**
- Registering `Marcus / q1w2e3r4t5y6` saves correctly to `users.txt`
- Re-registering `Alice` shows error message

---

### TASK-07 · Main Menu Loop
**File:** `main.c`  
**Difficulty:** ⭐⭐  

Wire login/register into a menu and set up the main menu after login.

- [ ] On launch: show welcome screen with options → Login or Register
- [ ] After login: show main menu with all 7 feature options + logout
- [ ] Loop until user selects logout
- [ ] Pass logged-in `User` to all system functions

**Done when:** app launches, user can register/login, and main menu displays correctly.

```
=== Welcome ===
1. Login
2. Register
0. Exit

=== Main Menu (Alice) ===
1. Create account
2. List accounts
3. Check account details
4. Update account
5. Make transaction
6. Remove account
7. Transfer ownership
0. Logout
```

---

## PHASE 3 — Account Features

---

### TASK-08 · Create Account
**File:** `system.c`  
**Difficulty:** ⭐⭐  

Implement `createAccount()`.

- [ ] Prompt: account_id, date (DD/MM/YYYY), country, phone, balance, type
- [ ] Validate type is one of: `current`, `savings`, `fixed01`, `fixed02`, `fixed03`
- [ ] Assign new record id (max existing id + 1)
- [ ] Append to `records.txt`

**Done when:** new account appears in `records.txt` with all fields correct.

---

### TASK-09 · List Accounts
**File:** `system.c`  
**Difficulty:** ⭐  

Implement `listAccounts()`.

- [ ] Load all records from `records.txt`
- [ ] Filter by `user_id` of logged-in user
- [ ] Display each account: account_id, date, country, phone, balance, type

**Done when:** Alice sees only her accounts, Michel sees only his.

---

### TASK-10 · Check Account Details + Interest
**File:** `system.c`  
**Difficulty:** ⭐⭐⭐  

Implement `checkAccountDetails()` and `calculateInterest()`.

- [ ] Prompt user for account_id
- [ ] If not found → display error
- [ ] Display all account fields
- [ ] Calculate and display interest based on type:
  - `current` → "You will not get interests because the account is of type current"
  - `savings` → "You will get $X as interest on day D of every month"
  - `fixed01` → "You will get $X as interest on DD/MM/YYYY" (date + 1 year)
  - `fixed02` → "You will get $X as interest on DD/MM/YYYY" (date + 2 years)
  - `fixed03` → "You will get $X as interest on DD/MM/YYYY" (date + 3 years)

**Done when:** all 4 interest values match exactly:

| Type | Balance | Expected |
|---|---|---|
| savings | $1001.20 | $5.84/month |
| fixed01 | $1001.20 | $40.05 on 10/10/2013 |
| fixed02 | $1001.20 | $100.12 on 10/10/2014 |
| fixed03 | $1001.20 | $240.29 on 10/10/2015 |

---

### TASK-11 · Update Account
**File:** `system.c`  
**Difficulty:** ⭐⭐  

Implement `updateAccount()`.

- [ ] Prompt user for account_id
- [ ] If not found → display error
- [ ] Prompt: update `phone` or `country`?
- [ ] Update the field in memory
- [ ] Call `saveAllRecords()` to persist changes

**Done when:** updated phone/country is visible in the app and in `records.txt`.

---

### TASK-12 · Make Transaction
**File:** `system.c`  
**Difficulty:** ⭐⭐⭐  

Implement `makeTransaction()`.

- [ ] Prompt user for account_id
- [ ] If type is `fixed01`, `fixed02`, or `fixed03` → display error, return
- [ ] Prompt: withdraw or deposit?
- [ ] **Withdraw:** if amount > balance → display error, do not proceed
- [ ] **Deposit:** add amount to balance
- [ ] Call `saveAllRecords()` to persist new balance

**Done when:**
- fixed accounts show error
- withdraw over balance shows error
- deposit and valid withdraw update `records.txt`

---

### TASK-13 · Remove Account
**File:** `system.c`  
**Difficulty:** ⭐⭐  

Implement `removeAccount()`.

- [ ] Prompt user for account_id
- [ ] If not found → display error
- [ ] Remove from accounts array in memory
- [ ] Call `saveAllRecords()` to persist deletion

**Done when:** accounts 834213, 320421, 3214 are removed from the app and `records.txt`.

---

### TASK-14 · Transfer Ownership
**File:** `system.c`  
**Difficulty:** ⭐⭐  

Implement `transferOwnership()`.

- [ ] Prompt user for account_id and target username
- [ ] If account not found → display error
- [ ] If target user not found → display error
- [ ] Update `user_id` and `username` in the account record
- [ ] Call `saveAllRecords()` to persist changes

**Done when:** account 3212 transferred from Alice to Michel, visible in app and `records.txt`.

---

## PHASE 4 — Bonus

---

### TASK-15 · TUI — Colors & Borders
**File:** `main.c`, `system.c`  
**Difficulty:** ⭐⭐  

Improve the terminal interface using ANSI escape codes.

- [ ] Add colors to menus (headers, errors, success messages)
- [ ] Add clear screen between views
- [ ] Format account display in a readable table layout

**Done when:** the interface is visually clean and uses at least colors for errors and success.

```c
// ANSI color codes
#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN  "\033[1;36m"
#define RESET "\033[0m"
```

---

### TASK-16 · Password Encryption
**File:** `auth.c`  
**Difficulty:** ⭐⭐⭐  

Hash passwords before saving to `users.txt`.

- [ ] On register: hash password before saving
- [ ] On login: hash input password and compare with stored hash
- [ ] Use SHA-256 via OpenSSL or implement a simple hash function

**Done when:** `users.txt` shows hashed passwords, not plain text. Login still works correctly.

---

### TASK-17 · SQLite Database
**File:** new file `db.c` + `db.h`  
**Difficulty:** ⭐⭐⭐⭐  

Replace text files with a SQLite database.

- [ ] Install / link `libsqlite3`
- [ ] Create tables: `users` and `accounts`
- [ ] Replace all `loadAllUsers`, `loadAllRecords`, `saveAllRecords` calls with SQL queries
- [ ] Update Makefile to link `-lsqlite3`

**Done when:** app works identically but data is stored in a `.db` file instead of `.txt` files.

```c
// Tables
CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT UNIQUE, password TEXT);
CREATE TABLE accounts (id INTEGER PRIMARY KEY, user_id INTEGER, username TEXT,
    account_id INTEGER, date TEXT, country TEXT, phone TEXT, balance REAL, type TEXT);
```

---

### TASK-18 · Real-time Notification (Pipes + Fork)
**File:** `system.c`, `main.c`  
**Difficulty:** ⭐⭐⭐⭐⭐  

Notify the receiving user instantly when an account is transferred to them.

- [ ] On app launch: `fork()` a child process that watches for incoming notifications
- [ ] On `transferOwnership()`: write a notification message to a named pipe (FIFO)
- [ ] Child process reads from the pipe and prints the alert to the terminal
- [ ] Create the FIFO file in `data/` named after the user (e.g. `data/Michel.fifo`)

**Done when:** with two terminals open (Alice + Michel), transferring account 3212 to Michel causes an instant message to appear in Michel's terminal.

```c
// Named pipe approach
mkfifo("data/Michel.fifo", 0666);  // created on login
// child process reads from it
// transfer function writes to it
```

---

## Summary

| Phase | Tasks | Features |
|---|---|---|
| 1 — Foundation | TASK 01–04 | Setup, Makefile, structs, file I/O |
| 2 — Auth | TASK 05–07 | Login, Register, Main menu |
| 3 — Features | TASK 08–14 | All 7 account operations |
| 4 — Bonus | TASK 15–18 | TUI, encryption, SQLite, pipes |

> Start with TASK-01 and do not skip ahead. Each task builds on the previous one.
