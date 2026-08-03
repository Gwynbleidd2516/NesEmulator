cmake_minimum_required(VERSION 3.15)

set(APP_NAME NesEmu)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0 # Укажите нужную вам версию или тег
)

FetchContent_MakeAvailable(spdlog)

set(ONLY_LIBS ON CACHE BOOL "Build only GLEW libraries, skip executables" FORCE)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "Disable shared library if you only want static" FORCE)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG        master
    GIT_SHALLOW    ON
)

# 4. Make the targets available
FetchContent_MakeAvailable(glew)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# 3. Declare the GLFW repository and version
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4  # Or use a specific commit hash for stability
    GIT_SHALLOW    ON   # Speeds up download by ignoring git history
)

# 4. Download and populate the target
FetchContent_MakeAvailable(glfw)

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

message(--TARGET_SRC--> ${TARGET_SRC})
message(--TARGET_HEADERS--> ${TARGET_HEADERS})
message(--INCLUDE--> ${MY_INCLUDES})

add_executable(main ${TARGET_SRC} ${TARGET_HEADERS})

# add_compile_definitions(DO_LOGS)

target_include_directories(main PUBLIC ${MY_INCLUDES})

target_link_libraries(main PRIVATE spdlog::spdlog)

find_package(OpenGL REQUIRED)
target_link_libraries(main PRIVATE libglew_static OpenGL::GL)
target_link_libraries(main PRIVATE glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${APP_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .exe)