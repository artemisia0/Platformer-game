#include <OptionsState.hpp>
#include <Signal.hpp>
#include <Util.hpp>
#include <SoundPlayer.hpp>
#include <MusicPlayer.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


OptionsState::OptionsState(Context* cntx)
	: State(cntx) {
	mExitButton = defaultExitButton();
	mMusicVolumeSlider = defaultMusicVolumeSlider();
	mSoundsVolumeSlider = defaultSoundsVolumeSlider();
	mMusicVolumeSliderText = defaultMusicVolumeSliderText();
	mSoundsVolumeSliderText = defaultSoundsVolumeSliderText();
}

std::vector<Button> OptionsState::defaultMusicVolumeSlider() {
	std::vector<Button> vec;
	auto [w, h] = context()->window->getSize();
	for (int i = 0; i != 21; ++i) {
		auto pos = sf::Vector2f(300 + w/6.f + 48.f*i, h/3.f + 24);
		int volume = i*5;
		sf::RectangleShape rect(sf::Vector2f(32.f, 48.f));
		sf::Text text(std::to_string(volume),
					  *context()->fonts->resource(FontID::Menu),
					  18);
		auto button = defaultButton(
			context(),
			rect,
			text,
			sf::Color(160, 128, 160, 128),
			sf::Color(160, 160, 160, 160),
			sf::Color(160, 160, 160, 160),
			pos,
			[=]() {
				context()->musicPlayer->setVolume(volume);
			});
		vec.push_back(*button);
	}
	return vec;
}

std::vector<Button> OptionsState::defaultSoundsVolumeSlider() {
	std::vector<Button> vec;
	auto [w, h] = context()->window->getSize();
	for (int i = 0; i != 21; ++i) {
		auto pos = sf::Vector2f(300 + w/6.f + 48.f*i, h/3.f + 24 + 200);
		int volume = i*5;
		sf::RectangleShape rect(sf::Vector2f(32.f, 48.f));
		sf::Text text(std::to_string(volume),
					  *context()->fonts->resource(FontID::Menu),
					  18);
		auto button = defaultButton(
			context(),
			rect,
			text,
			sf::Color(160, 128, 160, 128),
			sf::Color(160, 160, 160, 160),
			sf::Color(160, 160, 160, 160),
			pos,
			[=]() {
				context()->soundPlayer->setVolume(volume);
			});
		vec.push_back(*button);
	}
	return vec;
}

std::unique_ptr<sf::Text> OptionsState::defaultMusicVolumeSliderText() {
	auto text = std::make_unique<sf::Text>(
		"Music Volume",
		*context()->fonts->resource(FontID::Menu),
		24);
	auto [w, h] = context()->window->getSize();
	text->setPosition(w/6.f, h/3.);
	return text;
}

std::unique_ptr<sf::Text> OptionsState::defaultSoundsVolumeSliderText() {
	auto text = std::make_unique<sf::Text>(
		"Sounds Volume",
		*context()->fonts->resource(FontID::Menu),
		24);
	auto [w, h] = context()->window->getSize();
	text->setPosition(w/6.f, h/3 + 200);
	return text;
}

void OptionsState::handleSignal(const Signal& signal) {
	mExitButton->handleSignal(signal);
	for (auto& button : mMusicVolumeSlider) {
		button.handleSignal(signal);
	}
	for (auto& button : mSoundsVolumeSlider) {
		button.handleSignal(signal);
	}
}

void OptionsState::handleEvent(const sf::Event& event) {
	mExitButton->handleEvent(event);
	for (auto& button : mMusicVolumeSlider) {
		button.handleEvent(event);
	}
	for (auto& button : mSoundsVolumeSlider) {
		button.handleEvent(event);
	}
}

void OptionsState::update(sf::Time dt) {
	mExitButton->update(dt);
	for (auto& button : mMusicVolumeSlider) {
		button.update(dt);
	}
	for (auto& button : mSoundsVolumeSlider) {
		button.update(dt);
	}
}

void OptionsState::draw(sf::Time dt) {
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	sf::RectangleShape bg(sf::Vector2f(10000, 5000));
	bg.setFillColor(sf::Color(0, 0, 0, 160));
	context()->renderTarget->draw(bg);
	mExitButton->draw(dt);
	for (auto& button : mMusicVolumeSlider) {
		button.draw(dt);
	}
	for (auto& button : mSoundsVolumeSlider) {
		button.draw(dt);
	}
	context()->renderTarget->draw(*mMusicVolumeSliderText);
	context()->renderTarget->draw(*mSoundsVolumeSliderText);
	context()->renderTarget->setView(view);
}

std::unique_ptr<Button> OptionsState::defaultExitButton() {
	auto rect = sf::RectangleShape(sf::Vector2f(128, 32));
	auto text = sf::Text("Exit",
						 *context()->fonts->resource(FontID::Menu), 20);
	auto button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(128, 128, 128, 128),
		sf::Color(160, 160, 128, 160),
		sf::Color(160, 160, 160, 160),
		sf::Vector2f(context()->window->getSize().x/2.f,
					 context()->window->getSize().y-100.f),
		[=]() {
			context()->signalQueue->push_back(Signal{
					SignalID::PopState});
		});
	return button;
}
