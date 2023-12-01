cmake_minimum_required(VERSION 3.20)
project(simulation)

#Switch solutions by commenting undesired lines
set(SIMULATION_100 true)
#set(SIMULATION_151 true)

add_library(${PROJECT_NAME}.Library INTERFACE)
target_include_directories(${PROJECT_NAME}.Library INTERFACE ECS/include)
target_include_directories(${PROJECT_NAME}.Library INTERFACE ECS/src)

if (DEFINED SIMULATION_100)
    target_include_directories(${PROJECT_NAME}.Library INTERFACE "100")
endif ()
if (DEFINED SIMULATION_151)
    target_include_directories(${PROJECT_NAME}.Library INTERFACE "151")
endif ()

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}.Library)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
