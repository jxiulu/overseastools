cmake_minimum_required(VERSION 3.28)
project(sakugatranslator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)

find_package(CURL REQUIRED)
include_directories(lib/nlohmann)

add_executable(main src/main.cpp)
target_link_libraries(main CURL::libcurl)