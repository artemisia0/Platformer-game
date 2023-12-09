#include <Block.hpp>


bool isCollidableBlock(Block block) {
	if (block == Block::Dirt
		or block == Block::DirtWithGrass
		or block == Block::Stone
		or block == Block::Gold
		or block == Block::Diamond
		or block == Block::Sand
		or block == Block::SandWithGrass
		or block == Block::Mud
		or block == Block::MudWithSnow) {
		return true;
	}
	return false;
}

bool isBrightBlock(Block block) {
	if (block == Block::Torch) {
		return true;
	}
	return false;
}
