#include <GameOverState.hpp>
#include <Util.hpp>
#include <Signal.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


GameOverState::GameOverState(Context* cntx)
	: State(cntx) {
}

void GameOverState::handleSignal(const Signal& signal) {
}

void GameOverState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		context()->signalQueue->push_back(
			Signal{SignalID::PushState,
				SignalData::Menu});
	}
}

void GameOverState::update(sf::Time dt) {
}

void GameOverState::draw(sf::Time dt) {
	auto oldView = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	sf::Text text("You were killed...",
				  *context()->fonts->resource(FontID::Menu),
				  32);
	text.setPosition(
		static_cast<sf::Vector2f>(
			context()->window->getSize())/2.f);
	centerOrigin(&text);
	context()->renderTarget->draw(text);
	context()->renderTarget->setView(oldView);
}
