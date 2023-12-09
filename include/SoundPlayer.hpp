#pragma once

#include <SoundID.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>
#include <map>


class SoundPlayer {
public:
	SoundPlayer();

	void play(SoundID);

	void setVolume(int);
	int volume();

	bool load(SoundID, const std::string&);


private:
	int mVolume;
	std::map<SoundID, sf::SoundBuffer> mBuffers;
	sf::Sound mSound;
};
