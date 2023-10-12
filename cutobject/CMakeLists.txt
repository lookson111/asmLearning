cmake_minimum_required(VERSION 3.11)

project(cutobject CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)

conan_basic_setup(TARGETS)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CATCH2LIB CONAN_PKG::catch2)

add_executable(cutobject
	src/main.cpp
	src/camera.cpp
	src/camera.h
)

target_link_libraries(cutobject PRIVATE 
	CONAN_PKG::sfml
	CONAN_PKG::glu
)

add_executable(cutobject_tests
    tests/cutobject_tests.cpp
)

target_link_libraries(cutobject_tests PRIVATE 
	${CATCH2LIB} 
)
