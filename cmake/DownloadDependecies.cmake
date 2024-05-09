include(FetchContent)

# jsoncpp
# FetchContent_Declare(jsoncpp
# GIT_REPOSITORY https://github.com/DeepForge-Technology/jsoncpp.git
# GIT_TAG master
# )

# FetchContent_MakeAvailable(jsoncpp)

# SQLite
FetchContent_Declare(SQLite
  GIT_REPOSITORY https://github.com/DeepForge-Technology/SQLite.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SQLite)