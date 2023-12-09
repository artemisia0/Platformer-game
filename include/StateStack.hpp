#pragma once

#include <AppElement.hpp>
#include <Context.hpp>

#include <SFML/System/NonCopyable.hpp>

#include <vector>


class State;

class StateStack : public AppElement, private sf::NonCopyable {
public:
	StateStack(Context*);

	APP_ELEMENT;

	void push(std::unique_ptr<State>);
	void pop();
	void clear();

	size_t size() const;


private:
	std::vector<std::unique_ptr<State>> mContainer;
};
