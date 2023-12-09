#pragma once

#include <SFML/System/Time.hpp>

#include <Context.hpp>


namespace sf {
	class Event;
	class Time;
}
class Signal;


#define ABSTRACT_APP_ELEMENT \
	virtual void handleSignal(const Signal&) = 0; \
	virtual void handleEvent(const sf::Event&) = 0; \
	virtual void update(sf::Time) = 0; \
	virtual void draw(sf::Time) = 0

#define APP_ELEMENT \
	virtual void handleSignal(const Signal&) override; \
	virtual void handleEvent(const sf::Event&) override; \
	virtual void update(sf::Time) override; \
	virtual void draw(sf::Time) override


class AppElement {
public:
	AppElement(Context*);
	virtual ~AppElement() = default;
	
	ABSTRACT_APP_ELEMENT;

	Context* context();
	void setContext(Context*);


private:
	Context* mContext;
};
