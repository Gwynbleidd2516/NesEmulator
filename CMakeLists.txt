cmake_minimum_required(VERSION 3.15)

set(APP_NAME NesEmu)

project(main)

MACRO(HEADER_DIRECTORIES return_list search_path)
    # Find all header files recursively
    file(GLOB_RECURSE new_list "${search_path}/*.h" "${search_path}/*.hpp")
    
    set(dir_list "")
    # Extract the directory component from each file path
    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        list(APPEND dir_list ${dir_path})
    endforeach()
    
    # Remove duplicate directory entries
    list(REMOVE_DUPLICATES dir_list)
    
    # Set the output variable with the list of unique directories
    set(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(MY_INCLUDES src)

file(GLOB TARGET_SRC "src/*.cpp" "src/*/*.cpp")
file(GLOB TARGET_HEADERS "src/*.h" "src/*/*.h" "src/*/*/*.h")
# file(GLOB TARGET_HEADERS_2 "src/*/*.h")
# file(GLOB TARGET_HEADERS_3 "src/*/*/*.h")

message(--TARGET_SRC-->${TARGET_SRC})
message(--TARGET_HEADERS-->${TARGET_HEADERS})
message(--INCLUDE-->${MY_INCLUDES})
# message(--TARGET_HEADERS_2-->${TARGET_HEADERS_2})
# message(--TARGET_HEADERS_3-->${TARGET_HEADERS_3})

add_executable(main ${TARGET_HEADERS} ${TARGET_HEADERS})

target_include_directories(main PUBLIC ${MY_INCLUDES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${APP_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .exe)