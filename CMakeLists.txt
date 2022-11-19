cmake_minimum_required(VERSION 3.20)
project(Server_2 C)

set(CMAKE_C_STANDARD 99)
add_link_options(-pthread)
add_link_options(-lrt)

add_executable(Server_2 main.c create_array.h create_array.c buffer.h buffer.c beast.c beast.h)
