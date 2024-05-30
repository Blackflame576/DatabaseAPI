#include <string>
#include <gtest/gtest.h>
#include <Database/DatabaseAPI.hpp>
#include <filesystem>
#include <unordered_map>

using namespace std;
using namespace DB;

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        DB_PATH = std::filesystem::current_path().generic_string() + "/DB/AppInstaller.db";
        database.open(&DB_PATH);
        NameApp = "TestApp";
        Windows_Command = "Test_Windows_Command";
        macOS_Command = "Test_macOS_Command";
        Linux_Command = "Test_Linux_Command";
        Table = "Test";
    }

    string DB_PATH;
    Database database;
    string NameApp;
    string Windows_Command;
    string macOS_Command;
    string Linux_Command;
    string Table;
};

TEST_F(DatabaseTest, CreateValue) {
    int RESULT = database.InsertApplicationsToTable(Table, NameApp, Windows_Command, macOS_Command, Linux_Command);
    EXPECT_EQ(RESULT, 0);
}

TEST_F(DatabaseTest, GetValue) {
    string RESULT;
    RESULT = database.GetValueFromDB(Table, NameApp, "Windows");
    EXPECT_STREQ(Windows_Command.c_str(), RESULT.c_str());
    RESULT = database.GetValueFromDB(Table, NameApp, "Linux");
    EXPECT_STREQ(Linux_Command.c_str(), RESULT.c_str());
    RESULT = database.GetValueFromDB(Table, NameApp, "macOS");
    EXPECT_STREQ(macOS_Command.c_str(), RESULT.c_str());
}

TEST_F(DatabaseTest, DeleteValue) {
    int RESULT = database.RemoveApplicationFromTable(Table, NameApp);
    EXPECT_EQ(0, RESULT);
}

TEST_F(DatabaseTest, GetAllValues) {
    unordered_map<string, string> Packages = database.GetAllValuesFromDB("Applications", "Windows");
    EXPECT_LE(1, Packages.size());
}

TEST_F(DatabaseTest, GetDevPack) {
    unordered_map<string, string> DevelopmetPacks = database.GetDevPackFromDB("DevelopmentPacks", "Language");
    EXPECT_LE(1, DevelopmetPacks.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}