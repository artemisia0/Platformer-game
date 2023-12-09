#pragma once

#define TILE 32


enum class Block {
	None,
	Dirt,
	DirtWithGrass,
	Stone,
	Gold,
	Diamond,
	Sand,
	SandWithGrass,
	Mud,
	MudWithSnow,
	Workbench,
	HealthBox,
	Torch,
	Magic1,
	Magic2,
	Magic3,
	Token,
};


bool isCollidableBlock(Block);
bool isBrightBlock(Block);
