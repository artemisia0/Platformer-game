#pragma once

#include <AppElement.hpp>
#include <Button.hpp>
#include <Slot.hpp>
#include <Block.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>


class WorkbenchItem : public AppElement {
public:
	WorkbenchItem(Context*, Slot*, int price, sf::Vector2f pos, Block);

	APP_ELEMENT;


private:
	std::unique_ptr<Button> defaultSellButton();
	std::unique_ptr<Button> defaultBuyButton();
	std::unique_ptr<sf::Text> defaultCountLabel();
	std::unique_ptr<sf::Text> defaultPriceLabel();


private:
	Slot* mSlot;
	int mPrice;
	sf::Vector2f mPosition;
	std::unique_ptr<Button> mSellButton;
	std::unique_ptr<Button> mBuyButton;
	sf::Sprite mSprite;
	std::unique_ptr<sf::Text> mCountLabel;
	std::unique_ptr<sf::Text> mPriceLabel;
	Block mBlock;
};
