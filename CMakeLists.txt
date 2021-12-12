cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# add_definitions(-std=c++20)
project(simpleArkanoid)

if (MSVC)
    # warning level 4 
    add_compile_options(/W4)
else()
    # lots of warnings 
    add_compile_options(-Wall)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED) 
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} headers)

add_executable(simpleArkanoid 
src/main.cpp
src/Controller.cpp
src/Game.cpp
src/Renderer.cpp
src/LimitTimer.cpp
src/SDLexception.cpp
src/IntervalTimer.cpp
src/Ball.cpp
# src/gMath.cpp //NOTE: remove if not used
src/Texture.cpp
src/Paddle.cpp
src/StaticObject.cpp
src/SideWall.cpp
src/RandNum.cpp
src/Vector2d.cpp
src/MovableObject.cpp
src/Block.cpp
src/LevelData.cpp
src/TextElement.cpp
src/AudioMixer.cpp
src/SDLInitializers.cpp # REMOVE: cpp
# src/Sprite.cpp # NOTE: remove if not used
)

#REVIEW: remove tests before submitting
add_executable(tests 
# tests/VectorTests.cpp
# src/gMath.cpp
# src/RandNum.cpp
# tests/RandTests.cpp
  src/Vector2d.cpp
  tests/VectorDistanceTest.cpp
)
if (MSVC)
  target_link_libraries(simpleArkanoid ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
else()
  target_link_libraries(simpleArkanoid ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} pthread)
endif()
