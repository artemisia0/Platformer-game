#include <WorkbenchItem.hpp>
#include <Util.hpp>
#include <Player.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>


WorkbenchItem::WorkbenchItem(Context* cntx, Slot* slot, int price,
							 sf::Vector2f position, Block block)
	: AppElement(cntx)
	, mSlot(slot)
	, mPrice(price)
	, mPosition(position)
	, mBlock(block) {
	assert(context() and cntx);
	mSellButton = defaultSellButton();
	mBuyButton = defaultBuyButton();
	mSprite = static_cast<sf::Sprite>(
		Pickup(context(),
			   PickupID::Block,
			   static_cast<int>(block)));
	mSprite.setPosition(mPosition);
	assert(context() and cntx);
	mCountLabel = defaultCountLabel();
	mPriceLabel = defaultPriceLabel();
	mSprite.setScale(2.0, 2.0);
	mSprite.setPosition(mSprite.getPosition()-sf::Vector2f(32, -32));
	mCountLabel->setString("Count: 0");
	if (mSlot) {
		mCountLabel->setString("Count: " + std::to_string(mSlot->size()));
	}
}

void WorkbenchItem::handleSignal(const Signal& signal) {
	mSellButton->handleSignal(signal);
	mBuyButton->handleSignal(signal);
}

void WorkbenchItem::handleEvent(const sf::Event& event) {
	mSellButton = defaultSellButton();
	mBuyButton = defaultBuyButton();
	
	mSellButton->handleEvent(event);
	mBuyButton->handleEvent(event);
}

void WorkbenchItem::update(sf::Time dt) {
	mSellButton->update(dt);
	mBuyButton->update(dt);
	assert(mCountLabel and mPriceLabel);
}

void WorkbenchItem::draw(sf::Time dt) {
	mSellButton->draw(dt);
	mBuyButton->draw(dt);
	context()->renderTarget->draw(mSprite);
	context()->renderTarget->draw(*mCountLabel);
	context()->renderTarget->draw(*mPriceLabel);
}

std::unique_ptr<Button> WorkbenchItem::defaultSellButton() {
	auto rect = sf::RectangleShape(sf::Vector2f(128, 32));
	auto text = sf::Text("Sell",
						 *context()->fonts->resource(FontID::Menu), 20);
	auto button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(128, 64, 80, 180),
		sf::Color(128, 64, 90, 200),
		sf::Color(128, 80, 60, 220),
		mPosition + sf::Vector2f(80, 0),
		[&]() {
			if (context()
				and context()->player->inventory()->count(
					Pickup(context(), PickupID::Block,
						   static_cast<int>(mBlock)))) {
				context()->player->inventory()->takePickup(
					Pickup(context(), PickupID::Block,
						   static_cast<int>(mBlock)));
				mCountLabel->setString(
					"Count: " + std::to_string(
						context()->player->inventory()->count(
							Pickup(context(), PickupID::Block,
								   static_cast<int>(mBlock)))));
				context()->player->setCoins(
					context()->player->coins()+mPrice);
			}
		});
	return button;
}

std::unique_ptr<Button> WorkbenchItem::defaultBuyButton() {
	auto rect = sf::RectangleShape(sf::Vector2f(128, 32));
	auto text = sf::Text("Buy",
						 *context()->fonts->resource(FontID::Menu), 20);
	auto button = defaultButton(
		context(),
		rect,
		text,
		sf::Color(128, 64, 80, 180),
		sf::Color(128, 64, 90, 200),
		sf::Color(128, 80, 60, 220),
		mPosition + sf::Vector2f(80, 48),
		[=]() {
			if (context() and context()->player->coins()-mPrice >= 0) {
				context()->player->inventory()->add(
					Pickup(context(), PickupID::Block,
						   static_cast<int>(mBlock)));
				mCountLabel->setString(
					"Count: " + std::to_string(
						context()->player->inventory()->count(
							Pickup(context(), PickupID::Block,
								   static_cast<int>(mBlock)))));
				context()->player->setCoins(
					context()->player->coins()-mPrice);
			}
		});
	return button;
}

std::unique_ptr<sf::Text> WorkbenchItem::defaultCountLabel() {
	auto text = std::make_unique<sf::Text>(
		"Count: none",
		*context()->fonts->resource(FontID::Menu),
		20);
	text->setPosition(mPosition + sf::Vector2f(80, 2*48));
	centerOrigin(text.get());
	return text;
}

std::unique_ptr<sf::Text> WorkbenchItem::defaultPriceLabel() {
	auto text = std::make_unique<sf::Text>(
		"Price: " + std::to_string(mPrice),
		*context()->fonts->resource(FontID::Menu),
		20);
	text->setPosition(mPosition + sf::Vector2f(80, 3*48));
	centerOrigin(text.get());
	return text;
}
