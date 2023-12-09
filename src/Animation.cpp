#include <Animation.hpp>

#include <SFML/System/Time.hpp>

#include <cassert>


AnimationItem::AnimationItem(Context* cntx)
	: AppElement(cntx)
	, mPeriod(sf::seconds(0.16))
	, mPlayDeltaTime(sf::seconds(0.02))
	, mElapsedTime(sf::Time::Zero)
	, mSpriteIndex(0)
	, mPlayEnded(false) {
}

void AnimationItem::handleSignal(const Signal& signal) {
}

void AnimationItem::handleEvent(const sf::Event& event) {
}

void AnimationItem::update(sf::Time dt) {
	if (mPlayEnded) {
		mElapsedTime += dt;
	} else {
		mElapsedTime += mPlayDeltaTime;
	}
	
	if (mElapsedTime >= mPeriod) {
		if (mSpriteIndex+1 == int(size())) {
			mPlayEnded = true;
		}
		mSpriteIndex = (mSpriteIndex+1)%int(size());
		mElapsedTime -= mPeriod;
	};
}

bool AnimationItem::playEnded() {
	return mPlayEnded;
}

void AnimationItem::startPlay() {
	mPlayEnded = false;
	mSpriteIndex = 0;
}

void AnimationItem::draw(sf::Time dt) {
}

const sf::Sprite& AnimationItem::sprite() const {
	return at(mSpriteIndex);
}

void AnimationItem::setPeriod(sf::Time period) {
	mPeriod = period;
}

void AnimationItem::setPlayDeltaTime(sf::Time dt) {
	mPlayDeltaTime = dt;
}

void AnimationItem::setActiveSpriteIndex(int index) {
	assert(mSpriteIndex < int(size()));
	mSpriteIndex = index;
}

Animation::Animation(Context* cntx)
	: AppElement(cntx)
	, mItemIndex(0)
	, mIsPlaying(false) {
}

void Animation::handleSignal(const Signal& signal) {
	if (mItemIndex < int(size())) {
		at(mItemIndex).handleSignal(signal);
	}
}

void Animation::handleEvent(const sf::Event& event) {
	if (mItemIndex < int(size())) {
		at(mItemIndex).handleEvent(event);
	}
}

void Animation::update(sf::Time dt) {
	if (mItemIndex < int(size())) {
		at(mItemIndex).update(dt);
		if (at(mItemIndex).playEnded()) {
			mIsPlaying = false;
		}
	}
}

void Animation::draw(sf::Time dt) {
	if (mItemIndex < int(size())) {
		at(mItemIndex).draw(dt);
	}
}

void Animation::setActiveItemIndex(int index) {
	mItemIndex = index;
	assert(mItemIndex < int(size()));
}

const sf::Sprite& Animation::sprite() const {
	return at(mItemIndex).sprite();
}

void Animation::playItemWithIndex(int index) {
	mItemIndex = index;
	at(mItemIndex).startPlay();
	mIsPlaying = true;
}

bool Animation::isPlaying() {
	return mIsPlaying;
}
