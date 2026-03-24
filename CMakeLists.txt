cmake_minimum_required(VERSION 3.10)
project(train_ticket_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable exceptions and RTTI for better performance
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Werror -std=c++17")

# Create executable named 'code' as required
add_executable(code main.cpp)

# Add include directories
target_include_directories(code PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})