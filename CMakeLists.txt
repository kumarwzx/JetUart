cmake_minimum_required(VERSION 3.8)
project(serialTR)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem program_options system)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  $${Boost_INCLUDE_DIRS}
  )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

ADD_LIBRARY(MySerial SHARED
  src/SerialPort.cpp
  src/SerialStream.cpp
  src/SerialStreamBuf.cpp
  )
target_link_libraries(MySerial ${Boost_LIBRARIES} Threads::Threads)

add_executable(set_pack test/setpack.cpp)
target_link_libraries(set_pack
  MySerial
  Threads::Threads
  )



