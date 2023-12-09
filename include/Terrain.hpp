#pragma once

#include <AppElement.hpp>
#include <Block.hpp>
#include <Chunk.hpp>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>
#include <memory>
#include <array>


class Body;

constexpr int TerrainSizeI = 1<<8;
constexpr int TerrainSizeJ = 1<<10;
constexpr int TerrainChunksI = TerrainSizeI/ChunkSizeI + 1;
constexpr int TerrainChunksJ = TerrainSizeJ/ChunkSizeJ + 1;

template <typename T>
using Grid = std::array<std::array<T, TerrainSizeJ>, TerrainSizeI>;
using BlockGrid = Grid<Block>;

class Terrain : public AppElement,
				public BlockGrid {
public:
	Terrain(Context*);

	APP_ELEMENT;

	bool collides(Body&);

	Grid<signed char>& darkness();


private:
	void generate();
	void highlightBlockUnderCursor();
	sf::Vector2i positionOfBlockUnderCursor();
	void destroyBlock();
	void createBlock();
	void recalculateVertices();
	void recalculateVerticesForChunk(int chunkI, int chunkJ);
	void recalculateVerticesForBlock(int blockI, int blockJ);
	void recalculateDarkness();
	void recalculateDarkness(int blockI, int blockJ, float k);
	void recalculateDarknessVertices();
	void handleCommunicationWithBlock(sf::Vector2i blockPos);


private:
	Grid<signed char> mDarkness;
	std::vector<Chunk> mChunks;  // They contain vertices.
	sf::VertexArray mDarknessVertices;
	sf::Time mTimeSinceLastDarknessRecalculation;
};
