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


set (
FLAGS
-Wall
-Wextra
-Wpedantic
-Werror
-O2
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/kernels DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_compile_options(${FLAGS})

#libs
add_library(bitonic_sort source/bitonic_sort.cpp)
target_include_directories(bitonic_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIR})
target_link_libraries(bitonic_sort PRIVATE ${OPENCL_LIBRARIES})
target_compile_features(bitonic_sort PRIVATE cxx_std_20)

#apps
add_executable (bitonic_run apps/bitonic_run.cpp)
target_include_directories(bitonic_run PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIR})
target_link_libraries(bitonic_run PRIVATE ${OPENCL_LIBRARIES})
target_link_libraries(bitonic_run PRIVATE bitonic_sort)
target_compile_features(bitonic_run PRIVATE cxx_std_20)

#tests
enable_testing()
find_package (GTest REQUIRED)

add_executable(bitonic_test tests/bitonic_test.cpp)
target_include_directories(bitonic_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIR})
target_link_libraries (bitonic_test PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
target_link_libraries(bitonic_test PRIVATE ${OPENCL_LIBRARIES})
target_link_libraries(bitonic_test PRIVATE bitonic_sort)
target_compile_features(bitonic_test PRIVATE cxx_std_20)
