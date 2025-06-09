cmake_minimum_required(VERSION 3.31)
project(todoot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXECUTABLE_NAME todoot)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${EXECUTABLE_NAME} main.cpp
        src/database/DatabaseManager.cpp
        src/handlers/TodoHandlers.cpp
)

set(STATIC_FILES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/static)
set(STATIC_FILES_DEST_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/static) # Or CMAKE_BINARY_DIR for single-config
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(STATIC_FILES_DEST_DIR ${CMAKE_BINARY_DIR}/static)
endif()
if(EXISTS ${STATIC_FILES_SOURCE_DIR})
    add_custom_command(
            TARGET todoot POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different # Only copy if changed
            ${STATIC_FILES_SOURCE_DIR}
            ${STATIC_FILES_DEST_DIR}
            COMMENT "Copying static assets to output directory"
    )
endif()

#include(FetchContent)
#
#FetchContent_Declare(
#        sqlite_orm
#        GIT_REPOSITORY https://github.com/fnc12/sqlite_orm.git
#        GIT_TAG v1.9
#)
#FetchContent_MakeAvailable(sqlite_orm)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        ZLIB::ZLIB

)
add_subdirectory(vendor/sqlite_orm)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        Threads::Threads
        Crow::Crow
        SQLite::SQLite3
        sqlite_orm::sqlite_orm
)
set(CROW_ENABLE_SSL ON CACHE BOOL "Enable SSL support in Crow") # Off for simplicity
set(CROW_ENABLE_COMPRESSION ON CACHE BOOL "Enable HTTP compression in Crow") # Off for simplicity





