cmake_minimum_required(VERSION 3.2)

macro(include_boost)
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Library dir: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")

    # Include 'boost' directory
    include_directories(${Boost_INCLUDE_DIR})
endmacro()

macro(setup_boost)
    set(BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/boost_1_67_0")
    set(BOOST_BINARY_DIR "${CMAKE_BINARY_DIR}/boost-build")
    set(BOOST_COMPONENTS "date_time")

    set(BOOST_ROOT ${BOOST_BINARY_DIR})
    set(BOOST_INCLUDEDIR "${BOOST_BINARY_DIR}/include")
    set(BOOST_LIBRARYDIR "${BOOST_BINARY_DIR}/lib")

    set(Boost_USE_STATIC_LIBS ON)       # only find static libs
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_DEBUG ON)                 # Set to ON to enable debug output from FindBoost.

    find_package(Boost 1.67.0 COMPONENTS ${BOOST_COMPONENTS})
    if(Boost_FOUND)
        include_boost()
    else()
        message(status "** Boost couldn't be found. Don't worry, let's compile it ; )")

        if (WIN32)
            if (NOT EXISTS ${BOOST_INCLUDEDIR} OR NOT EXISTS ${BOOST_LIBRARYDIR})
                set(BOOTSTRAP_COMMAND "${BOOST_LIB_DIR}/bootstrap.bat")
                set(B2_COMMAND "${BOOST_LIB_DIR}/b2.exe")
                set(B2_ARGS "install" "--prefix=${BOOST_BINARY_DIR}" "--with-${BOOST_COMPONENTS}")

                # Compile ${BOOST_COMPONENTS}
                # https://www.boost.org/doc/libs/1_67_0/more/getting_started/windows.html#or-build-binaries-from-source
                execute_process(COMMAND ${BOOTSTRAP_COMMAND} WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
                if(result)
                    message(FATAL_ERROR "CMake step for boost failed: '${result}'")
                endif()
                execute_process(COMMAND ${B2_COMMAND} ${B2_ARGS} WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
                if(result)
                    message(FATAL_ERROR "CMake step for boost failed: '${result}'")
                endif()
            endif()

            # set Boost_* variables manually
            set(Boost_DIR ${BOOST_ROOT})
            set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${BOOST_INCLUDEDIR})
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${BOOST_LIBRARYDIR})

            set(Boost_INCLUDE_DIR "${BOOST_INCLUDEDIR}/boost-1_67")
            set(Boost_LIBRARY_DIRS ${BOOST_LIBRARYDIR}) # - Default value for Boost_LIBRARY_DIR_RELEASE and Boost_LIBRARY_DIR_DEBUG.
            link_directories(${Boost_LIBRARY_DIRS})

            # set boost libraries manually
            set(BOOST_LIB_TYPE "mt-x64")    # multithread x64
            foreach(lib_boost ${BOOST_COMPONENTS})
                file(GLOB lib_file "${Boost_LIBRARY_DIRS}/libboost_${lib_boost}*${BOOST_LIB_TYPE}*.lib") 
                set(Boost_LIBRARIES ${Boost_LIBRARIES} ${lib_file})
            endforeach(lib_boost)

        elseif(UNIX)
            # Compile ${BOOST_COMPONENTS}
            # https://www.boost.org/doc/libs/1_67_0/more/getting_started/unix-variants.html#or-build-custom-binaries
            execute_process(COMMAND bash -c "${BOOST_LIB_DIR}/bootstrap.sh --prefix=${BOOST_BINARY_DIR} --with-libraries=${BOOST_COMPONENTS}" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
            if(result)
                message(FATAL_ERROR "CMake step for boost failed: '${result}'")
            endif()
            execute_process(COMMAND bash -c "${BOOST_LIB_DIR}/b2 install" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
            if(result)
                message(FATAL_ERROR "CMake step for boost failed: '${result}'")
            endif()

            # re-find package to set the Boost_* variables
            find_package(Boost 1.67.0 COMPONENTS ${BOOST_COMPONENTS})
            if(NOT Boost_FOUND)
                message(FATAL_ERROR "Something went wrong, couldn't find boost after compile it.")
            endif()

        else()
            message(FATAL_ERROR "Unsupported platform.")
        endif()

        # include Boost
        include_boost()
    endif()
endmacro()

macro(setup_sqlite)
    set(SQLITE3_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/sqlite-autoconf-3240000")
    include_directories(${SQLITE3_LIB_DIR})    # -I compilation command
    # https://www.sqlite.org/howtocompile.html
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(SQLITE3_LINK_FLAGS "-lpthread -ldl")
    endif()
    file(GLOB SQLITE3_SRC "${SQLITE3_LIB_DIR}/sqlite3.c")
endmacro()

# Configure project
project(CppSqlLiteDbAdapter)

# Setup Boost libraries
setup_boost()

# Setup SQLite
setup_sqlite()

# include
include_directories(${CMAKE_SOURCE_DIR}/include)    # -I compilation command

# Looking for files
file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE HDR "include/*.h")

add_executable(${PROJECT_NAME} "main.cpp" ${SRC} ${HDR} ${SQLITE3_SRC})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${SQLITE3_LINK_FLAGS})




