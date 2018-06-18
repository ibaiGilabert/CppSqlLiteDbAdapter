cmake_minimum_required(VERSION 3.2)

project(CppSqlLiteDbAdapter)

# set cpp-db-adapter dir
set(DB_ADAPTER_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp-db-adapter)
include_directories(${DB_ADAPTER_INTERFACE})    # -I compilation command

# looking for files
file(GLOB_RECURSE src "*.cpp")
file(GLOB_RECURSE hdr "*.h")


add_executable(${PROJECT_NAME} ${src} ${hdr})
