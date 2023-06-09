cmake_minimum_required(VERSION 3.16)
project(Interpol CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
endif()

include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# download and configure the glad project
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)
FetchContent_MakeAvailable(glad)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(
        GLAD_PROFILE "core"
        CACHE STRING "OpenGL profile"
    )
    set(
        GLAD_API "gl=4.6"
        CACHE STRING "API type/version pairs, "
                        "like \"gl=3.2,gles=\", no version means latest"
    )
    set(
        GLAD_GENERATOR "c"
        CACHE STRING "Language to generate the binding for")
    add_subdirectory(
        ${glad_SOURCE_DIR}
        ${glad_BINARY_DIR}
    )
endif()

# download and configure the GLFW project
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_DOCS     off CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    off CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES off CACHE BOOL "" FORCE)

    add_subdirectory(
        ${glfw_SOURCE_DIR}
        ${glfw_BINARY_DIR}
    )
endif()

# download the Dear ImGUI project
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
)
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    add_subdirectory(
        ${imgui_SOURCE_DIR}
    )
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
find_package(OpenGL REQUIRED)

# Dear ImGui library configuration
add_library(
    imgui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

include_directories(
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glad_BINARY_DIR}/include # so imgui can find glad
)

target_include_directories(
    imgui PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glad_BINARY_DIR}/include # so imgui can find glad
)

target_link_libraries(
    imgui PRIVATE
    glad
    glfw
    ${OPENGL_LIBRARY}
)

file(GLOB_RECURSE SOURCES_MATH src/math/*.cpp)
file(GLOB_RECURSE SOURCES_CONSOLE src/user_interaction/*.cpp)
file(GLOB_RECURSE SOURCES_GUI src/drawing/*.cpp)
file(GLOB_RECURSE SOURCES_UTILS src/utils/*.cpp)

file(GLOB_RECURSE TESTS tests/*.cpp)

# unit_test configuration
add_executable(
        unit_tests
        ${TESTS}
        ${SOURCES_MATH}
        ${SOURCES_CONSOLE}
        ${SOURCES_GUI}
        ${SOURCES_UTILS}
)

target_include_directories(unit_tests PUBLIC googletest)
target_include_directories(unit_tests PUBLIC include)
target_include_directories(unit_tests PUBLIC imgui)

target_link_libraries(
        unit_tests
        imgui
        glfw
        glad
        gtest
        gtest_main
)

# triangle_intersection configuration
add_executable(
        Interpol
        src/main.cpp

        ${SOURCES_MATH}
        ${SOURCES_CONSOLE}
        ${SOURCES_GUI}
        ${SOURCES_UTILS}
)

target_include_directories(Interpol PUBLIC include)
target_include_directories(Interpol PUBLIC imgui)
target_include_directories(Interpol PUBLIC glad)
target_include_directories(Interpol PUBLIC glfw)

target_link_libraries(
    Interpol
    imgui
    glfw
    glad
)