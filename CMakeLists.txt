cmake_minimum_required(VERSION 3.20)
project(Grimm)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS json)

add_executable(Grimm main.cpp GrimmMap.cpp)

target_include_directories(Grimm PUBLIC ${Boost_INCLUDE_DIRS})
target_link_directories(Grimm PUBLIC ${Boost_LIBRARY_DIRS})
target_link_libraries(Grimm PUBLIC ${Boost_LIBRARIES})
