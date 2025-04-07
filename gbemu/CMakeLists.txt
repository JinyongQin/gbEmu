set(MAIN_SOURCE main.c)
set(CMAKE_BUILD_TYPE Debug)
file(GLOB headers "${PROJECT_SOURCE_DIR}/include/*.h")

add_executable(gbemu ${MAIN_SOURCE} ${headers})

target_link_libraries(gbemu emu)
target_include_directories(gbemu PUBLIC ${PROJECT_SOURCE_DIR}/include)

# info for SDL2
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
#info for SDL2_ttf
message(STATUS "SDL2_TTF_INCLUDE_DIRS: ${SDL2_TTF_INCLUDE_DIRS}")
message(STATUS "SDL2_TTF_LIBRARIES: ${SDL2_TTF_LIBRARIES}")

install(TARGETS gbemu DESTINATION bin)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_SOURCE_DIR}/NotoSansMono-Medium.ttf
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM)