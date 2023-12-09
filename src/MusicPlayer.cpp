#include <MusicPlayer.hpp>


MusicPlayer::MusicPlayer()
	: mVolume(0)
	, mLastMusicID(MusicID::Main) {
}

void MusicPlayer::play(MusicID musicID) {
	mMusic[musicID].setVolume(mVolume);
	mMusic[musicID].play();
	mLastMusicID = musicID;
}

void MusicPlayer::setVolume(int volume) {
	mVolume = volume;
	mMusic[mLastMusicID].setVolume(mVolume);
}

int MusicPlayer::volume() {
	return mVolume;
}

bool MusicPlayer::load(MusicID musicID, const std::string& filename) {
	if (not mMusic[musicID].openFromFile(filename)) {
		return false;
	}
	return true;
}

sf::Music::Status MusicPlayer::status(MusicID musicID) {
	return mMusic[musicID].getStatus();
}

void MusicPlayer::reset(MusicID musicID) {
	mMusic[musicID].setPlayingOffset(sf::Time::Zero);
}
