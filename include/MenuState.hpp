#pragma once

#include <State.hpp>
#include <Button.hpp>
#include <Parallax.hpp>

#include <memory>
#include <vector>


class MenuState : public State {
public:
	MenuState(Context*);
	
	APP_ELEMENT;


private:
	std::vector<std::unique_ptr<Button>> mButtons;
};
