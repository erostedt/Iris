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
  set(GLPATH ${THIRD_PARTY_DIR}/linux)
elseif(APPLE)
  set(GLPATH ${THIRD_PARTY_DIR}/mac)
else()
  message(FATAL_ERROR "Currently linux/mac only")
endif()

include_directories(${CORE_PATH})
include_directories(${PRIMITIVES_PATH})

include_directories(${THIRD_PARTY_DIR})
include_directories(${THIRD_PARTY_DIR}/imgui/include)

include_directories(${GLPATH}/glfw/include)
include_directories(${GLPATH}/glfw/include/GLFW)
include_directories(${GLPATH}/glew/include)
include_directories(${GLPATH}/glew/include/GL)
include_directories(${GLPATH}/glm/include)

link_directories(${GLPATH}/glfw/lib)
link_directories(${GLPATH}/glew/lib)

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

if (LINUX)
    target_link_libraries(${DEMO_NAME} glfw GLEW GL -ltbb)
elseif(APPLE)
    target_link_libraries(${DEMO_NAME} glfw glew "-framework OpenGL")
endif()
endforeach()
