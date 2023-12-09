#include <WorkbenchState.hpp>
#include <Signal.hpp>
#include <Util.hpp>
#include <Player.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <cassert>


WorkbenchState::WorkbenchState(Context* cntx)
	: State(cntx)
	, mNextItemPosition(100, 100) {
	mExitButton = defaultExitButton();
	mCoinsLabel = defaultCoinsLabel();

	mNextItemPosition = sf::Vector2f(100, 100);
	addItem(Block::Dirt);
	addItem(Block::DirtWithGrass);
	addItem(Block::Stone);
	addItem(Block::Gold);
	addItem(Block::Diamond);
	mNextItemPosition = sf::Vector2f(100, 400);
	addItem(Block::Sand);
	addItem(Block::SandWithGrass);
	addItem(Block::Mud);
	addItem(Block::MudWithSnow);
	addItem(Block::Workbench);
	mNextItemPosition = sf::Vector2f(100, 700);
	addItem(Block::Torch);
	addItem(Block::Magic1);
	addItem(Block::Magic2);
	addItem(Block::Magic3);
	addItem(Block::Token);
	addItem(Block::HealthBox);
}

void WorkbenchState::handleSignal(const Signal& signal) {
	mExitButton->handleSignal(signal);
	for (auto& item : *this) {
		item.handleSignal(signal);
	}
}

void WorkbenchState::handleEvent(const sf::Event& event) {
	mExitButton->handleEvent(event);
	for (auto& item : *this) {
		item.handleEvent(event);
	}
}

void WorkbenchState::update(sf::Time dt) {
	mExitButton->update(dt);
	mCoinsLabel->setString("Coins: " + std::to_string(
							   context()->player->coins()));
	for (auto& item : *this) {
		item.update(dt);
	}
}

void WorkbenchState::draw(sf::Time dt) {
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	mExitButton->draw(dt);
	for (auto& item : *this) {
		item.draw(dt);
	}
	context()->renderTarget->draw(*mCoinsLabel);
	context()->renderTarget->setView(view);
}

std::unique_ptr<Button> WorkbenchState::defaultExitButton() {
	auto rectSize = sf::Vector2f(128, 32);
	auto rect = sf::RectangleShape(rectSize);
	auto text = sf::Text("Exit",
						 *context()->fonts->resource(FontID::Menu), 20);
	std::unique_ptr<Button> button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(120, 100, 110, 200),
		sf::Color(120, 100, 120, 200),
		sf::Color(120, 100, 130, 200),
		sf::Vector2f(context()->window->getSize().x/2 + 128,
					 context()->window->getSize().y-48),
		[=]()-> void {
			context()->signalQueue->push_back(
				Signal{SignalID::PopState});
		});
	return button;
}

std::unique_ptr<sf::Text> WorkbenchState::defaultCoinsLabel() {
	auto text = std::make_unique<sf::Text>(
		"Coins: " + std::to_string(context()->player->coins()),
		*context()->fonts->resource(FontID::Menu),
		20);
	text->setPosition(sf::Vector2f(context()->window->getSize().x/2 - 128,
								   context()->window->getSize().y-48));
	centerOrigin(text.get());
	return text;
}

void WorkbenchState::addItem(Block block) {
	static std::map<Block, int> price;
	price[Block::Dirt] = 1;
	price[Block::DirtWithGrass] = 2;
	price[Block::Stone] = 2;
	price[Block::Gold] = 5;
	price[Block::Diamond] = 10;
	price[Block::Sand] = 1;
	price[Block::SandWithGrass] = 2;
	price[Block::Mud] = 1;
	price[Block::MudWithSnow] = 2;
	price[Block::Workbench] = 3;
	price[Block::HealthBox] = 40;
	price[Block::Torch] = 15;
	price[Block::Magic1] = 2;
	price[Block::Magic2] = 50;
	price[Block::Magic3] = 200;
	price[Block::Token] = 25;

	WorkbenchItem item(
		context(),
		context()->player->inventory()->slotWithPickup(
			Pickup(context(),
				   PickupID::Block,
				   static_cast<int>(block))),
		price[block],
		mNextItemPosition,
		block);
	push_back(std::move(item));

	mNextItemPosition += sf::Vector2f(320, 0);
}
