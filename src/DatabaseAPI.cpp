// Importing a Header File
#include <Database/DatabaseAPI.hpp>

int DB::Database::CreateTable(const std::string &NameTable, DB::DatabaseValues Columns)
{
    std::string SQL_QUERY;
    SQL_QUERY = "CREATE TABLE IF NOT EXISTS " + NameTable + "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\n";
    for (int i = 1; const auto &element : Columns)
    {
        SQL_QUERY = SQL_QUERY + element.first + " " + to_upper(element.second);
        if (i != Columns.size())
        {
            SQL_QUERY += ",";
        }
        i++;
    }
    SQL_QUERY += ")";
    int RESULT_SQL = sqlite3_exec(db, SQL_QUERY.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution CREATE TABLE command: " + errorMessage);
    }
    return 0;
}
int DB::Database::InsertRowToTable(const std::string &NameTable, DB::DatabaseValues Fields)
{
    std::string SQL_QUERY;
    SQL_QUERY = "INSERT INTO " + NameTable + " ";
    std::string Columns = "(";
    std::string Values = "(";
    for (int i = 1; const auto &element : Fields)
    {
        Columns += "\"" + element.first + "\"";
        Values += "\"" + element.second + "\"";
        if (i != Fields.size())
        {
            Columns += ", ";
            Values += ", ";
        }
        i++;
    }
    Columns += ")";
    Values += ")";
    SQL_QUERY += Columns + " VALUES " + Values + ";";
    int RESULT_SQL = sqlite3_exec(db, SQL_QUERY.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution INSERT command: " + errorMessage);
    }
    return 0;
}
bool DB::Database::ExistRowInTable(const std::string &NameTable, const std::string &NameColumn, const std::string &Value)
{
    std::string SQL_QUERY;
    // Create SQL statement
    SQL_QUERY = "SELECT * FROM " + NameTable + " WHERE " + NameColumn + "='" + Value + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        sqlite3_finalize(statement);
        return true;
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return false;
}

std::string DB::Database::GetValueFromRow(const std::string &NameTable, const std::string &NameColumn, std::unordered_map<std::string,std::string> Parameters)
{
    std::string AnswerDB;
    std::string SQL_QUERY;
    // Create SQL statement
    SQL_QUERY = "SELECT " + NameColumn + " FROM " + NameTable;
    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    else
    {
        throw std::logic_error("Parameters is empty");
    }
    SQL_QUERY += ";";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT command: " + errorMessage);
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (std::string(reinterpret_cast<const char *>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

DB::DatabaseValues DB::Database::GetRowByID(const std::string &NameTable,const int &id)
{
    int num_columns;
    int RESULT_SQL;
    DB::DatabaseValues WriteMap;
    std::string Key;
    std::string Value;
    std::string SQL_QUERY;
    
    // Create SQL statement
    SQL_QUERY = "SELECT * FROM " + NameTable + " WHERE id='"  + std::to_string(id)  + "'";
    // Execute SQL statement
    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT * FROM command: " + errorMessage);
    }
    // Execute the SQL statement
    RESULT_SQL = sqlite3_step(statement);
    if (RESULT_SQL!= SQLITE_ROW) {
        throw std::runtime_error("No rows returned");
    }

    // Fetch all rows from the result set
    num_columns = sqlite3_column_count(statement);
    while (RESULT_SQL == SQLITE_ROW) {
        for (int i = 0; i < num_columns; i++) {
            Key = sqlite3_column_name(statement, i);
            Value = reinterpret_cast<const char*>(sqlite3_column_text(statement, i));
            if (Key != "Empty" && Value != "Empty")
            {
                WriteMap.insert(std::pair<std::string, std::string>(Key, Value));
            }
        }
        RESULT_SQL = sqlite3_step(statement);
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}


std::unordered_map<int, DB::DatabaseValues> DB::Database::GetRowFromTable(const std::string &NameTable, DB::DatabaseValues Parameters)
{
    std::unordered_map<int, DB::DatabaseValues> WriteMap;
    int n = 0;
    int RESULT_SQL;
    int num_columns;
    std::string SQL_QUERY;
    std::string id;

    // Create SQL statement
    SQL_QUERY = "SELECT * FROM " + NameTable;
    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    SQL_QUERY += ";";
    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);

    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT * FROM command: " + errorMessage);
    }
    num_columns = sqlite3_column_count(statement);
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        id = (const char *)sqlite3_column_text(statement, 0);
        DB::DatabaseValues row;
        row.insert(std::pair<std::string, std::string>("id", id));
        for (int i = 1; i < num_columns; i++)
        {
            std::string columnName = sqlite3_column_name(statement, i);
            std::string columnValue = (const char *)sqlite3_column_text(statement, i);
            row.insert(std::pair<std::string, std::string>(columnName, columnValue));
        }
        WriteMap.insert(std::pair<int, DB::DatabaseValues>(n, row));
        n++;
    }
    /*example returned WriteMap:
    {0, {
            {"id",1},{"Name","VSCode"},{"Windows","winget install vscode"...}
        }
    },
    {1, {
            {"id",2},{"Name","CMake"},{"Windows","winget install cmake"...}
        }
    }
    */
    sqlite3_finalize(statement);
    return WriteMap;
}

DB::DatabaseValues DB::Database::GetTwoColumnsFromTable(const std::string  &NameTable, const std::string &FirstColumn, const std::string &SecondColumn,DB::DatabaseValues  Parameters)
{
    int num_columns;
    int RESULT_SQL;
    DB::DatabaseValues WriteMap;
    std::string Key;
    std::string Value;
    std::string SQL_QUERY;
    
    // Create SQL statement
    SQL_QUERY = "SELECT " + FirstColumn + ", " + SecondColumn + " FROM " + NameTable;
    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    SQL_QUERY += ";";
    // Execute SQL statement
    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT * FROM command: " + errorMessage);
    }
    // Execute the SQL statement
    RESULT_SQL = sqlite3_step(statement);
    if (RESULT_SQL!= SQLITE_ROW) {
        throw std::runtime_error("No rows returned");
    }

    // Fetch all rows from the result set
    num_columns = sqlite3_column_count(statement);
    while (RESULT_SQL == SQLITE_ROW) {
        for (int i = 0; i < num_columns; i++) {
            Key = (const char *)sqlite3_column_text(statement, 0);
            Value = (const char *)sqlite3_column_text(statement, 1);
            if (Key != "Empty" && Value != "Empty")
            {
                WriteMap.insert(std::pair<std::string, std::string>(Key, Value));
            }
        }
        RESULT_SQL = sqlite3_step(statement);
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

DB::DatabaseValues DB::Database::GetOneColumnFromTable(const std::string  &NameTable, const std::string  &NameColumn, DB::DatabaseValues Parameters)
{
    int num_columns;
    int RESULT_SQL;
    DB::DatabaseValues WriteMap;
    std::string Key;
    std::string Value;
    std::string SQL_QUERY;
    
    // Create SQL statement
    SQL_QUERY = "SELECT id," + NameColumn + " FROM " + NameTable;
    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    SQL_QUERY += ";";
    // Execute SQL statement
    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT * FROM command: " + errorMessage);
    }
    // Execute the SQL statement
    RESULT_SQL = sqlite3_step(statement);
    if (RESULT_SQL!= SQLITE_ROW) {
        throw std::runtime_error("No rows returned");
    }

    // Fetch all rows from the result set
    num_columns = sqlite3_column_count(statement);
    while (RESULT_SQL == SQLITE_ROW) {
        for (int i = 0; i < num_columns; i++) {
            Key = (const char *)sqlite3_column_text(statement, 0);
            Value = (const char *)sqlite3_column_text(statement, 1);
            if (Key != "Empty" && Value != "Empty")
            {
                WriteMap.insert(std::pair<std::string, std::string>(Key, Value));
            }
        }
        RESULT_SQL = sqlite3_step(statement);
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

std::unordered_map<int, DB::DatabaseValues> DB::Database::GetAllRowsFromTable(const std::string &NameTable)
{
    std::unordered_map<int, DB::DatabaseValues> WriteMap;
    int n = 0;
    int RESULT_SQL;
    int num_columns;
    std::string SQL_QUERY;
    std::string id;

    SQL_QUERY = "SELECT * FROM " + NameTable;
    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);

    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT * FROM command: " + errorMessage);
    }
    num_columns = sqlite3_column_count(statement);
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        id = (const char *)sqlite3_column_text(statement, 0);
        DB::DatabaseValues row;
        row.insert(std::pair<std::string, std::string>("id", id));
        for (int i = 1; i < num_columns; i++)
        {
            std::string columnName = sqlite3_column_name(statement, i);
            std::string columnValue = (const char *)sqlite3_column_text(statement, i);
            row.insert(std::pair<std::string, std::string>(columnName, columnValue));
        }
        WriteMap.insert(std::pair<int, DB::DatabaseValues>(n, row));
        n++;
    }
    /*example returned WriteMap:
    {0, {
            {"id",1},{"Name","VSCode"},{"Windows","winget install vscode"...}
        }
    },
    {1, {
            {"id",2},{"Name","CMake"},{"Windows","winget install cmake"...}
        }
    }
    */
    sqlite3_finalize(statement);
    return WriteMap;
}

DB::DatabaseValues DB::Database::GetMaxRowFromTable(const std::string &NameTable, const std::string &NameColumn, DB::DatabaseValues Parameters)
{
    int num_columns;
    int RESULT_SQL;
    int tableCount = GetArraySize(NameTable, NameColumn);
    DB::DatabaseValues WriteMap;
    std::string Key;
    std::string Value;
    std::string SQL_QUERY;
    std::string id;

    // Create SQL statement
    SQL_QUERY = "SELECT * FROM " + NameTable + " WHERE " + NameColumn + "=(SELECT max(" + NameColumn + ") FROM " + NameTable;
    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    SQL_QUERY += ");";
    // Execute SQL statement
    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT max() command: " + errorMessage);
    }
    // Fetch all rows from the result set
    num_columns = sqlite3_column_count(statement);
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        id = (const char *)sqlite3_column_text(statement, 0);
        WriteMap.insert(std::pair<std::string, std::string>("id", id));
        for (int i = 1; i < num_columns; i++)
        {
            std::string columnName = sqlite3_column_name(statement, i);
            std::string columnValue = (const char *)sqlite3_column_text(statement, i);
            WriteMap.insert(std::pair<std::string, std::string>(columnName, columnValue));
        }
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

std::string DB::Database::GetMaxValueFromTable(const std::string &NameTable, const std::string &NameColumn, std::unordered_map<std::string,std::string> Parameters)
{
    std::string AnswerDB;
    std::string SQL_QUERY;
    // Create SQL statement
    SQL_QUERY = "SELECT max(" + NameColumn + ") FROM " + NameTable;
    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    else
    {
        throw std::logic_error("Parameters is empty");
    }
    SQL_QUERY += ";";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT max() command: " + errorMessage);
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (std::string(reinterpret_cast<const char *>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

int DB::Database::GetArraySize(const std::string &NameTable, const std::string &NameColumn)
{
    std::string SQL_QUERY;
    // Create SQL statement
    SQL_QUERY = "SELECT count(" + NameColumn + ") " + " FROM " + NameTable;
    // SQL_QUERY = "SELECT count(Windows) FROM Applications";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution SELECT count() command: " + errorMessage);
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        ArraySize = sqlite3_column_int(statement, 0);
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return ArraySize;
}

int DB::Database::RemoveRowFromTable(const std::string &NameTable, DB::DatabaseValues Parameters)
{
    std::string SQL_QUERY;
    SQL_QUERY = "DELETE FROM " + NameTable + " WHERE ";
    for (int i = 1; const auto &element : Parameters)
    {
        SQL_QUERY += element.first + "='" + element.second + "'";
        if (i != Parameters.size())
        {
            SQL_QUERY += " AND ";
        }
        i++;
    }
    SQL_QUERY += ";";
    int RESULT_SQL = sqlite3_exec(db, SQL_QUERY.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution INSERT command: " + errorMessage);
    }
    return 0;
}

int DB::Database::AddApplications(const std::string Tables[])
{
    std::string NameApp;
    std::string Windows_Command;
    std::string macOS_Command;
    std::string Linux_Command;
    DB::DatabaseValues values;
    int RESULT_COMMAND;

    /* The bellow code is checking if the size of the "Tables" vector is greater than or equal to 1. If
    it is, it prompts the user to enter the name, Windows command, macOS command, and Linux command
    for an application. Then, it iterates through each element in the "Tables" vector and calls the
    "InsertApplicationsToTable" function to insert the application information into each table. If
    the insertion is successful (RESULT_COMMAND is 0), it prints a success message indicating that
    the application was added to the corresponding table. */
    if (Tables->size() >= 1)
    {
        std::cout << "Name:";
        std::getline(std::cin, NameApp);
        std::cout << "Windows:";
        std::getline(std::cin, Windows_Command);
        std::cout << "macOS:";
        std::getline(std::cin, macOS_Command);
        std::cout << "Linux:";
        std::getline(std::cin, Linux_Command);

        values = {{"Name", NameApp}, {"Windows", Windows_Command}, {"macOS", macOS_Command}, {"Linux", Linux_Command}};
        for (int i = 0; i < Tables->size(); i++)
        {

            RESULT_COMMAND = InsertRowToTable(Tables[i], values);
            // RESULT_COMMAND = InsertApplicationsToTable(Tables[i], NameApp, Windows_Command, macOS_Command, Linux_Command);
            if (RESULT_COMMAND == 0)
            {
                std::cout << NameApp << " successfully added to " << Tables[i] << std::endl;
            }
        }
    }
    else
    {
        throw std::logic_error("Array is empty");
    }
    return 0;
}

int DB::Database::RemoveApplications(const std::string Tables[])
{
    std::string NameApp;
    std::string Windows_Command;
    std::string macOS_Command;
    std::string Linux_Command;
    DB::DatabaseValues values;
    int RESULT_COMMAND;

    if (Tables->size() >= 1)
    {
        std::cout << "Name:";
        std::getline(std::cin, NameApp);
        std::cout << "Windows:";
        std::getline(std::cin, Windows_Command);
        std::cout << "macOS:";
        std::getline(std::cin, macOS_Command);
        std::cout << "Linux:";
        std::getline(std::cin, Linux_Command);
        values = {{"Name", NameApp}};
        for (int i = 0; i < Tables->size(); i++)
        {
            RESULT_COMMAND = RemoveRowFromTable(Tables[i], values);
            if (RESULT_COMMAND == 0)
                std::cout << NameApp << " successfully removed to " << Tables[i] << std::endl;
        }
    }
    return 0;
}

int DB::Database::DeleteAllRows(const std::string &NameTable)
{
    std::string SQL_QUERY;
    SQL_QUERY = "DELETE FROM " + NameTable + " WHERE EXISTS (SELECT 1 FROM " + NameTable + ");\nUPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = '" + NameTable + "';\nVACUUM;";
    int RESULT_SQL = sqlite3_exec(db, SQL_QUERY.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution TRUNCATE TABLE command: " + errorMessage);
    }
    return 0;
}

int DB::Database::RunQuery(const std::string &SQL_QUERY)
{
    int RESULT_SQL = sqlite3_exec(db, SQL_QUERY.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution " + SQL_QUERY + " command: " + errorMessage);
    }
    return 0;
}

std::unordered_map<int, DB::DatabaseValues> DB::Database::ExecuteQuery(const std::string &SQL_QUERY)
{
    std::unordered_map<int, DB::DatabaseValues> WriteMap;
    int n = 0;
    int RESULT_SQL;
    int num_columns;

    RESULT_SQL = sqlite3_prepare_v2(db, SQL_QUERY.c_str(), SQL_QUERY.length(), &statement, nullptr);

    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution " + SQL_QUERY + " command:  " + errorMessage);
    }
    num_columns = sqlite3_column_count(statement);
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        std::string id = (const char *)sqlite3_column_text(statement, 0);
        DB::DatabaseValues row;
        row.insert(std::pair<std::string, std::string>("id", id));
        for (int i = 1; i < num_columns; i++)
        {
            std::string columnName = sqlite3_column_name(statement, i);
            std::string columnValue = (const char *)sqlite3_column_text(statement, i);
            row.insert(std::pair<std::string, std::string>(columnName, columnValue));
        }
        WriteMap.insert(std::pair<int, DB::DatabaseValues>(n, row));
        n++;
    }
    /*example returned WriteMap:
    {
        {0, {
                {"id",1},{"Name","VSCode"},{"Windows","winget install vscode"...}
            }
        },
        {1, {
                {"id",2},{"Name","CMake"},{"Windows","winget install cmake"...}
            }
        }
    }
    */
    sqlite3_finalize(statement);
    return WriteMap;
}

int DB::Database::UpdateRowInTable(const std::string &NameTable, DB::DatabaseValues Values, DB::DatabaseValues Parameters)
{
    std::string SQL_QUERY;
    SQL_QUERY = "UPDATE " + NameTable + " SET ";
    for (int i = 1; const auto &element : Values)
    {
        SQL_QUERY += element.first + "='" + element.second + "'";
        if (i != Values.size())
        {
            SQL_QUERY += ",";
        }
        i++;
    }

    if (Parameters.size() != 0)
    {
        SQL_QUERY += " WHERE ";
        for (int i = 1; const auto &element : Parameters)
        {
            SQL_QUERY += element.first + "='" + element.second + "'";
            if (i != Parameters.size())
            {
                SQL_QUERY += " AND ";
            }
            i++;
        }
    }
    SQL_QUERY += ";";
    int RESULT_SQL = sqlite3_exec(db, SQL_QUERY.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
    {
        std::string errorMessage = sqlite3_errmsg(db);
        throw std::runtime_error("Error in execution UPDATE command: " + errorMessage);
    }
    return 0;
}