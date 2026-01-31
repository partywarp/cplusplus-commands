cmake_minimum_required(VERSION 4.2)
project(AllSolutions)

add_library(check_solution)
target_compile_features(check_solution PRIVATE cxx_std_20)
target_sources(check_solution
	PRIVATE
		check.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			check.hpp
)
add_executable(find_run_tests find_run_tests.cpp)
target_compile_features(find_run_tests PRIVATE cxx_std_20)
target_link_libraries(find_run_tests
	PRIVATE
		check_solution
)

add_compile_options(-g -fmax-errors=1 -pipe -std=c++17)
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

add_subdirectory(/Users/kali/cpp/counting-divisors)
add_subdirectory(/Users/kali/cpp/counting-divisors-fast)
add_subdirectory(/Users/kali/cpp/div-game)
add_subdirectory(/Users/kali/cpp/gcd-extreme)
add_subdirectory(/Users/kali/cpp/prod-1-mod-n)
add_subdirectory(/Users/kali/cpp/test-with-cerr)
add_subdirectory(/Users/kali/cpp/power-products)
add_subdirectory(/Users/kali/cpp/diluc-and-kaeya)
add_subdirectory(/Users/kali/cpp/permutation-rounds)
