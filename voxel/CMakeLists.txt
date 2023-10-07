cmake_minimum_required(VERSION 3.11)

project(cut_object CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)

conan_basic_setup(TARGETS)


#find_package(SFML EXACT REQUIRED COMPONENTS system window graphics audio)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CATCH2LIB CONAN_PKG::catch2)

add_executable(cut_object
	src/main.cpp
	src/window/Events.h
	src/window/Events.cpp
	src/window/Window.h
	src/window/Window.cpp
)

target_link_libraries(cut_object PRIVATE 
	CONAN_PKG::glfw
	CONAN_PKG::glew
)

add_executable(cut_object_tests
    tests/cut_object_tests.cpp
)

target_link_libraries(cut_object_tests PRIVATE 
	${CATCH2LIB} 
)
