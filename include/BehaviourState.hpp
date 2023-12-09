#pragma once


enum class BehaviourState {
	Idle,  // Walking and searching for player.
	Targeting,  // Following a player and fighting with him.
};
