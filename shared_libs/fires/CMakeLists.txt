
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/)
set(fires_SOURCES ${fires_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/fires.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../common_src/JSONParser.cpp)
set(fires_HEADERS ${fires_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Entity/ComponentManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Entity/Entity.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Entity/ManagerException.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Component/Component.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Component/IComponent.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/JSONParser.hh)

add_library(fires SHARED ${fires_SOURCES} ${fires_HEADERS})

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	set_target_properties(fires PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	set_target_properties(fires PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR})
	set_target_properties(fires PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR})
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	set_target_properties(fires PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wall -Wextra")
endif()
