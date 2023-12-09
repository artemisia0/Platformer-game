#pragma once

#include <State.hpp>
#include <Terrain.hpp>
#include <Player.hpp>
#include <Pickup.hpp>
#include <Parallax.hpp>
#include <Weather.hpp>

#include <memory>
#include <vector>


class Orc;
class Lizard;
class Wizard;

class GameState : public State {
public:
	GameState(Context*);
	virtual ~GameState() = default;

	APP_ELEMENT;

	bool collides(Body&);

	std::vector<std::unique_ptr<Body>>* bodies();
	std::unique_ptr<Weather> defaultWeather();


private:
	std::unique_ptr<Player> defaultPlayer();
	std::unique_ptr<Terrain> defaultTerrain();	

	void spawnNPCs(sf::Time);
	void spawnLizard();
	void spawnWizard();
	void spawnOrc();

	void handleNPCRemoval(Character*);
	void handleOrcRemoval(Orc*);
	void handleLizardRemoval(Lizard*);
	void handleWizardRemoval(Wizard*);


private:
	Player* mPlayer;
	std::unique_ptr<Terrain> mTerrain;
	std::vector<std::unique_ptr<Body>> mBodies;
	// It is bad idea to add bodies directory to mBodies.
	std::vector<std::unique_ptr<Body>> mBodiesToAdd;
	sf::Time mTimeSinceLastSpawn;
	std::unique_ptr<Weather> mWeather;
	sf::Time mElapsedTime;
};
