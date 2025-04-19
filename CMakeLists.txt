cmake_minimum_required(VERSION 3.30)
project(banking_project_ C)

set(CMAKE_C_STANDARD 23)

add_executable(banking_project_ bank_project.c
        acoount.c
        bank.c
        transaction.c
        function.c
        acoount.h
        bank.h)
