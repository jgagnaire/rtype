
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(CMAKE_BUILD_TYPE Debug)

set(client_SOURCES ${client_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Window.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Network/NetworkManager.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Network/TcpSocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Network/UdpSocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Audio/AudioCallSystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Audio/Recorder.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Event/EventAggregator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Render/AnimatedSprite.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Render/RenderSystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Render/ReadyScene.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Shoot/Pattern.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/System/Render/ScrollingSprite.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/Clock.cpp)

set(client_HEADERS ${client_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/Component/Component.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Component/IComponent.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Entity/ComponentManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Entity/Entity.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Entity/ManagerException.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/SystemManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network/IPacket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network/ITcpSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network/IUdpSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network/NetworkManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network/TcpSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network/UdpSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/ASystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Audio/AudioCallSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Mob/MobSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Audio/Recorder.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Event/EventAggregator.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/ADrawable.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/AnimatedSprite.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/AView.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/GameRoomScene.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/IWindow.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/LoginScene.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/MenuScene.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/RenderSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/Scene.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/ScrollingSprite.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/StageScene.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/Text.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/View.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render/Window.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/Clock.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/IClock.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/RTime.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/IRTime.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/ISoundBuffer.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/SoundBuffer.hh  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/ISound.hh  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility/Sound.hh)

add_executable(client_rtype ${client_SOURCES} ${client_HEADERS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/inc/Component ${CMAKE_CURRENT_SOURCE_DIR}/inc/Entity/ ${CMAKE_CURRENT_SOURCE_DIR}/inc/Network ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/ ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Audio/ ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Event ${CMAKE_CURRENT_SOURCE_DIR}/inc/System/Render ${CMAKE_CURRENT_SOURCE_DIR}/inc/Utility)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window network graphics audio)

target_compile_features(client_rtype PRIVATE cxx_range_for)

set_target_properties(client_rtype PROPERTIES OUTPUT_NAME "client_rtype")

target_link_libraries(client_rtype ${CMAKE_THREAD_LIBS_INIT} ${SFML_LIBRARIES})

if (MSVC)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/)
  set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
  set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
  find_package(SFML 2 REQUIRED system window network graphics audio main)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra")
  set(CMAKE_EXE_LINKER_FLAGS, "${CMAKE_EXE_LINKER_FLAGS} -pthread -Wall -Wextra")
  find_package(SFML 2 REQUIRED system window network graphics audio)
endif (MSVC)
