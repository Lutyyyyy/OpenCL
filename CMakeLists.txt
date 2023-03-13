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

add_subdirectory(source)

message (STATUS "\n////////////////")
message (STATUS "Collected LIBS:")
foreach(LIB ${LIBS})
    message (STATUS "   ${LIB}")
endforeach()
message (STATUS "")

foreach(LIB ${LIBS})
    message (STATUS "lib: ${LIB}")

    target_compile_features(${LIB} PRIVATE cxx_std_20)

    target_link_options(${LIB} PRIVATE "${FLAGS}")
    target_compile_options(${LIB} PRIVATE "${FLAGS}")

    target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(${LIB} PUBLIC ${OPENCL_INCLUDE_DIR})

    get_property(dirs TARGET ${LIB} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    foreach(dir ${dirs})
        message(STATUS "    ${LIB} includes dir = '${dir}'")    
    endforeach()
    message (STATUS "")
endforeach()


foreach(LIB ${LIBS})
    foreach(LIB_TO_LINK ${LIBS})
        if (NOT ${LIB} STREQUAL ${LIB_TO_LINK})
            target_link_libraries(${LIB} PRIVATE ${LIB_TO_LINK})
            endif()
    endforeach()

    target_link_libraries(${LIB} ${OPENCL_LIBRARIES})

    get_target_property(OUT ${LIB} LINK_LIBRARIES)
    
    message(STATUS "TO LIB: ${LIB}")

    foreach(LIB_MESSAGE ${OUT})
        message(STATUS "    linked: ${LIB_MESSAGE}")
    endforeach()
    message(STATUS "")        
endforeach()


enable_testing()
find_package (GTest REQUIRED)


add_subdirectory(tests)

message (STATUS "\n////////////////")
message (STATUS "Collected TESTS:")
foreach(TEST ${TESTS})
message (STATUS "   ${TEST}")
endforeach()
message (STATUS "")


foreach (TEST ${TESTS})
    message (STATUS "tests: ${TEST}")

    target_compile_features(${TEST} PRIVATE cxx_std_20)

    target_link_options(${TEST} PRIVATE "${FLAGS}")
    target_compile_options(${TEST} PRIVATE "${FLAGS}")

    target_include_directories(${TEST} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries (${TEST} PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
    target_link_libraries (${TEST} PRIVATE ${LIBS})
    
    target_include_directories(${TEST} PUBLIC ${OPENCL_INCLUDE_DIR})
    target_link_libraries(${TEST} PRIVATE ${OPENCL_LIBRARIES})

    get_target_property(OUT ${TEST} LINK_LIBRARIES)
    get_target_property(OUT_OPTIONS ${TEST} LINK_OPTIONS)
    
    foreach(OUT_OPTION ${OUT_OPTIONS})
    message (STATUS "     linked option: ${OUT_OPTION}")
    endforeach()
    
    foreach(LIB ${OUT})
    message(STATUS "    linked: ${LIB}")
    endforeach()
    
    message(STATUS "")
    
endforeach ()

add_subdirectory(apps)
message (STATUS "\n////////////////")
message (STATUS "Collected APPS: ${APPS}")
    
foreach(APP ${APPS})
    message (STATUS "app: ${APP}")

    target_compile_features(${APP} PRIVATE cxx_std_20)

    target_link_options(${APP} PRIVATE "${FLAGS}")
    target_compile_options(${APP} PRIVATE "${FLAGS}")

    target_include_directories(${APP} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${APP} PRIVATE ${LIBS})
    
    target_include_directories(${APP} PUBLIC ${OPENCL_INCLUDE_DIR})
    target_link_libraries(${APP} PRIVATE ${OPENCL_LIBRARIES})
    
    get_property(dirs TARGET ${APP} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    foreach(dir ${dirs})
        message(STATUS "    ${APP} includes dir = '${dir}'")    
    endforeach()

    get_target_property(OUT ${APP} LINK_LIBRARIES)
    get_target_property(OUT_OPTIONS ${APP} LINK_OPTIONS)
    
    foreach(OUT_OPTION ${OUT_OPTIONS})
    message (STATUS "     linked option: ${OUT_OPTION}")
    endforeach()
    
    foreach(LIB ${OUT})
        message(STATUS "    linked: ${LIB}")
    endforeach()

    message (STATUS "")
endforeach()
 
#///////////////////////////////////////////////////////////////////////////////////////
