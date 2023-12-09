#include <StateStack.hpp>
#include <State.hpp>

#include <SFML/System/Time.hpp>

#include <cassert>


StateStack::StateStack(Context* cntx)
	: AppElement(cntx) {
}

void StateStack::handleSignal(const Signal& signal) {
	if (mContainer.size()) {
		mContainer.back()->handleSignal(signal);
	}
}

void StateStack::handleEvent(const sf::Event& event) {
	if (mContainer.size()) {
		mContainer.back()->handleEvent(event);
	}
}

void StateStack::update(sf::Time dt) {
	if (mContainer.size()) {
		mContainer.back()->update(dt);
	}
}

void StateStack::draw(sf::Time dt) {
	if (mContainer.size()) {
		mContainer.back()->draw(dt);
	}
}

void StateStack::push(std::unique_ptr<State> state) {
	mContainer.push_back(std::move(state));
}

void StateStack::pop() {
	assert(mContainer.size());
	mContainer.pop_back();
}

void StateStack::clear() {
	mContainer.clear();
}

size_t StateStack::size() const {
	return mContainer.size();
}
