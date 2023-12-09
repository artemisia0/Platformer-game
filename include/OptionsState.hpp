#pragma once

#include <State.hpp>
#include <Button.hpp>
#include <Parallax.hpp>

#include <SFML/Graphics/Text.hpp>

#include <memory>
#include <vector>


class OptionsState : public State {
public:
	OptionsState(Context*);

	APP_ELEMENT;


private:
	std::unique_ptr<Button> defaultExitButton();
	std::vector<Button> defaultMusicVolumeSlider();
	std::vector<Button> defaultSoundsVolumeSlider();
	std::unique_ptr<sf::Text> defaultMusicVolumeSliderText();
	std::unique_ptr<sf::Text> defaultSoundsVolumeSliderText();


private:
	std::unique_ptr<Button> mExitButton;
	std::vector<Button> mMusicVolumeSlider;
	std::vector<Button> mSoundsVolumeSlider;
	std::unique_ptr<sf::Text> mMusicVolumeSliderText;
	std::unique_ptr<sf::Text> mSoundsVolumeSliderText;
};
