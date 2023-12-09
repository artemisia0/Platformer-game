#include <Button.hpp>
#include <Util.hpp>
#include <SoundPlayer.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>


Button::Button(Context* cntx,
			   const sf::RectangleShape& rect,
			   const sf::Text& text,
			   sf::Color idle,
			   sf::Color hover,
			   sf::Color hit,
			   std::function<void()> callback)
	: AppElement(cntx)
	, mRectangleShape(rect)
	, mText(text)
	, mIdleColor(idle)
	, mHoverColor(hover)
	, mHitColor(hit)
	, mState(WidgetState::Idle)
	, mCallback(callback) {
	mRectangleShape.setFillColor(mIdleColor);
}

void Button::handleSignal(const Signal& signal) {
}

void Button::handleEvent(const sf::Event& event) {
	auto mousePos = sf::Mouse::getPosition(*context()->window);

	if (not mRectangleShape
		.getGlobalBounds()
		.contains(static_cast<sf::Vector2f>(mousePos))) {
		mState = WidgetState::Idle;
	}

	if (mState == WidgetState::Idle
		and mRectangleShape
		.getGlobalBounds()
		.contains(static_cast<sf::Vector2f>(mousePos))) {
		mState = WidgetState::Hover;
		context()->soundPlayer->play(SoundID::ButtonHover);
	}
	
	if (mState == WidgetState::Hover
		and event.type == sf::Event::MouseButtonPressed) {
		mState = WidgetState::Hit;
		context()->soundPlayer->play(SoundID::ButtonHit);
	}
}

void Button::update(sf::Time dt) {
	if (mState == WidgetState::Hit) {
		mCallback();
		mState = WidgetState::Idle;
	}
	
	if (mState == WidgetState::Idle) {
		mRectangleShape.setFillColor(mIdleColor);
	} else if (mState == WidgetState::Hover) {
		mRectangleShape.setFillColor(mHoverColor);
	} else if (mState == WidgetState::Hit) {
		mRectangleShape.setFillColor(mHitColor);
	}
}

void Button::draw(sf::Time dt) {
	context()->renderTarget->draw(mRectangleShape);
	context()->renderTarget->draw(mText);
}

std::unique_ptr<Button> defaultButton(Context* cntx,
									  sf::RectangleShape rect,
									  sf::Text text,
									  sf::Color idle,
									  sf::Color hover,
									  sf::Color hit,
									  sf::Vector2f position,
									  std::function<void()> callback) {
	rect.setPosition(position);
	text.setPosition(position);
	centerOrigin(&rect);
	centerOrigin(&text);
	
	auto button = std::make_unique<Button>(
		cntx, rect, text, idle, hover, hit, callback);
	return button;
}
