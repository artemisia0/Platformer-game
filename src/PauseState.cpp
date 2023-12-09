#include <PauseState.hpp>
#include <Signal.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>


PauseState::PauseState(Context* cntx)
	: State(cntx) {
	mButtons.push_back(defaultContinueButton());
	mButtons.push_back(defaultOptionsButton());
	mButtons.push_back(defaultExitButton());
}

void PauseState::handleSignal(const Signal& signal) {
	for (auto& button : mButtons) {
		button->handleSignal(signal);
	}
}

void PauseState::handleEvent(const sf::Event& event) {
	for (auto& button : mButtons) {
		button->handleEvent(event);
	}
}

void PauseState::update(sf::Time dt) {
	for (auto& button : mButtons) {
		button->update(dt);
	}
}

void PauseState::draw(sf::Time dt) {
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	sf::RectangleShape bg(sf::Vector2f(10000, 5000));
	bg.setFillColor(sf::Color(0, 0, 0, 128));
	context()->renderTarget->draw(bg);
	for (auto& button : mButtons) {
		button->draw(dt);
	}
	context()->renderTarget->setView(view);
}

std::unique_ptr<Button> PauseState::defaultContinueButton() {
	sf::RectangleShape rect(sf::Vector2f(240, 40));
	sf::Text text("Continue", *context()->fonts->resource(FontID::Menu), 22);
	auto [w, h] = context()->window->getSize();
	sf::Vector2f pos(w/2.f, h/2.f - 60.f);
	auto button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(160, 128, 160, 128),
		sf::Color(160, 128, 160, 160),
		sf::Color(160, 160, 160, 160),
		pos,
		[&]() {
			context()->signalQueue->push_back(
				Signal{SignalID::PopState});
		});
	return button;
}

std::unique_ptr<Button> PauseState::defaultOptionsButton() {
	sf::RectangleShape rect(sf::Vector2f(240, 40));
	sf::Text text("Options", *context()->fonts->resource(FontID::Menu), 22);
	auto [w, h] = context()->window->getSize();
	sf::Vector2f pos(w/2.f, h/2.f);
	auto button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(160, 128, 160, 128),
		sf::Color(160, 128, 160, 160),
		sf::Color(160, 160, 160, 160),
		pos,
		[&]() {
			context()->signalQueue->push_back(
				Signal{SignalID::PushState,
					SignalData::Options});
		});
	return button;
}

std::unique_ptr<Button> PauseState::defaultExitButton() {
	sf::RectangleShape rect(sf::Vector2f(240, 40));
	sf::Text text("Exit", *context()->fonts->resource(FontID::Menu), 22);
	auto [w, h] = context()->window->getSize();
	sf::Vector2f pos(w/2.f, h/2.f + 60.f);
	auto button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(160, 128, 160, 128),
		sf::Color(160, 128, 160, 160),
		sf::Color(160, 160, 160, 160),
		pos,
		[&]() {
			context()->signalQueue->push_back(
				Signal{SignalID::ClearStack});
			context()->signalQueue->push_back(
				Signal{SignalID::PushState,
					SignalData::Menu});
		});
	return button;
}
