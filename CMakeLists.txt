cmake_minimum_required(VERSION 3.0.0)
project(RayTracer VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(RayTracer main.cpp src/hittables.cpp src/vector3.cpp src/camera.cpp src/materials.cpp src/light.cpp)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( RayTracer ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
