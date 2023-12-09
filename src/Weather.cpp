#include <Weather.hpp>

#include <cmath>
#include <cassert>


Weather::Weather(Context* cntx)
	: AppElement(cntx)
	, mDay(1)
	, mDayFactor(1.f)
	, mElapsedTime(sf::Time::Zero) {
}

void Weather::handleSignal(const Signal& signal) {
}

void Weather::handleEvent(const sf::Event& event) {
}

void Weather::update(sf::Time dt) {
	mElapsedTime += dt;
	mDay = sin(mElapsedTime.asSeconds()*mDayFactor)/2.f + 0.5f;
	assert(0.f <= mDay and mDay <= 1.f);
}

void Weather::draw(sf::Time dt) {
}

void Weather::setDayFactor(float factor) {
	mDayFactor = factor;
}

float Weather::day() {
	return mDay;
}
