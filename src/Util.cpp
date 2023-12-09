#include <Util.hpp>
#include <Parallax.hpp>

#include <SFML/Graphics/Texture.hpp>


std::unique_ptr<Parallax> defaultParallax(Context* context) {
	auto parallax = std::make_unique<Parallax>(context);

	context->textures->resource(
		TextureID::ParallaxLayer1)->setRepeated(true);
	
	ParallaxLayer layer1(context);
	layer1.setTexture(
		*context->textures->resource(TextureID::ParallaxLayer1));
	ParallaxLayer layer2(context);
	layer2.setTexture(
		*context->textures->resource(TextureID::ParallaxLayer2));
	ParallaxLayer layer3(context);
	layer3.setTexture(
		*context->textures->resource(TextureID::ParallaxLayer3));
	ParallaxLayer layer4(context);
	layer4.setTexture(
		*context->textures->resource(TextureID::ParallaxLayer4));
	ParallaxLayer layer5(context);
	layer5.setTexture(
		*context->textures->resource(TextureID::ParallaxLayer5));

	layer1.setScale(7.0, 7.0);
	layer2.setScale(7.0, 7.0);
	layer3.setScale(7.0, 7.0);
	layer4.setScale(7.0, 7.0);
	layer5.setScale(7.0, 7.0);

	layer1.setVelocityFactor(sf::Vector2f(2.0, 0.0));
	layer2.setVelocityFactor(sf::Vector2f(4.0, 0.0));
	layer3.setVelocityFactor(sf::Vector2f(8.0, 0.0));
	layer4.setVelocityFactor(sf::Vector2f(16.0, 0.0));
	layer5.setVelocityFactor(sf::Vector2f(32.0, 0.0));

	parallax->push_back(std::move(layer1));
	parallax->push_back(std::move(layer2));
	parallax->push_back(std::move(layer3));
	parallax->push_back(std::move(layer4));
	parallax->push_back(std::move(layer5));
	
	return parallax;
}


