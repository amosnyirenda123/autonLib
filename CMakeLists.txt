cmake_minimum_required(VERSION 3.30)

project(AutonLib C)

add_executable(AutonLib
    main.c
    src/automaton.c
    src/auton_memory.c
)