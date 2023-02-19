cmake_minimum_required(VERSION 3.10)

project(c-opengl VERSION 1.0)
add_executable(main 
    source/main.c
    source/shader.c
    source/fileops.c
    thirdparty/glad/src/glad.c
    )

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(thirdparty/glad/include)
include_directories(thirdparty/stb)
include_directories(C:/devel/cglm/include)
include_directories(C:/devel/glfw/include)

target_link_libraries(main C:/devel/glfw/build/src/Debug/glfw3.lib)
target_link_libraries(main C:/devel/cglm/win/x64/Debug/cglm.lib)
