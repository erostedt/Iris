cmake_minimum_required(VERSION 3.25)

project(Iris)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall")
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(EXAMPLES_PATH ${PROJECT_SOURCE_DIR}/examples)
set(CORE_PATH ${SRC_DIR}/core)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
elseif(NOT APPLE)
  message(FATAL_ERROR "Currently linux/mac only")
endif()

find_package(OpenCV REQUIRED)

include_directories(${CORE_PATH})

include_directories(${THIRD_PARTY_DIR})
include_directories(${THIRD_PARTY_DIR}/imgui/include)
include_directories(${THIRD_PARTY_DIR}/stb_image)
include_directories(${OpenCV_INCLUDE_DIRS})


if(APPLE)
  set(MAC_GL_PATH ${THIRD_PARTY_DIR}/mac)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library")

  include_directories(${MAC_GL_PATH}/glfw/include)
  include_directories(${MAC_GL_PATH}/glfw/include/GLFW)
  include_directories(${MAC_GL_PATH}/glew/include)
  include_directories(${MAC_GL_PATH}/glew/include/GL)
  include_directories(${MAC_GL_PATH}/glm/include)

  link_directories(${MAC_GL_PATH}/glfw/lib)
  link_directories(${MAC_GL_PATH}/glew/lib)
endif()

file(GLOB IMGUI_SOURCES ${THIRD_PARTY_DIR}/imgui/src/*.cpp)
file(GLOB CORE_SOURCES ${CORE_PATH}/*.cpp)
file(GLOB EXAMPLES ${EXAMPLES_PATH}/*)

foreach(EXAMPLE ${EXAMPLES})
    if (NOT IS_DIRECTORY ${EXAMPLE})
        continue()
    endif()
  get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
  message("Building: " ${EXAMPLE_NAME})
  file(GLOB EXAMPLE_SOURCES ${EXAMPLE}/*.cpp)
  add_executable(${EXAMPLE_NAME} ${CORE_SOURCES} ${EXAMPLE_SOURCES} ${IMGUI_SOURCES})
  if(LINUX)
    target_link_libraries(${EXAMPLE_NAME} glfw GLEW GL ${OpenCV_LIBS} -ltbb)
  elseif(APPLE)
    target_link_libraries(${EXAMPLE_NAME} glfw glew "-framework OpenGL" ${OpenCV_LIBS})
  endif()
endforeach()

