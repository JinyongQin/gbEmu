# 寻找 SDL2_ttf 库
# 1. 查找头文件
find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h)
# 2. 查找库文件
find_library(SDL2_TTF_LIBRARY SDL2_ttf)
if(SDL2_TTF_INCLUDE_DIR AND SDL2_TTF_LIBRARY)
    # 3. 设置变量
    set(SDL2_TTF_FOUND TRUE)
else()
    set(SDL2_TTF_FOUND FALSE)
endif()

mark_as_advanced(SDL2_TTF_INCLUDE_DIR SDL2_TTF_LIBRARY) # Hide these variables from the user