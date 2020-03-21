cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(rollercoaster)

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(rollercoaster
  ${CMAKE_SOURCE_DIR}/rollercoaster/camera.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/cart.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/catmullrom.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/facevertexmesh.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/freetypefont.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/game.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/hud.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/lamp.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/lighting.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/main.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/matrixstack.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/objmodel.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/point.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/pyramid.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/rollercoaster.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/shader.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/shaderprogram.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/skybox.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/sphere.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/support.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/terrain.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/texture.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/timer.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/tree.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/vbo.cpp
  ${CMAKE_SOURCE_DIR}/rollercoaster/window.cpp
)

add_definitions(
  -DUINT=unsigned
  -DGLFW_INCLUDE_GLCOREARB)

target_include_directories(rollercoaster PRIVATE
  ${CMAKE_SOURCE_DIR}/rollercoaster/include/freetype
  OpenGL::GL
  /usr/local/Cellar/glew/2.1.0/include
  /usr/local/Cellar/glfw/3.3/include
)

target_link_libraries(rollercoaster PRIVATE
  OpenGL::GL
  Freetype::Freetype
  /usr/local/Cellar/freeimage/3.18.0/lib/libfreeimage.3.18.0.dylib
  /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.0.dylib
  /usr/local/Cellar/glfw/3.3/lib/libglfw.3.3.dylib
)
