cmake_minimum_required(VERSION 3.30)
project(learning)

set(CMAKE_CXX_STANDARD 20)

include_directories("${CMAKE_SOURCE_DIR}/include/")
# Add ImGui source files
set(IMGUI_SOURCES
        ${CMAKE_SOURCE_DIR}/include/ImGUI/imgui.cpp
        ${CMAKE_SOURCE_DIR}/include/ImGUI/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/include/ImGUI/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/include/ImGUI/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/include/ImGUI/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/include/ImGUI/backends/imgui_impl_opengl3.cpp
        Client.cpp
        Client.h
)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(learning main.cpp ${IMGUI_SOURCES})

find_package(CURL REQUIRED)

link_directories("${CMAKE_SOURCE_DIR}/lib")

# Link GLFW, ImGui, and the required macOS frameworks
target_link_libraries(learning PRIVATE
        "${CMAKE_SOURCE_DIR}/lib/libglfw3.a"
        CURL::libcurl

        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
)
