#pragma once

#include <AppElement.hpp>

#include <SFML/System/Time.hpp>


class Weather : public AppElement {
public:
	Weather(Context*);

	APP_ELEMENT;

	void setDayFactor(float);

	float day();


private:
	float mDay;  // Normalized value, belongs to the range [0; 1.0]
	             // (where 0 is a night and 1 is a day);
	float mDayFactor;
	sf::Time mElapsedTime;
};
