# test/CMakeLists.txt
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_BUILD_TYPE Debug)
# find check
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})
set(TEST_SOURCE
    test_gbe.c
)

add_executable(test_gbemu ${TEST_SOURCE})
target_link_libraries(test_gbemu emu ${CHECK_LIBRARIES})
target_include_directories(test_gbemu PRIVATE ${PROJECT_SOURCE_DIR}/include )

target_link_libraries(test_gbemu ${CHECK_LIBRARIES} pthread m)