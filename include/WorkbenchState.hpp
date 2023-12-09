#pragma once

#include <State.hpp>
#include <WorkbenchItem.hpp>
#include <Button.hpp>
#include <Block.hpp>

#include <vector>
#include <memory>

#include <SFML/Graphics/Text.hpp>


class WorkbenchState : public State,
					   public std::vector<WorkbenchItem> {
public:
	WorkbenchState(Context*);

	APP_ELEMENT;


private:
	std::unique_ptr<Button> defaultExitButton();
	std::unique_ptr<sf::Text> defaultCoinsLabel();
	void addItem(Block);


private:
	std::unique_ptr<Button> mExitButton;
	std::unique_ptr<sf::Text> mCoinsLabel;
	sf::Vector2f mNextItemPosition;
};
