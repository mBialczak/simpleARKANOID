cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(simpleArkanoid)

if (MSVC)
    # warning level 4 
    add_compile_options(/W4)
else()
    # lots of warnings 
    add_compile_options(-Wall)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED) 
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} 
${SDL2_MIXER_INCLUDE_DIRS} headers)

add_executable(simpleArkanoid 
src/main.cpp
src/AudioMixer.cpp
src/Ball.cpp
src/Block.cpp
src/Controller.cpp
src/Game.cpp
src/IntervalTimer.cpp
src/LevelData.cpp
src/LimitTimer.cpp
src/MovableObject.cpp
src/Paddle.cpp
src/RandNum.cpp
src/Renderer.cpp
src/SDLexception.cpp
src/SDLInitializers.cpp
src/SideWall.cpp
src/StaticObject.cpp
src/TextElement.cpp
src/Texture.cpp
src/Vector2d.cpp
)

if (MSVC)
  target_link_libraries(simpleArkanoid ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
else()
  target_link_libraries(simpleArkanoid ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} pthread)
endif()
