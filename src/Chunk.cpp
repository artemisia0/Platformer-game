#include <Chunk.hpp>
#include <Terrain.hpp>
#include <Block.hpp>
#include <Player.hpp>
#include <GameState.hpp>

#include <stb_perlin.h>

#include <cassert>
#include <cmath>
#include <functional>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


Chunk::Chunk(Context* cntx, sf::Vector2i chunkPos)
	: AppElement(cntx)
	, sf::Vector2i(chunkPos) {
	clear();
	setPrimitiveType(sf::Triangles);
}

void Chunk::handleSignal(const Signal& signal) {
}

void Chunk::handleEvent(const sf::Event& event) {
}

void Chunk::update(sf::Time dt) {
}

void Chunk::draw(sf::Time dt) {
	sf::RenderStates states;
	states.texture = context()->textures->resource(TextureID::Terrain);
	context()->renderTarget->draw(*this, states);
}

void Chunk::recalculateVertices() {
	clear();
	setPrimitiveType(sf::Triangles);
	for (int i = 0; i != ChunkSizeI; ++i) {
		for (int j = 0; j != ChunkSizeJ; ++j) {
			auto appendBlock = [&](sf::Vector2f texCoords,
								   sf::VertexArray& vertices) {
				sf::Vertex vertex;
				vertex.position = sf::Vector2f((j + y*ChunkSizeJ)*32,
											   -(i + x*ChunkSizeI)*32);
				vertex.texCoords = texCoords;
				vertices.append(vertex);
				vertex.position.x += TILE;
				vertex.texCoords.x += TILE;
				vertices.append(vertex);
				vertex.position.y += TILE;
				vertex.texCoords.y += TILE;
				vertices.append(vertex);
				vertices.append(vertex);
				vertex.position.x -= TILE;
				vertex.texCoords.x -= TILE;
				vertices.append(vertex);
				vertex.position.y -= TILE;
				vertex.texCoords.y -= TILE;
				vertices.append(vertex);
			};

			auto& blocks = *context()->terrain;

			int blockI = i + x*ChunkSizeI;
			int blockJ = j + y*ChunkSizeJ;
			if (blockI < 0 or blockI >= TerrainSizeI
				or blockJ < 0 or blockJ >= TerrainSizeJ) {
				continue;
			}
			
			if (blocks.at(blockI).at(blockJ) == Block::Dirt) {
				appendBlock(sf::Vector2f(0, 0), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::DirtWithGrass) {
				appendBlock(sf::Vector2f(0, 32), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Stone) {
				appendBlock(sf::Vector2f(0, 32*2), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Gold) {
				appendBlock(sf::Vector2f(0, 32*3), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Diamond) {
				appendBlock(sf::Vector2f(0, 32*4), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Sand) {
				appendBlock(sf::Vector2f(0, 32*5), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::SandWithGrass) {
				appendBlock(sf::Vector2f(0, 32*6), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Mud) {
				appendBlock(sf::Vector2f(0, 32*7), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::MudWithSnow) {
				appendBlock(sf::Vector2f(0, 32*8), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Torch) {
				appendBlock(sf::Vector2f(0, 32*19), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Magic1) {
				appendBlock(sf::Vector2f(0, 32*13), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Magic2) {
				appendBlock(sf::Vector2f(0, 32*14), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Magic3) {
				appendBlock(sf::Vector2f(0, 32*15), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Workbench) {
				appendBlock(sf::Vector2f(0, 32*16), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::HealthBox) {
				appendBlock(sf::Vector2f(0, 32*17), *this);
			} else if (blocks.at(blockI).at(blockJ) == Block::Token) {
				appendBlock(sf::Vector2f(0, 32*18), *this);
			}
		}
	}
}

