#include <MenuState.hpp>
#include <Util.hpp>
#include <Signal.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <functional>
#include <cassert>


MenuState::MenuState(Context* cntx)
	: State(cntx) {

	sf::Vector2f buttonPosition(500, 400);
	buttonPosition.x = context()->window->getSize().x/2.f;
	buttonPosition.y = context()->window->getSize().y/2.f - 60;
	// Utility functor to add new buttons easily.
	auto addButton = [&](const std::string& label,
					std::function<void()> callback) {
		mButtons.push_back(defaultButton(
								context(),
								sf::RectangleShape(
									sf::Vector2f(240, 40)),
								sf::Text(
									label,
									*context()->fonts->resource(FontID::Menu),
									22),
								sf::Color(160, 128, 160, 128),
								sf::Color(160, 160, 160, 144),
								sf::Color(160, 144, 192, 160),
								buttonPosition,
								callback
								));
		buttonPosition.y += 60;
	};
	
	addButton("Play", [&]() {
		context()->signalQueue->push_back(
			Signal{SignalID::PushState, SignalData::Game});
	});
	addButton("Options", [&]() {
		context()->signalQueue->push_back(
			Signal{SignalID::PushState, SignalData::Options});
	});
	addButton("Exit", [&]() {
		context()->signalQueue->push_back(
			Signal{SignalID::ClearStack, SignalData::Options});
	});
}

void MenuState::handleSignal(const Signal& signal) {
	for (auto& button : mButtons)
		button->handleSignal(signal);
}

void MenuState::handleEvent(const sf::Event& event) {
	for (auto& button : mButtons)
		button->handleEvent(event);
}

void MenuState::update(sf::Time dt) {
	for (auto& button : mButtons)
		button->update(dt);
}

void MenuState::draw(sf::Time dt) {
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	sf::RectangleShape bg(sf::Vector2f(10000, 5000));
	bg.setFillColor(sf::Color(0, 0, 0, 128));
	context()->renderTarget->draw(bg);
	for (auto& button : mButtons)
		button->draw(dt);
	context()->renderTarget->setView(view);
}
