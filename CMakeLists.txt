cmake_minimum_required(VERSION 3.20)

project(libdan
	VERSION 0.1.0
	LANGUAGES CXX
)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(CTest)

add_library(dan
	src/dimension.cpp
	src/unit.cpp
	src/libdan.cpp
)

set_target_properties(dan PROPERTIES
	VERSION   ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

add_library(libdan::dan ALIAS dan)

target_compile_features(dan PUBLIC cxx_std_20)

target_include_directories(dan
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (MSVC)
	target_compile_options(dan PRIVATE /W4)
else ()
	target_compile_options(dan PRIVATE -Wall -Wextra -Wpedantic)
endif ()

if(BUILD_TESTING)
	include(FetchContent)

	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v3.16.1
	)

	FetchContent_MakeAvailable(Catch2)

	add_executable(libdan_tests
		tests/test_dimension.cpp
		tests/test_unit.cpp
	)

	target_include_directories(libdan_tests PRIVATE
		src
	)

	target_link_libraries(libdan_tests
		PRIVATE
			libdan::dan
			Catch2::Catch2WithMain
	)

	list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
	include(Catch)
	catch_discover_tests(libdan_tests)
endif()
