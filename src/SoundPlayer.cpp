#include <SoundPlayer.hpp>

#include <cassert>


SoundPlayer::SoundPlayer()
	: mVolume(0) {
}

void SoundPlayer::play(SoundID soundID) {
	assert(mBuffers.find(soundID) != mBuffers.end());
	mSound.setBuffer(mBuffers[soundID]);
	mSound.setVolume(mVolume);
	mSound.play();
}

void SoundPlayer::setVolume(int volume) {
	mVolume = volume;
	mSound.setVolume(mVolume);
}

int SoundPlayer::volume() {
	return mVolume;
}

bool SoundPlayer::load(SoundID soundID, const std::string& filename) {
	if (not mBuffers[soundID].loadFromFile(filename)) {
		return false;
	}
	return true;
}
