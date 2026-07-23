cmake_minimum_required(VERSION 4.2)
project(AllSolutions)
set(CMAKE_CXX_STANDARD 17)

add_library(check_solution)
target_sources(check_solution
	PRIVATE check.cpp
	PUBLIC FILE_SET HEADERS FILES check.hpp
)
add_executable(find_run_tests find_run_tests.cpp)
target_link_libraries(find_run_tests PRIVATE check_solution)
add_library(freopen_shim SHARED no_freopen.c)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-fmax-errors=1 -Wall -pipe -g)
link_libraries(m)
function(add_lldb TARGET_NAME)
add_custom_target("${TARGET_NAME}.lldb"
	COMMAND tmux splitw -h lldb $<TARGET_FILE:${TARGET_NAME}>
	DEPENDS ${TARGET_NAME}
	WORKING_DIRECTORY $<TARGET_PROPERTY:${TARGET_NAME},SOURCE_DIR>
)
endfunction()
function(add_custom_test TARGET_NAME)
add_custom_target("${TARGET_NAME}.test"
	COMMAND $<TARGET_FILE:find_run_tests>
		$<TARGET_FILE:${TARGET_NAME}>
	DEPENDS ${TARGET_NAME} find_run_tests
	WORKING_DIRECTORY $<TARGET_PROPERTY:${TARGET_NAME},SOURCE_DIR>
	USES_TERMINAL
)
endfunction()
function(add_solution TARGET_NAME)
add_executable(${TARGET_NAME} ${ARGN})
target_link_libraries(${TARGET_NAME} PRIVATE freopen_shim)
add_custom_test(${TARGET_NAME})
add_lldb(${TARGET_NAME})
endfunction()
function(solve TARGET_NAME)
add_solution(${TARGET_NAME} solution.cpp)
endfunction()

file(GLOB_RECURSE CMAKE_LISTS RELATIVE ${PROJECT_SOURCE_DIR} CONFIGURE_DEPENDS "CMakeLists.txt")
list(REMOVE_ITEM CMAKE_LISTS "CMakeLists.txt")
foreach(cmake_list IN LISTS CMAKE_LISTS)
	cmake_path(GET cmake_list PARENT_PATH subdir)
	add_subdirectory("${subdir}")
endforeach()

