cmake_minimum_required(VERSION 3.19)
project(td03 C)

set(CMAKE_C_STANDARD 99)

add_executable(multiple_fork.exe multiple_fork.c)
add_executable(fcat.exe fcat.c)
add_executable(zombie.exe zombie.c)
add_executable(orphelin.exe orphelin.c)
add_executable(exec_prop.exe exec_prop.c)
add_executable(exec_prop-aux.exe exec_prop-aux.c)
add_executable(shell_exec.exe shell_exec.c)
add_executable(shell_system.exe shell_system.c)