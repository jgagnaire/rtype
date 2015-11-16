
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)

file(GLOB_RECURSE client_SOURCES "src/*.cpp")
file(GLOB_RECURSE client_HEADERS "inc/*.h")

add_executable(client_rtype ${client_SOURCES} ${client_HEADERS})

target_compile_features(client_rtype PRIVATE cxx_range_for)

set_target_properties(client_rtype PROPERTIES OUTPUT_NAME "client_rtype")

if (MSVC)
    set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
    set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
else()
    set(CMAKE_EXE_LINKER_FLAGS, "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
endif (MSVC)

target_link_libraries(client_rtype ${CMAKE_THREAD_LIBS_INIT})
