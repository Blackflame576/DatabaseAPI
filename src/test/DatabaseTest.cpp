#include <DatabaseTest.hpp>

TEST_F(DatabaseTest, DeleteAllRows)
{
    bool expression;
    expression = database.DeleteAllRows(Table) == 0 && database.DeleteAllRows(DevelopmentTable)  ==  0;
    EXPECT_TRUE(expression);
}
TEST_F(DatabaseTest, CreateTable)
{
    int result;
    std::unordered_map<std::string, std::string> columns;
    columns = {{"Name", "TEXT"}, {"Windows", "TEXT"}, {"Linux", "TEXT"}, {"macOS", "TEXT"}};
    result = database.CreateTable(Table, columns);
    EXPECT_EQ(result, 0);

    columns = {{"Number", "TEXT"}, {"Language", "TEXT"}};
    result = database.CreateTable(DevelopmentTable, columns);
    EXPECT_EQ(result, 0);
}

TEST_F(DatabaseTest, InsertValue)
{
    int result;
    std::unordered_map<std::string, std::string> values;
    values = {{"Name", NameApp}, {"Windows", Windows_Command}, {"Linux", Linux_Command}, {"macOS", macOS_Command}};
    result = database.InsertRowToTable(Table, values);
    EXPECT_EQ(result, 0);
    values = {{"Number", "1"}, {"Language", "PythonDevelopmentTools"}};
    result = database.InsertRowToTable(DevelopmentTable, values);
    EXPECT_EQ(result, 0);
}

TEST_F(DatabaseTest, GetValue)
{
    std::unordered_map<int, std::unordered_map<std::string, std::string>> db_rows;
    std::unordered_map<std::string, std::string> parameters;
    parameters  = {{"Name", NameApp}};
    db_rows = database.GetRowFromTable(Table, parameters);
    EXPECT_STREQ(Windows_Command.c_str(), db_rows[0]["Windows"].c_str());
}

TEST_F(DatabaseTest, GetRowByID)
{
    std::unordered_map<std::string, std::string> db_values;
    bool expression;
    db_values = database.GetRowByID(Table,1);
    expression = Windows_Command == db_values["Windows"] && Linux_Command == db_values["Linux"] && macOS_Command == db_values["macOS"];
    EXPECT_TRUE(expression);
}

TEST_F(DatabaseTest, GetAllRows)
{
    typedef std::unordered_map<std::string, std::string> db_values;
    std::unordered_map<int,db_values> db_rows;
    bool expression;
    db_rows = database.GetAllRowsFromTable(Table);
    expression = Windows_Command == db_rows[0]["Windows"];
    EXPECT_TRUE(expression);
}

TEST_F(DatabaseTest, ExecuteQuery)
{
    typedef std::unordered_map<std::string, std::string> db_values;
    std::unordered_map<int,db_values> db_rows;
    bool expression;
    db_rows = database.ExecuteQuery("SELECT * FROM " + Table);
    expression = Windows_Command == db_rows[0]["Windows"];
    EXPECT_TRUE(expression);
}

TEST_F(DatabaseTest, RunQuery)
{
    int result;
    bool expression;
    std::unordered_map<std::string, std::string> db_values;
    std::unordered_map<std::string, std::string> values;
    values = {
        {"Windows", "Updated_Test_Windows_Command"}, 
        {"Linux", "Updated_Test_Linux_Command"}, 
        {"macOS", "Updated_Test_macOS_Command"}
    };
    result = database.RunQuery("UPDATE Test SET Windows='Updated_Test_Windows_Command',macOS='Updated_Test_macOS_Command',Linux='Updated_Test_Linux_Command' WHERE Windows='Test_Windows_Command' AND macOS='Test_macOS_Command' AND Linux='Test_Linux_Command';");
    db_values = database.GetRowByID(Table,1);
    expression = result == 0 && values["Windows"] == db_values["Windows"]  && values["Linux"] == db_values["Linux"]  && values["macOS"] == db_values["macOS"];
    EXPECT_TRUE(expression);
}

TEST_F(DatabaseTest, UpdateValues)
{
    int result;
    bool expression;
    std::unordered_map<std::string, std::string> db_values;
    std::unordered_map<std::string, std::string> values;
    std::unordered_map<std::string, std::string> parameters;
    values = {
        {"Windows", "Updated_Test_Windows_Command"}, 
        {"Linux", "Updated_Test_Linux_Command"}, 
        {"macOS", "Updated_Test_macOS_Command"}
    };
    parameters = {{"Windows", Windows_Command}, {"Linux", Linux_Command}, {"macOS", macOS_Command}};
    result = database.UpdateRowInTable(Table, values, parameters);
    db_values = database.GetRowByID(Table,1);
    expression = result == 0 && values["Windows"] == db_values["Windows"]  && values["Linux"] == db_values["Linux"]  && values["macOS"] == db_values["macOS"];
    EXPECT_TRUE(expression);
}

TEST_F(DatabaseTest, RemoveRow)
{
    int result;
    std::unordered_map<std::string, std::string> values;
    values = {{"Name",NameApp}};
    result = database.RemoveRowFromTable(Table, values);
    EXPECT_EQ(0, result);
    
    values = {{"Number", "1"}, {"Language", "PythonDevelopmentTools"}};
    result = database.RemoveRowFromTable(DevelopmentTable, values);
    EXPECT_EQ(0, result);
}

int main(int argc, char **argv)
{
    std::filesystem::path current_dir = argv[0];
    std::filesystem::current_path(current_dir.parent_path().generic_string());
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}