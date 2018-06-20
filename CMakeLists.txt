cmake_minimum_required(VERSION 3.2)

macro(include_boost)
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")

    # Include 'boost' directory
    include_directories(${Boost_INCLUDE_DIR})
endmacro()

macro(setup_boost)
    set(BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/boost_1_67_0")
    set(BOOST_BINARY_DIR "${CMAKE_BINARY_DIR}/boost-build")
    set(BOOST_ROOT ${BOOST_BINARY_DIR})
    set(BOOST_COMPONENTS "date_time")

    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)

    find_package(Boost 1.67.0 COMPONENTS ${BOOST_COMPONENTS})
    if(Boost_FOUND)
        include_boost()
    else()
        message(status "** Boost couldn't be found. Don't worry, let's compile it ; )")

        # Compile ${BOOST_COMPONENTS}
        execute_process(COMMAND bash -c "./bootstrap.sh --prefix=${BOOST_BINARY_DIR} --with-libraries=${BOOST_COMPONENTS}" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
        if(result)
            message(FATAL_ERROR "CMake step for boost failed: '${result}'")
        endif()
        execute_process(COMMAND bash -c "./b2 install -j 4" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
        if(result)
            message(FATAL_ERROR "CMake step for boost failed: '${result}'")
        endif()

        # re-find package to set the Boost_* variables
        find_package(Boost 1.67.0 COMPONENTS ${BOOST_COMPONENTS})
        if(NOT Boost_FOUND)
            message(FATAL_ERROR "Something wrong happend, couldn't find boost after compile it.")
        endif()

        # include Boost
        include_boost()
    endif()
endmacro()

macro(setup_sqlite)
    set(SQLITE_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/sqlite-autoconf-3240000")
    include_directories(${SQLITE_LIB_DIR})    # -I compilation command
    file(GLOB sqlite_src "${SQLITE_LIB_DIR}/sqlite3.o")
    add_library(sqlite3 STATIC ${SQLITE_LIB_DIR}/sqlite3.c)
#    set(sqlite3 -lthread,-ldl)
#    target_sources(${PROJECT_NAME} "${SQLITE_LIB_DIR}/sqlite3.c")
endmacro()

# Configure project
project(CppSqlLiteDbAdapter)

# Setup Boost libraries
setup_boost()

# Setup SQLite
setup_sqlite()
message(STATUS "** sqlite files: ${sqlite_src}")

# include
include_directories(${CMAKE_SOURCE_DIR}/include)    # -I compilation command

# Looking for files
file(GLOB_RECURSE src "src/*.cpp")
file(GLOB_RECURSE hdr "include/*.h")

add_executable(${PROJECT_NAME} "main.cpp" ${src} ${hdr} ${sqlite_src})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} sqlite3  -lpthread -ldl)




