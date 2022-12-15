cmake_minimum_required(VERSION 3.0.0)
project(tp2-cg VERSION 0.1.0)

add_executable(tp2-cg main.cpp Camera.cpp Camera.h stb_image.h stb_image.cpp)
target_link_libraries(tp2-cg -lglut -lGL -lGLU)

include(CTest)
enable_testing()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
