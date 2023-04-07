cmake_minimum_required (VERSION 3.14)

project(bitonic_sort VERSION 1.0)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file).
Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

find_package(OpenCLICDLoader)
if (NOT OpenCLICDLoader_FOUND)
    message("If cmake couldn't find OpenCL loader, please set the path to OpenCLICDLoaderConfig.cmake in CMAKE_PREFIX_PATH")
endif()

find_package(OpenCL REQUIRED)
find_package(Threads REQUIRED)

set(OPENCL_LIBRARIES OpenCL::OpenCL Threads::Threads)

find_package(OpenCLHeaders)
if (NOT OpenCLHeaders_FOUND)
    message("If cmake couldn't find OpenCL header files, please set the path to OpenCLHeadersConfig.cmake in CMAKE_PREFIX_PATH")
else()
    set(OPENCL_LIBRARIES ${OPENCL_LIBRARIES} OpenCL::Headers)
endif()

find_package(OpenCLHeadersCpp)
if (NOT OpenCLHeadersCpp_FOUND)
    message("If cmake couldn't find OpenCL C++ header files, please set the path to OpenCLHeadersCppConfig.cmake in CMAKE_PREFIX_PATH")
else()
    set(OPENCL_LIBRARIES ${OPENCL_LIBRARIES} OpenCL::HeadersCpp)
endif()

if(NOT BUILD_TYPE)
	set(BUILD_TYPE RELEASE)
endif()

if ("${BUILD_TYPE}" STREQUAL "DEBUG")
    set (OPTIONS -Wall -Wextra -Wpedantic -Werror -DDEBUG_MODE=1)
else ()
    set (OPTIONS -Wall -Wextra -Wpedantic -Werror -O3)
endif ()
add_compile_options(${OPTIONS})

#libs
add_subdirectory(Bsort)

#apps
find_package(Boost COMPONENTS program_options REQUIRED)

add_executable (Bsort_run apps/Bsort_run.cpp)
target_include_directories (Bsort_run PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries (Bsort_run PRIVATE ${OPENCL_LIBRARIES} ${Boost_LIBRARIES} tbb)
target_link_libraries (Bsort_run PRIVATE Bsort)
target_compile_features (Bsort_run PRIVATE cxx_std_20)

#tests
enable_testing()
find_package (GTest REQUIRED)

add_executable (Bsort_test tests/Bsort_test.cpp)
target_include_directories (Bsort_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIR} ${Boost_INCLUDE_DIR}/program_options)
target_link_libraries (Bsort_test PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
target_link_libraries (Bsort_test PRIVATE ${OPENCL_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries (Bsort_test PRIVATE Bsort)
target_compile_features (Bsort_test PRIVATE cxx_std_20)
