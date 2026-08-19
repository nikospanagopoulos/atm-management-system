#include "header.h"

sqlite3 *db = NULL;

int initDatabase(void)
{
    int rc = sqlite3_open("data/atm.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id INTEGER PRIMARY KEY,"
        "  name TEXT UNIQUE,"
        "  password TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS records ("
        "  id INTEGER PRIMARY KEY,"
        "  user_id INTEGER,"
        "  name TEXT,"
        "  account_nbr INTEGER,"
        "  deposit_month INTEGER,"
        "  deposit_day INTEGER,"
        "  deposit_year INTEGER,"
        "  country TEXT,"
        "  phone TEXT,"
        "  amount REAL,"
        "  account_type TEXT,"
        "  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

    char *errMsg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }

    return 1;
}

int loadAllRecords(struct Record records[])
{
    sqlite3_stmt *stmt;
    const char *sql =
        "SELECT id, user_id, name, account_nbr, "
        "deposit_month, deposit_day, deposit_year, "
        "country, phone, amount, account_type FROM records";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int index = 0;
    while (index < 100 && sqlite3_step(stmt) == SQLITE_ROW)
    {
        records[index].id = sqlite3_column_int(stmt, 0);
        records[index].userId = sqlite3_column_int(stmt, 1);

        const unsigned char *nameText = sqlite3_column_text(stmt, 2);
        strncpy(records[index].name, (const char *)nameText, sizeof(records[index].name) - 1);
        records[index].name[sizeof(records[index].name) - 1] = '\0';

        records[index].accountNbr = sqlite3_column_int(stmt, 3);

        records[index].deposit.month = sqlite3_column_int(stmt, 4);
        records[index].deposit.day   = sqlite3_column_int(stmt, 5);
        records[index].deposit.year  = sqlite3_column_int(stmt, 6);

        const unsigned char *countryText = sqlite3_column_text(stmt, 7);
        strncpy(records[index].country, (const char *)countryText, sizeof(records[index].country) - 1);
        records[index].country[sizeof(records[index].country) - 1] = '\0';

        const unsigned char *phoneText = sqlite3_column_text(stmt, 8);
        strncpy(records[index].phone, (const char *)phoneText, sizeof(records[index].phone) - 1);
        records[index].phone[sizeof(records[index].phone) - 1] = '\0';

        records[index].amount = sqlite3_column_double(stmt, 9);

        const unsigned char *typeText = sqlite3_column_text(stmt, 10);
        strncpy(records[index].accountType, (const char *)typeText, sizeof(records[index].accountType) - 1);
        records[index].accountType[sizeof(records[index].accountType) - 1] = '\0';

        index++;
    }

    sqlite3_finalize(stmt);
    return index;
}