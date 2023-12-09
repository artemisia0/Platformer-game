#pragma once

#include <State.hpp>


class GameOverState : public State {
public:
	GameOverState(Context*);

	APP_ELEMENT;
};
