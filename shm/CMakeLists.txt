cmake_minimum_required(VERSION 3.10)
project(shm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TO_COMPILE
    Cargo.cpp 
    Coordinates.cpp 
    Fruit.cpp 
    Map.cpp 
    Player.cpp 
    Ship.cpp 
    Store.cpp
)

#add_executable(${PROJECT_NAME} ${TO_COMPILE})
add_library(${PROJECT_NAME}-lib STATIC ${TO_COMPILE})

add_executable(${PROJECT_NAME} Main.cpp)
add_executable(${PROJECT_NAME}-ut Test.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)
target_link_libraries(${PROJECT_NAME}-ut ${PROJECT_NAME}-lib)

set(FLAGS 
    -Wall 
    -Wextra 
    -Werror 
    -pedantic 
    -Wconversion
)

target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})
target_compile_options(${PROJECT_NAME}-lib PRIVATE ${FLAGS})

enable_testing()
add_test(Name shmTest COMMAND ${PROJECT_NAME}-ut)
