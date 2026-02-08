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
	add_custom_test(${TARGET_NAME})
	add_lldb(${TARGET_NAME})
endfunction()
function(solve TARGET_NAME)
	add_solution(${TARGET_NAME} solution.cpp)
endfunction()

add_subdirectory(permutation-rounds)
add_subdirectory(counting-divisors)
add_subdirectory(counting-divisors-fast)
add_subdirectory(div-game)
add_subdirectory(gcd-extreme)
add_subdirectory(prod-1-mod-n)
add_subdirectory(power-products)
add_subdirectory(diluc-and-kaeya)
add_subdirectory(test-with-cerr)
add_subdirectory(common-divisors)
