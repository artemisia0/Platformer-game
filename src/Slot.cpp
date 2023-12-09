#include <Slot.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


Slot::Slot(Context* cntx)
	: AppElement(cntx)
	, std::vector<Pickup>{}
	, mIsActive(false) {
}

void Slot::handleSignal(const Signal& signal) {
}

void Slot::handleEvent(const sf::Event& event) {
}

void Slot::update(sf::Time dt) {
}

void Slot::draw(sf::Time dt) {
	if (size()) {
		at(0).setPosition(position() + sf::Vector2f(48, 48)/2.f);
		at(0).setScale(1.f, 1.f);
		centerOrigin(&at(0));
	}

	sf::RectangleShape rect(sf::Vector2f(48, 48));
	rect.setPosition(position());
	rect.setFillColor(sf::Color(128, 128, 128, 128));
	if (mIsActive) {
		rect.setFillColor(sf::Color(255, 128, 128, 128));
	}
	sf::Text text(std::to_string(size()),
				  *context()->fonts->resource(FontID::Menu),
				  16);
	text.setPosition(position() + sf::Vector2f(48, 48)/2.f);
	centerOrigin(&text);

	context()->renderTarget->draw(rect);
	if (size()) {
		context()->renderTarget->draw(at(0));
		context()->renderTarget->draw(text);
	}
}

void Slot::setActive(bool flag) {
	mIsActive = flag;
}
