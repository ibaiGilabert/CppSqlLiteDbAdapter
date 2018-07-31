include_directories("${gtest_SOURCE_DIR}/include")
include_directories("${gmock_SOURCE_DIR}/include")

file(GLOB_RECURSE SRC "*.cpp")
file(GLOB_RECURSE HDR "*.h")

add_executable(DbSQLiteAdapterTest ${SRC} ${HDR})
target_link_libraries(DbSQLiteAdapterTest ${Boost_LIBRARIES} cpp-sqlite-db-adapter sqlite3 ${SQLITE3_LINK_FLAGS})

# google_mock
target_link_libraries(DbSQLiteAdapterTest gmock_main)


