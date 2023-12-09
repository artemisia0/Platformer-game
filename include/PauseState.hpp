#pragma once

#include <State.hpp>
#include <Button.hpp>

#include <memory>
#include <vector>


class PauseState : public State {
public:
	PauseState(Context*);

	APP_ELEMENT;


private:
	std::unique_ptr<Button> defaultContinueButton();
	std::unique_ptr<Button> defaultOptionsButton();
	std::unique_ptr<Button> defaultExitButton();


private:
	std::vector<std::unique_ptr<Button>> mButtons;
};
