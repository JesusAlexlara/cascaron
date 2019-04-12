#Versión minima requerida de cmake
cmake_minimum_required(VERSION 3.1)

#Nombre del proyecto.
project(cascaron)


#Librerias externas
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

FIND_PACKAGE(OpenCL REQUIRED)
INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS})

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(headers)

file(GLOB PROJECT_HEADERS headers/*.hpp)
file(GLOB PROJECT_SOURCES sources/*.cpp)
file(GLOB PROJECT_KERNELCL kernels/*.cl)
file(GLOB PROJECT_CONFIGS CMakeLists.txt)
file(GLOB PROJECT_FILTROS filters/*)

source_group("headers" FILES ${PROJECT_HEADERS})
source_group("sources" FILES ${PROJECT_SOURCES})
source_group("kernels" FILES ${PROJECT_KERNELCL})
source_group("filtros" FILES ${PROJECT_FILTERS})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}
    ${PROJECT_HEADERS} ${PROJECT_KERNELCL} ${PROJECT_CONFIGS} ${PROJECT_FILTERS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS} ${OpenCL_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

if(CMAKE_THREAD_LIBS_INIT)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()
