# lib/CMakeLists.txt
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
file(GLOB sources CMAKE_CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/*.c")
file(GLOB headers CMAKE_CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")
add_library(emu STATIC ${sources} ${headers})
target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/include)

# SDL2 library
target_include_directories(emu PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(emu ${SDL2_LIBRARIES})
target_link_libraries(emu ${SDL2_TTF_LIBRARY})

