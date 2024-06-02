#include <string>
#include <gtest/gtest.h>
#include <Database/DatabaseAPI.hpp>
#include <filesystem>
#include <unordered_map>
#include <fstream>

class DatabaseTest : public ::testing::Test
{
protected:
    std::string DatabasePath;
    DB::Database database;
    std::string NameApp;
    std::string Windows_Command;
    std::string macOS_Command;
    std::string Linux_Command;
    std::string Table;

    void SetUp() override
    {
        DatabasePath = std::filesystem::current_path().generic_string() + "/DB/AppInstaller.db";
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
    int RESULT;
    std::unordered_map<std::string, std::string> columns;
    columns = {{"Name","TEXT"},{"Windows", "TEXT"}, {"Linux", "TEXT"}, {"macOS", "TEXT"}};
    RESULT = database.CreateTable(Table, columns);
    EXPECT_EQ(RESULT, 0);

    columns = {{"Number","TEXT"},{"Language",  "TEXT"}};
    RESULT = database.CreateTable("DevelopmentPacks", columns);
    EXPECT_EQ(RESULT, 0);
}

TEST_F(DatabaseTest, CreateValue)
{
    int RESULT;
    std::unordered_map<std::string, std::string> values;
    RESULT = database.InsertApplicationsToTable(Table, NameApp, Windows_Command, macOS_Command, Linux_Command);
    EXPECT_EQ(RESULT, 0);
    values  = {{"Number","1"}, {"Language", "PythonDevelopmentTools"}};
    RESULT = database.InsertValuesToTable("DevelopmentPacks", values);
    EXPECT_EQ(RESULT, 0);
}

TEST_F(DatabaseTest, GetValue)
{
    std::string RESULT;
    RESULT = database.GetValueFromDB(Table, NameApp, "Windows");
    EXPECT_STREQ(Windows_Command.c_str(), RESULT.c_str());
    RESULT = database.GetValueFromDB(Table, NameApp, "Linux");
    EXPECT_STREQ(Linux_Command.c_str(), RESULT.c_str());
    RESULT = database.GetValueFromDB(Table, NameApp, "macOS");
    EXPECT_STREQ(macOS_Command.c_str(), RESULT.c_str());
}

TEST_F(DatabaseTest, GetAllValues)
{
    std::unordered_map<std::string, std::string> Packages = database.GetAllValuesFromDB("Test", "Windows");
    EXPECT_LE(1, Packages.size());
}

TEST_F(DatabaseTest, GetDevPack)
{
    std::unordered_map<std::string, std::string> DevelopmetPacks = database.GetDevPackFromDB("DevelopmentPacks", "Language");
    EXPECT_LE(1, DevelopmetPacks.size());
}

TEST_F(DatabaseTest, UpdateValues)
{
    int RESULT;
    std::string RESULT_STR;
    std::unordered_map<std::string, std::string> values;
    std::unordered_map<std::string, std::string> parameters;
    values = {{"Windows", "Updated_Test_Windows_Command"}, {"Linux", "Updated_Test_Linux_Command"}, {"macOS", "Updated_Test_macOS_Command"}};
    parameters = {{"Windows", Windows_Command}, {"Linux", Linux_Command}, {"macOS", macOS_Command}};
    RESULT = database.UpdateValuesInTable(Table, values, parameters);
    EXPECT_EQ(0, RESULT);

    RESULT_STR = database.GetValueFromDB(Table, NameApp, "Windows");
    EXPECT_STREQ(values["Windows"].c_str(), RESULT_STR.c_str());

    RESULT_STR = database.GetValueFromDB(Table, NameApp, "Linux");
    EXPECT_STREQ(values["Linux"].c_str(), RESULT_STR.c_str());

    RESULT_STR = database.GetValueFromDB(Table, NameApp, "macOS");
    EXPECT_STREQ(values["macOS"].c_str(), RESULT_STR.c_str());
}

TEST_F(DatabaseTest, DeleteValue)
{
    int RESULT = database.RemoveApplicationFromTable(Table, NameApp);
    EXPECT_EQ(0, RESULT);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}