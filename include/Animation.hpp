#pragma once

#include <AppElement.hpp>
#include <Context.hpp>

#include <vector>

#include <SFML/Graphics/Sprite.hpp>


// Animation item is just a set of frames.
// Frame that is to be drawn is changed with some mPeriod period.
class AnimationItem : public AppElement, public std::vector<sf::Sprite> {
public:
	AnimationItem(Context*);
	virtual ~AnimationItem() = default;

	APP_ELEMENT;

	void setPeriod(sf::Time);
	void setPlayDeltaTime(sf::Time);
	
	const sf::Sprite& sprite() const;

	void setActiveSpriteIndex(int);

	bool playEnded();
	void startPlay();


private:
	sf::Time mPeriod;
	sf::Time mPlayDeltaTime;
	sf::Time mElapsedTime;
	int mSpriteIndex;
	bool mPlayEnded;
};

// Animation is a wrapper over a set of an AnimationItem.
class Animation : public AppElement, public std::vector<AnimationItem> {
public:
	Animation(Context*);

	APP_ELEMENT;

	void setActiveItemIndex(int);
	void playItemWithIndex(int);

	bool isPlaying();
	
	const sf::Sprite& sprite() const;


private:
	int mItemIndex;
	bool mIsPlaying;
};
