#include <cstring>
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
    std::string DevelopmentTable;

    void SetUp() override
    {
        ProjectFolder = std::filesystem::current_path().generic_string();
        DatabasePath = ProjectFolder + "/test/DB/AppInstaller.db";
        NameApp = "TestApp";
        Windows_Command = "Test_Windows_Command";
        macOS_Command = "Test_macOS_Command";
        Linux_Command = "Test_Linux_Command";
        Table = "Test";
        DevelopmentTable = "DevelopmentPacks";
        if (!std::filesystem::exists(DatabasePath))
        {
            std::ofstream file(DatabasePath);
            file << "";
            file.close();
        }
        database.open(&DatabasePath);
    }
};