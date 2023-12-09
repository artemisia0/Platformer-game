#pragma once

#include <MusicID.hpp>

#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>


class MusicPlayer {
public:
	MusicPlayer();

	void play(MusicID);

	void setVolume(int);
	int volume();

	void reset(MusicID);
	
	bool load(MusicID, const std::string&);

	sf::Music::Status status(MusicID);


private:
	int mVolume;
	std::map<MusicID, sf::Music> mMusic;
	MusicID mLastMusicID;
};
