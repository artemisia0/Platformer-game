#pragma once

#include <AppElement.hpp>

#include <SFML/System/Vector2.h>
#include <SFML/Graphics/VertexArray.hpp>


constexpr int ChunkSizeI = 1<<5;
constexpr int ChunkSizeJ = 1<<5;

class Chunk : public AppElement,
			  public sf::VertexArray,
			  public sf::Vector2i {
public:
	Chunk(Context*, sf::Vector2i chunkPos);
	virtual ~Chunk() = default;

	APP_ELEMENT;

	void recalculateVertices();
};
