#include <Terrain.hpp>
#include <Block.hpp>
#include <Player.hpp>
#include <GameState.hpp>
#include <Chunk.hpp>
#include <Util.hpp>
#include <Signal.hpp>
#include <SoundPlayer.hpp>

#include <stb_perlin.h>

#include <cassert>
#include <cmath>
#include <functional>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


Terrain::Terrain(Context* cntx)
	: AppElement(cntx)
	, mTimeSinceLastDarknessRecalculation(sf::Time::Zero) {
	context()->terrain = this;

	// Filling grids with initial values.
	for (int i = 0; i != TerrainSizeI; ++i) {
		for (int j = 0; j != TerrainSizeJ; ++j) {
			at(i).at(j) = Block::None;
		}
	}

	assert(TerrainSizeI % ChunkSizeI == 0
		   and TerrainSizeJ % ChunkSizeJ == 0);

	// Creating chunks.
	for (int i = 0; i != TerrainChunksI; ++i) {
		for (int j = 0; j != TerrainChunksJ; ++j) {
			mChunks.push_back(Chunk(context(), sf::Vector2i(i, j)));
		}
	}

	generate();  // Generate terrain with some algorithm.
	
	recalculateVertices();

	/*
	// Add workbench to player's inventory.
	// TODO: remove this.
	context()->player->inventory()->add(
		Pickup(
			context(),
			PickupID::Block,
			static_cast<int>(Block::Workbench)));
	*/
}

void Terrain::handleSignal(const Signal& signal) {
}

void Terrain::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			destroyBlock();
		}
		if (event.mouseButton.button == sf::Mouse::Button::Right) {
			createBlock();
		}
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Space) {
			handleCommunicationWithBlock(positionOfBlockUnderCursor());
		}
	}
}

void Terrain::handleCommunicationWithBlock(sf::Vector2i blockPos) {
	blockPos = blockPos/TILE;
	auto block = at(-blockPos.y).at(blockPos.x);

	if (block == Block::Workbench) {
		context()->signalQueue->push_back(
			Signal{
				SignalID::PushState,
				SignalData::Workbench});
	} else if (block == Block::HealthBox) {
		context()->player->health += 20;
		
		at(-blockPos.y).at(blockPos.x) = Block::None;
		recalculateVerticesForBlock(-blockPos.y, blockPos.x);
		recalculateDarkness();
		recalculateDarknessVertices();
	}
}

void Terrain::update(sf::Time dt) {
	mTimeSinceLastDarknessRecalculation += dt;
	if (mTimeSinceLastDarknessRecalculation > sf::seconds(0.4)) {
		mTimeSinceLastDarknessRecalculation -= sf::seconds(0.4);
		recalculateDarkness();
		recalculateDarknessVertices();
	}
}

bool Terrain::collides(Body& body) {
	auto bodyBounds = body.bounds();
	int bodyJ = bodyBounds.left/TILE;
	int bodyI = -bodyBounds.top/TILE;

	const int delta = 8;
	for (int i = bodyI-delta; i < bodyI+delta; ++i) {
		for (int j = bodyJ-delta; j < bodyJ+delta; ++j) {
			sf::FloatRect blockBounds(j*TILE, -i*TILE, TILE, TILE);
			if (i >= 0 and i < TerrainSizeI
				and j >= 0 and j < TerrainSizeJ
				and isCollidableBlock(at(i).at(j))
				and blockBounds.intersects(bodyBounds)) {
				return true;
			}
		}
	}
	return false;
}

void generateTerrain(BlockGrid& grid) {
	auto filter = [&](float value)-> float {
		return std::min(std::max(value, 10.f), float(TerrainSizeI)-10);
	};
	
	constexpr float biomFreq = 0.005f;
	constexpr float desertFreq = 0.08f;
	constexpr float mountainsFreq = 0.08f;
	constexpr float forestFreq = 0.08f;
	constexpr float fieldFreq = 0.08f;
	constexpr float stoneFreq = 0.08f;
	constexpr float caveFreq = 0.08;
	
	for (int j = 0; j != TerrainSizeJ; ++j) {
		float biomNoise = 2*stb_perlin_noise3(j*biomFreq, -j*biomFreq,
											  j*biomFreq, 0, 0, 0);
		// Desert
		if (-1.f <= biomNoise and biomNoise < -0.5f) {
			float noise = stb_perlin_fbm_noise3(
				j*desertFreq, j*desertFreq, -j*desertFreq, 2.0, 0.5, 6);
			int i = filter(noise*8 + TerrainSizeI/1.5f);
			grid.at(i).at(j) = Block::SandWithGrass;
			for (int ti = 0; ti != i and ti < TerrainSizeI; ++ti) {
				grid.at(ti).at(j) = Block::Sand;
			}
		}
		// Mountains
		if (-0.5f <= biomNoise and biomNoise < 0.0f) {
			float noise = stb_perlin_fbm_noise3(
				j*mountainsFreq, -j*mountainsFreq,
				-j*mountainsFreq, 2.0, 0.5, 6);
			int i = filter(noise*32 + TerrainSizeI/1.5f);
			grid.at(i).at(j) = Block::MudWithSnow;
			for (int ti = 0; ti != i and ti < TerrainSizeI; ++ti) {
				grid.at(ti).at(j) = Block::Mud;
			}
		}
		// Forest
		if (0.0f <= biomNoise and biomNoise < 0.5f) {
			float noise = 2*stb_perlin_fbm_noise3(
				j*forestFreq, j*forestFreq, -j*forestFreq, 2.0, 0.5, 6);
			int i = filter(noise*16 + TerrainSizeI/1.5f);
			grid.at(i).at(j) = Block::DirtWithGrass;
			for (int ti = 0; ti != i and ti < TerrainSizeI; ++ti) {
				grid.at(ti).at(j) = Block::Dirt;
			}
		}
		// Field
		if (0.5f <= biomNoise and biomNoise <= 1.f) {
			float noise = stb_perlin_fbm_noise3(
				j*fieldFreq, j*fieldFreq, -j*fieldFreq, 2.0, 0.5, 6);
			int i = filter(noise*8 + TerrainSizeI/1.5f);
			grid.at(i).at(j) = Block::DirtWithGrass;
			for (int ti = 0; ti != i and ti < TerrainSizeI; ++ti) {
				grid.at(ti).at(j) = Block::Dirt;
			}
		}
	}

	// Stone.
	for (int j = 0; j != TerrainSizeJ; ++j) {
		float noise = stb_perlin_turbulence_noise3(
			j*stoneFreq, j*stoneFreq-20, 100+j*stoneFreq, 2.0, 0.5, 6);
		int i = filter(noise*8 + TerrainSizeI/1.7f);
		for (int ti = 0; ti < TerrainSizeI and ti < i; ++ti) {
			grid.at(ti).at(j) = Block::Stone;
		}
	}

	// Underground, caves.
	for (int i = 0; i != TerrainSizeI; ++i) {
		for (int j = 0; j != TerrainSizeJ; ++j) {
			if (isCollidableBlock(grid.at(i).at(j))) {
				float noise = stb_perlin_fbm_noise3(
					j*caveFreq, i*caveFreq, sqrt(i+j), 1.0, 2., 6)/6;
				noise += 8.f*(i*i)/(TerrainSizeI*TerrainSizeI);
				if (noise < 0) {
					grid.at(i).at(j) = Block::None;
				}
			}
		}
	}

	// Gold, diamond.
	for (int i = 0; i != TerrainSizeI; ++i) {
		for (int j = 0; j != TerrainSizeJ; ++j) {
			if (grid.at(i).at(j) == Block::Stone) {
				constexpr float freq = 0.16;
				float noise = stb_perlin_noise3(
					-j*freq, i*freq, -sqrt(i+j), 0, 0, 0);
				if (noise < -0.55) {
					grid.at(i).at(j) = Block::Gold;
				} else if (noise > 0.6) {
					grid.at(i).at(j) = Block::Diamond;
				}
			}
		}
	}
}

void Terrain::draw(sf::Time dt) {
	sf::RectangleShape bg(sf::Vector2f(10000, 5000));
	bg.setFillColor(sf::Color(0, 0, 0,
							  127*(1.f-context()->weather->day()) + 127));
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	context()->renderTarget->draw(bg);
	context()->renderTarget->setView(view);
	
	for (auto& chunk : mChunks) {
		if (distance(context()->player->position(),
					 sf::Vector2f(chunk.y*ChunkSizeJ*32,
								  -chunk.x*ChunkSizeI*32)) < 3000) {
			chunk.draw(dt);
		}
	}
	sf::RenderStates states;
	states.texture = context()->textures->resource(TextureID::Terrain);
	context()->renderTarget->draw(mDarknessVertices, states);
	highlightBlockUnderCursor();
}

void Terrain::highlightBlockUnderCursor() {
	auto v = positionOfBlockUnderCursor();
	sf::RectangleShape rect(sf::Vector2f(TILE, TILE));
	rect.setFillColor(sf::Color(0, 128, 128, 64));
	rect.setPosition(static_cast<sf::Vector2f>(v));
	context()->renderTarget->draw(rect);
}

sf::Vector2i Terrain::positionOfBlockUnderCursor() {
	sf::Vector2i v(sf::Mouse::getPosition(*context()->window));
	v += static_cast<sf::Vector2i>(context()->player->position());
	v -= static_cast<sf::Vector2i>(context()->window->getSize()/2u);
	v.x = v.x/TILE*TILE;
	v.y = (v.y-TILE)/TILE*TILE;
	return v;
}

void Terrain::generate() {
	generateTerrain(*this);
	recalculateVertices();
}

void Terrain::createBlock() {
	Pickup* pickup = context()->player->inventory()->activePickup();
	if (not pickup or pickup->ID() != PickupID::Block) {
		return;
	}
	context()->player->inventory()->takePickup(*pickup);
	Block block = static_cast<Block>(pickup->genericEnumValue());
	auto blockPos = positionOfBlockUnderCursor()/TILE;
	at(-blockPos.y).at(blockPos.x) = block;

	recalculateVerticesForBlock(-blockPos.y, blockPos.x);
	recalculateDarkness();
	recalculateDarknessVertices();

	context()->soundPlayer->play(SoundID::CreateBlock);
}

void Terrain::destroyBlock() {
	auto blockPos = positionOfBlockUnderCursor()/TILE;
	auto block = at(-blockPos.y).at(blockPos.x);
	at(-blockPos.y).at(blockPos.x) = Block::None;

	recalculateVerticesForBlock(-blockPos.y, blockPos.x);
	recalculateDarkness();
	recalculateDarknessVertices();

	if (block == Block::None) {
		return;
	}

	auto pickup = std::make_unique<Pickup>(
		context(),
		PickupID::Block,
		static_cast<int>(block),
		static_cast<sf::Vector2f>(blockPos)*float(TILE));
	
	pickup->setCallback([=]() {
		context()->player->inventory()->add(
			Pickup(
				context(),
				PickupID::Block,
				static_cast<int>(block),
				static_cast<sf::Vector2f>(blockPos)*float(TILE)));
	});

	context()->gameState->bodies()->push_back(std::move(pickup));

	context()->soundPlayer->play(SoundID::DestroyBlock);
}

Grid<signed char>& Terrain::darkness() {
	return mDarkness;
}

void Terrain::recalculateVertices() {
	for (int i = 0; i != TerrainChunksI; ++i) {
		for (int j = 0; j != TerrainChunksJ; ++j) {
			recalculateVerticesForChunk(i, j);
		}
	}
	recalculateDarkness();
	recalculateDarknessVertices();
}

void Terrain::recalculateVerticesForChunk(int chunkI, int chunkJ) {
	for (auto& chunk : mChunks) {
		if (chunk == sf::Vector2i(chunkI, chunkJ)) {
			chunk.recalculateVertices();
		}
	}
}

void Terrain::recalculateVerticesForBlock(int blockI, int blockJ) {
	recalculateVerticesForChunk(blockI/ChunkSizeI,
								blockJ/ChunkSizeJ);
}

void Terrain::recalculateDarkness() {
	int playerBlockJ = context()->player->position().x/32;
	constexpr int dj = 40;
	for (int j = playerBlockJ-dj; j < playerBlockJ+dj; ++j) {
		for (int i = 0; i < TerrainSizeI; ++i) {
			mDarkness.at(i).at(j) = 4;
		}
	}
	for (int i = 0; i != TerrainSizeI; ++i) {
		for (int j = 0; j != TerrainSizeJ; ++j) {
			if (isBrightBlock(context()->terrain->at(i).at(j))) {
				recalculateDarkness(i, j, 0.f);
			}
		}
	}
	for (int j = playerBlockJ-dj; j < playerBlockJ+dj; ++j) {
		for (int i = TerrainSizeI-1; i >= 0; --i) {
			if (context()->terrain->at(i).at(j) == Block::None) {
				recalculateDarkness(i, j,
									1.f-context()->weather->day());
			} else {
				break;
			}
		}
	}
}

void Terrain::recalculateDarkness(int i, int j, float k) {
	constexpr int d = 4;
	auto& darkness = mDarkness;

	for (int ti = i-d; ti <= i+d; ++ti) {
		for (int tj = j-d; tj <= j+d; ++tj) {
			if (ti < 0 or ti >= TerrainSizeI
				or tj < 0 or tj >= TerrainSizeJ) {
				continue;
			}
			darkness.at(ti).at(tj) = 
				std::max(
					0,
					std::min(
						int(std::round((std::max(
							int(abs(i-ti)),
							int(abs(j-tj))))/(1.f-k))) - 1,
						int(darkness.at(ti).at(tj)) ));
			darkness.at(ti).at(tj) = std::min(int(darkness.at(ti).at(tj)), 4);
		}
	}	
}

void Terrain::recalculateDarknessVertices() {
	mDarknessVertices.clear();
	mDarknessVertices.setPrimitiveType(sf::Triangles);

	int playerBlockJ = context()->player->position().x/32;
	constexpr int dj = 40;
	for (int j = playerBlockJ-dj; j < playerBlockJ+dj; ++j) {
		for (int i = 0; i < TerrainSizeI; ++i) {
			auto appendBlock = [&](sf::Vector2f texCoords,
								   sf::VertexArray& vertices) {
				sf::Vertex vertex;
				vertex.position = sf::Vector2f((j)*32,
											   -(i)*32);
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

			auto& darkness = context()->terrain->darkness();

			int blockI = i;
			int blockJ = j;
			if (blockI < 0 or blockI >= TerrainSizeI
				or blockJ < 0 or blockJ >= TerrainSizeJ) {
				continue;
			}

			assert(darkness.at(blockI).at(blockJ) >= 0
				   and darkness.at(blockI).at(blockJ) < 5);
			
			if (darkness.at(blockI).at(blockJ)) {
				appendBlock(
					sf::Vector2f(
						0, 32*(8+darkness.at(blockI).at(blockJ))),
					mDarknessVertices);
			}
		}
	}
}
