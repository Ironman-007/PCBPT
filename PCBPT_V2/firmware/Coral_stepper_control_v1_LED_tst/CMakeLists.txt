cmake_minimum_required(VERSION 3.20)

project("Coral_stepper_control_v1_LED_tst" C CXX ASM)

include(cmake/st-project.cmake)

add_executable(${PROJECT_NAME})
add_st_target_properties(${PROJECT_NAME})