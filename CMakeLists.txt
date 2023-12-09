cmake_minimum_required(VERSION 3.10)
project(PixelDream)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -g -Og)

find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(PixelDream
  src/main.cpp
  src/Log.cpp
  src/App.cpp
  src/State.cpp
  src/StateStack.cpp
  src/MenuState.cpp
  src/Button.cpp
  src/GameState.cpp
  src/PauseState.cpp
  src/OptionsState.cpp
  src/Terrain.cpp
  src/Body.cpp
  src/Animation.cpp
  src/Block.cpp
  src/AppElement.cpp
  src/Inventory.cpp
  src/Slot.cpp
  src/Point.cpp
  src/Pickup.cpp
  src/Chunk.cpp
  src/Character.cpp
  src/Player.cpp
  src/Lizard.cpp
  src/MagicBall.cpp
  src/GameOverState.cpp
  src/Wizard.cpp
  src/Orc.cpp
  src/Parallax.cpp
  src/Weather.cpp
  src/WorkbenchState.cpp
  src/WorkbenchItem.cpp
  src/Util.cpp
  src/SoundPlayer.cpp
  src/MusicPlayer.cpp

  external/stb_perlin.c
)

target_link_libraries(PixelDream
    sfml-graphics
    sfml-window
    sfml-system
	sfml-audio
)

target_include_directories(PixelDream
  PRIVATE ${SFML_INCLUDE_DIRS} include external)
