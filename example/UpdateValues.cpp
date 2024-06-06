#include <iostream>
#include <Database/DatabaseAPI.hpp>
#include <unordered_map>
#include <filesystem>
int main(int argc, char* argv[])
{
    #if defined(__APPLE__)
        std::filesystem::path current_dir = argv[0];
        std::filesystem::current_path(current_dir.parent_path().generic_string());
    #endif
    DB::DatabaseValues values = {{"Version","0.3"}};
    DB::DatabaseValues parameters  = {{"Version","0.1"}};
    DB::Database database;
    std::string ProjectFolder;
    std::string DatabasePath;
    ProjectFolder = std::filesystem::current_path().generic_string();
    DatabasePath = ProjectFolder + "/test/DB/Versions.db";
    database.open(&DatabasePath);
    // Values and parameters must be type of std::map<std::string, std::string> or DB::DatabaseValues
    database.UpdateRowInTable("macOSVersions",values,parameters);
    return 0;
}