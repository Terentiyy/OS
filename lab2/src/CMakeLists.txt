cmake_minimum_required(VERSION 3.10)
project(multithreaded_summing)

# Найти библиотеку pthread
find_package(Threads REQUIRED)

add_executable(multithread_summing main.c)
target_link_libraries(multithread_summing Threads::Threads)