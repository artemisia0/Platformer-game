#pragma once

#include <Resources.hpp>

#include <deque>


namespace sf {
	class RenderTarget;
	class Window;
}
class Signal;
class Player;
class Terrain;
class GameState;
class Weather;
class MusicPlayer;
class SoundPlayer;
class Parallax;

using SignalQueue = std::deque<Signal>;

class Context {
public:
	sf::RenderTarget* renderTarget;
	sf::Window* window;
	TextureResources* textures;
	FontResources* fonts;
	SignalQueue* signalQueue;
	Player* player;
	Terrain* terrain;
	GameState* gameState;
	Weather* weather;
	MusicPlayer* musicPlayer;
	SoundPlayer* soundPlayer;
	Parallax* parallax;
};
