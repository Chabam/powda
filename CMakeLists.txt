cmake_minimum_required(VERSION 3.22)

project(powda)

add_executable(powda)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
  FIND_PACKAGE_ARGS NAMES glfw3
)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        glad2
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)

FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        5.0
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
  FIND_PACKAGE_ARGS NAMES Eigen3
)

FetchContent_MakeAvailable(glfw glad eigen)
add_subdirectory(${glad_SOURCE_DIR}/cmake ${glad_BINARY_DIR})
glad_add_library(glad STATIC REPRODUCIBLE LOADER LANGUAGE C API gl:core=4.6)

target_link_libraries(powda PRIVATE glfw glad tbb)
set_target_properties(powda PROPERTIES CXX_STANDARD 20)

target_include_directories(powda PRIVATE include/)

add_subdirectory(src)
