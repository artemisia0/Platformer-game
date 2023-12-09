#pragma once

#include <Context.hpp>
#include <AppElement.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>


class State : public AppElement, private sf::NonCopyable {
public:
	State(Context*);
	virtual ~State() = default;

	ABSTRACT_APP_ELEMENT;
};
