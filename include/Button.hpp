#pragma once

#include <memory>
#include <functional>

#include <AppElement.hpp>
#include <Context.hpp>
#include <WidgetState.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


// Button (interactive clickable GUI widget) class.
// The best practice of creating buttons is using 'defaultButton' function.
class Button : public AppElement {
public:
	Button(Context*,
		   const sf::RectangleShape&,
		   const sf::Text&,
		   sf::Color,
		   sf::Color,
		   sf::Color,
		   std::function<void()>);
	virtual ~Button() = default;

	APP_ELEMENT;


private:
	sf::RectangleShape mRectangleShape;
	sf::Text mText;
	sf::Color mIdleColor;
	sf::Color mHoverColor;
	sf::Color mHitColor;
	WidgetState mState;
	std::function<void()> mCallback;
};

// The best way of creating new buttons.
std::unique_ptr<Button> defaultButton(Context*,
									  sf::RectangleShape,
									  sf::Text,
									  sf::Color,
									  sf::Color,
									  sf::Color,
									  sf::Vector2f,
									  std::function<void()>);
