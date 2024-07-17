cmake_minimum_required(VERSION 3.25)

project(iris)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall")
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(DEMOS_PATH ${PROJECT_SOURCE_DIR}/demos)
set(CORE_PATH ${SRC_DIR}/core)
set(PRIMITIVES_PATH ${SRC_DIR}/primitives)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
else()
  message(FATAL_ERROR "Currently linux only")
endif()

include_directories(${CORE_PATH})
include_directories(${PRIMITIVES_PATH})

include_directories(${THIRD_PARTY_DIR})
include_directories(${THIRD_PARTY_DIR}/imgui/include)

include_directories(${THIRD_PARTY_DIR}/glfw/include)
include_directories(${THIRD_PARTY_DIR}/glfw/include/GLFW)
include_directories(${THIRD_PARTY_DIR}/GL)
include_directories(${THIRD_PARTY_DIR}/glm/include)

if(LINUX)
  link_directories(${THIRD_PARTY_DIR}/glfw/linux/lib)
  link_directories(${THIRD_PARTY_DIR}/glew/linux/lib)
endif()

file(GLOB IMGUI_SOURCES ${THIRD_PARTY_DIR}/imgui/src/*.cpp)
file(GLOB CORE_SOURCES ${CORE_PATH}/*.cpp)
file(GLOB PRIMITIVES_SOURCES ${PRIMITIVES_PATH}/*.cpp)
file(GLOB DEMOS ${DEMOS_PATH}/*)

foreach(DEMO ${DEMOS})
  get_filename_component(DEMO_NAME ${DEMO} NAME_WE)
  message("Building: " ${DEMO_NAME})
  file(GLOB DEMO_SOURCES ${DEMO}/*.cpp)
  add_executable(${DEMO_NAME} ${CORE_SOURCES} ${PRIMITIVES_SOURCES}
                              ${DEMO_SOURCES} ${IMGUI_SOURCES})

  target_link_libraries(${DEMO_NAME} glfw GLEW GL -ltbb)
endforeach()
