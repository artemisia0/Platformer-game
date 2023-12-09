#include <GameState.hpp>
#include <Lizard.hpp>
#include <Wizard.hpp>
#include <Orc.hpp>
#include <Signal.hpp>
#include <MagicBall.hpp>
#include <Util.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cstdlib>
#include <utility>
#include <cassert>


std::unique_ptr<Player> GameState::defaultPlayer() {
	auto player = std::make_unique<Player>(context());
	return player;
}

std::unique_ptr<Terrain> GameState::defaultTerrain() {
	auto terrain = std::make_unique<Terrain>(context());

	// terrain generation etc.
	
	return terrain;
}

std::unique_ptr<Weather> GameState::defaultWeather() {
	auto weather = std::make_unique<Weather>(context());
	weather->setDayFactor(0.02);
	return weather;
}

GameState::GameState(Context* cntx)
	: State(cntx)
	, mTimeSinceLastSpawn(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero) {
	cntx->gameState = this;
	setContext(cntx);

	mWeather = defaultWeather();
	context()->weather = mWeather.get();
	mPlayer = defaultPlayer().release();
	context()->player = mPlayer;
	mTerrain = defaultTerrain();
	context()->terrain = mTerrain.get();

	mBodies.push_back(std::unique_ptr<Player>(mPlayer));
}

void GameState::handleSignal(const Signal& signal) {
	for (int i = 0, lim = int(mBodies.size()); i != lim; ++i) {
		mBodies.at(i)->handleSignal(signal);
	}
	mTerrain->handleSignal(signal);
}

void GameState::handleEvent(const sf::Event& event) {
	for (int i = 0, lim = int(mBodies.size()); i != lim; ++i) {
		mBodies.at(i)->handleEvent(event);
	}
	mTerrain->handleEvent(event);

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			context()->signalQueue->push_back(
				Signal{SignalID::PushState,
					SignalData::Pause});
		}
	}
}

void GameState::update(sf::Time dt) {
	mElapsedTime += dt;
	
	spawnNPCs(dt);
	mWeather->update(dt);
	mTerrain->update(dt);
	
	for (int i = 0, lim = int(mBodies.size()); i != lim; ++i) {
		mBodies.at(i)->update(dt);
	}

	std::vector<std::vector<std::unique_ptr<Body>>::iterator> bodiesToRemove;
	for (auto iter = mBodies.begin(); iter != mBodies.end(); ++iter) {
		if (iter[0]->markedForRemoval()) {
			if (iter[0]->type() == BodyType::NPC) {
				Character* character = dynamic_cast<Character*>(iter->get());
				assert(character);
				handleNPCRemoval(character);
			}
			if (iter[0]->type() == BodyType::Player) {
				context()->signalQueue->push_back(
					Signal{SignalID::PushState,
						SignalData::GameOver});
			}			
			bodiesToRemove.push_back(iter);
		}
	}
	for (auto& body : bodiesToRemove) {
		mBodies.erase(body);
	}
}

void GameState::draw(sf::Time dt) {
	mTerrain->draw(dt);
	for (int i = 0, lim = mBodies.size(); i != lim; ++i) {
		mBodies.at(i)->draw(dt);
	}

	sf::Text elapsedTimeText(
		std::to_string(int(mElapsedTime.asSeconds())) + " sec",
		*context()->fonts->resource(FontID::Menu),
		18);
	elapsedTimeText.setPosition(10, context()->window->getSize().y-32);
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	context()->renderTarget->draw(elapsedTimeText);
	context()->renderTarget->setView(view);

	for (auto& body : mBodiesToAdd) {
		mBodies.push_back(std::move(body));
	}
	mBodiesToAdd.clear();
}

bool GameState::collides(Body& body) {
	if (mTerrain->collides(body)) {
		return true;
	}
	return false;
}

std::vector<std::unique_ptr<Body>>* GameState::bodies() {
	return &mBodies;
}

void GameState::spawnNPCs(sf::Time dt) {
	mTimeSinceLastSpawn += dt;
	const sf::Time period = std::max(sf::seconds(0.5),
								 sf::seconds(4*context()->weather->day()));
	if (mTimeSinceLastSpawn > period) {
		mTimeSinceLastSpawn -= period;
		if (rand()%5 == 0) {
			spawnLizard();
		} else if (rand()%3 == 0) {
			spawnWizard();
		} else {
			spawnOrc();
		}
	}
}

void GameState::spawnLizard() {
	auto pos = mPlayer->position();
	pos.y += (rand()%800)*(rand()%2 ? -1 : 1);
	pos.x += (rand()%400) * (rand()%2 ? -1 : 1);
	auto lizard = std::make_unique<Lizard>(context());
	lizard->setPosition(pos);
	mBodies.push_back(std::move(lizard));
}

void GameState::spawnWizard() {
	auto pos = mPlayer->position();
	pos.y += (rand()%800)*(rand()%2 ? -1 : 1);
	pos.x += (rand()%400) * (rand()%2 ? -1 : 1);
	auto wizard = std::make_unique<Wizard>(context());
	wizard->setPosition(pos);
	mBodies.push_back(std::move(wizard));
}

void GameState::spawnOrc() {
	auto pos = mPlayer->position();
	pos.y += (rand()%800)*(rand()%2 ? -1 : 1);
	pos.x += (rand()%400) * (rand()%2 ? -1 : 1);
	auto orc = std::make_unique<Orc>(context());
	orc->setPosition(pos);
	mBodies.push_back(std::move(orc));
}

void GameState::handleNPCRemoval(Character* character) {
	Orc* orc = dynamic_cast<Orc*>(character);
	Wizard* wizard = dynamic_cast<Wizard*>(character);
	Lizard* lizard = dynamic_cast<Lizard*>(character);
	if (orc) {
		handleOrcRemoval(orc);
	} else if (wizard) {
		handleWizardRemoval(wizard);
	} else if (lizard) {
		handleLizardRemoval(lizard);
	}
}

void GameState::handleOrcRemoval(Orc* orc) {
	Block block(Block::None);
	int randVal = rand()%4;
	if (randVal == 0) {
		block = Block::Workbench;
	} else if (randVal == 1) {
		block = Block::Torch;
	}

	if (block == Block::None) {
		return;
	}
	assert(block == Block::Workbench or block == Block::Torch);
	auto pickup = std::make_unique<Pickup>(
		context(),
		PickupID::Block,
		static_cast<int>(block),
		orc->position(),
		[=]()->void {
			context()->player->inventory()->add(
				Pickup(
					context(),
					PickupID::Block,
					static_cast<int>(block)));
		});
	assert(pickup);
	mBodiesToAdd.push_back(std::move(pickup));
}

void GameState::handleLizardRemoval(Lizard* lizard) {
	Block block(Block::None);
	int randVal = rand()%2;
	if (randVal == 0) {
		block = Block::Token;
	}

	if (block == Block::None) {
		return;
	}
	auto pickup = std::make_unique<Pickup>(
		context(),
		PickupID::Block,
		static_cast<int>(block),
		lizard->position(),
		[=]()->void {
			context()->player->inventory()->add(
				Pickup(
					context(),
					PickupID::Block,
					static_cast<int>(block)));
		});
	mBodiesToAdd.push_back(std::move(pickup));
}

void GameState::handleWizardRemoval(Wizard* wizard) {
	Block block(Block::None);
	int randVal = rand()%5;
	MagicType magicType = MagicType::First;
	if (randVal == 1) {
		block = Block::Magic2;
		magicType = MagicType::Second;
	} else if (randVal == 2) {
		block = Block::Magic3;
		magicType = MagicType::Third;
	}

	if (block == Block::None) {
		return;
	}
	auto pickup = std::make_unique<Pickup>(
		context(),
		PickupID::Block,
		static_cast<int>(block),
		wizard->position(),
		[=]()->void {
			context()->player->inventory()->add(
				Pickup(
					context(),
					PickupID::Block,
					static_cast<int>(block)));
			context()->player->setMagicType(magicType);
		});
	mBodiesToAdd.push_back(std::move(pickup));
}
