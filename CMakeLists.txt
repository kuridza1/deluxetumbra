cmake_minimum_required(VERSION 3.20)

project(DeLuxEtUmbra)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(PACKAGES_DIR ${CMAKE_SOURCE_DIR}/packages)

# -----------------------------
# Source files
# -----------------------------

file(GLOB_RECURSE PROJECT_SOURCES
    ${SRC_DIR}/*.cpp
)

# -----------------------------
# GLAD
# -----------------------------

set(GLAD_SRC
    ${EXTERNAL_DIR}/glad/src/glad.c
)

# -----------------------------
# ImGui
# -----------------------------

set(IMGUI_SOURCES
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

# -----------------------------
# Executable
# -----------------------------

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${GLAD_SRC}
    ${IMGUI_SOURCES}
)

# -----------------------------
# Include directories
# -----------------------------

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${INCLUDE_DIR}

    ${EXTERNAL_DIR}/glad/include

    ${EXTERNAL_DIR}/imgui
    ${EXTERNAL_DIR}/imgui/backends
    
)


# -----------------------------
# Libraries
# -----------------------------

target_link_libraries(${PROJECT_NAME}
PRIVATE
    opengl32.lib
)


# -----------------------------
# Copy assets after build
# -----------------------------

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)