#include <string>
#include <gtest/gtest.h>
#include <Database/DatabaseAPI.hpp>
#include <filesystem>
#include <unordered_map>
#include <fstream>

class DatabaseTest : public ::testing::Test
{
protected:
    std::string ProjectFolder;
    std::string DatabasePath;
    DB::Database database;
    std::string NameApp;
    std::string Windows_Command;
    std::string macOS_Command;
    std::string Linux_Command;
    std::string Table;

    void SetUp() override
    {
        ProjectFolder = std::filesystem::current_path().generic_string();
        DatabasePath = ProjectFolder + "/test/DB/AppInstaller.db";
        NameApp = "TestApp";
        Windows_Command = "Test_Windows_Command";
        macOS_Command = "Test_macOS_Command";
        Linux_Command = "Test_Linux_Command";
        Table = "Test";
        if (!std::filesystem::exists(DatabasePath))
        {
            std::ofstream file(DatabasePath);
            file << "";
            file.close();
        }
        database.open(&DatabasePath);
    }
};

TEST_F(DatabaseTest, CreateTable)
{
    int result;
    std::unordered_map<std::string, std::string> columns;
    columns = {{"Name", "TEXT"}, {"Windows", "TEXT"}, {"Linux", "TEXT"}, {"macOS", "TEXT"}};
    result = database.CreateTable(Table, columns);
    EXPECT_EQ(result, 0);

    columns = {{"Number", "TEXT"}, {"Language", "TEXT"}};
    result = database.CreateTable("DevelopmentPacks", columns);
    EXPECT_EQ(result, 0);
}

TEST_F(DatabaseTest, CreateValue)
{
    int result;
    std::unordered_map<std::string, std::string> values;
    values = {{"Name", NameApp}, {"Windows", Windows_Command}, {"Linux", Linux_Command}, {"macOS", macOS_Command}};
    result = database.InsertValueToTable(Table, values);
    EXPECT_EQ(result, 0);
    values = {{"Number", "1"}, {"Language", "PythonDevelopmentTools"}};
    result = database.InsertValueToTable("DevelopmentPacks", values);
    EXPECT_EQ(result, 0);
}

TEST_F(DatabaseTest, GetValue)
{
    std::string result;
    result = database.GetValueFromDB(Table, NameApp, "Windows");
    EXPECT_STREQ(Windows_Command.c_str(), result.c_str());
}

TEST_F(DatabaseTest, GetAllValues)
{
    std::unordered_map<std::string, std::string> db_values;
    bool expression;
    db_values = database.GetAllValuesFromDB(Table);
    expression = Windows_Command == db_values["Windows"] && Linux_Command == db_values["Linux"] && macOS_Command == db_values["macOS"];
    EXPECT_TRUE(expression, true);
}

TEST_F(DatabaseTest, GetDevPack)
{
    std::unordered_map<std::string, std::string> DevelopmetPacks = database.GetDevPackFromDB("DevelopmentPacks", "Language");
    EXPECT_LE(1, DevelopmetPacks.size());
}

TEST_F(DatabaseTest, UpdateValues)
{
    int result;
    bool expression;
    std::unordered_map<std::string, std::string> db_values;
    std::unordered_map<std::string, std::string> values;
    std::unordered_map<std::string, std::string> parameters;
    values = {{"Windows", "Updated_Test_Windows_Command"}, {"Linux", "Updated_Test_Linux_Command"}, {"macOS", "Updated_Test_macOS_Command"}};
    parameters = {{"Windows", Windows_Command}, {"Linux", Linux_Command}, {"macOS", macOS_Command}};
    result = database.UpdateValuesInTable(Table, values, parameters);
    db_values = database.GetAllValuesFromDB(Table);
    expression = result == 0 && values["Windows"] == db_values["Windows"]  && values["Linux"] == db_values["Linux"]  && values["macOS"] == db_values["macOS"];
    EXPECT_TRUE(expression, true);
}

TEST_F(DatabaseTest, RemoveValue)
{
    int result;
    std::unordered_map<std::string, std::string> values;
    values = {{"Name",NameApp}};
    result = database.RemoveValueFromTable(Table, values);
    EXPECT_EQ(0, result);
    
    values = {{"Number", "1"}, {"Language", "PythonDevelopmentTools"}};
    result = database.RemoveValueFromTable("DevelopmentPacks", values);
    EXPECT_EQ(0, result);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}