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
elseif(NOT APPLE)
  message(FATAL_ERROR "Currently linux/mac only")
endif()

include_directories(${CORE_PATH})

include_directories(${THIRD_PARTY_DIR})
include_directories(${THIRD_PARTY_DIR}/imgui/include)
include_directories(${THIRD_PARTY_DIR}/stb_image)

if(APPLE)
  set(MAC_GL_PATH ${THIRD_PARTY_DIR}/mac)

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
file(GLOB DEMOS ${DEMOS_PATH}/*)

foreach(DEMO ${DEMOS})
    if (NOT IS_DIRECTORY ${DEMO})
        continue()
    endif()
  get_filename_component(DEMO_NAME ${DEMO} NAME_WE)
  message("Building: " ${DEMO_NAME})
  file(GLOB DEMO_SOURCES ${DEMO}/*.cpp)
  add_executable(${DEMO_NAME} ${CORE_SOURCES} ${PRIMITIVES_SOURCES}
                              ${DEMO_SOURCES} ${IMGUI_SOURCES})

  if(LINUX)
    target_link_libraries(${DEMO_NAME} glfw GLEW GL -ltbb)
  elseif(APPLE)
    target_link_libraries(${DEMO_NAME} glfw glew "-framework OpenGL")
  endif()
endforeach()
