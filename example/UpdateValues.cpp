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
    std::unordered_map<std::string, std::string> values = {{"Version","0.3"}};
    std::unordered_map<std::string, std::string> parameters  = {{"Version","0.1"}};
    DB::Database database;
    std::string ProjectFolder;
    std::string DatabasePath;
    ProjectFolder = std::filesystem::current_path().generic_string();
    DatabasePath = ProjectFolder + "/DB/Versions.db";
    database.open(&DatabasePath);
    // Values and parameters must be type of std::map<std::string, std::string> or std::unordered_map<std::string, std::string>
    database.UpdateValuesInTable("macOSVersions",values,parameters);
    return 0;
}