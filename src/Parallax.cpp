#include <Parallax.hpp>
#include <Player.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>


ParallaxLayer::ParallaxLayer(Context* cntx)
	: AppElement(cntx)
	, mVelocityFactor(0.f, 0.f)
	, mVelocity(-4, 0) {
}

void ParallaxLayer::handleSignal(const Signal& signal) {
}

void ParallaxLayer::handleEvent(const sf::Event& event) {
}

void ParallaxLayer::update(sf::Time dt) {
	if (context() and context()->player) {
		auto velocity = context()->player->previousPosition()
			- context()->player->position();
		velocity.x *= mVelocityFactor.x;
		velocity.y *= mVelocityFactor.y;
		move(velocity * dt.asSeconds());
	}
	else {
		auto velocity = mVelocity;
		velocity.x *= mVelocityFactor.x;
		velocity.y *= mVelocityFactor.y;
		move(velocity * dt.asSeconds());
	}
}

void ParallaxLayer::setVelocity(sf::Vector2f vel) {
	mVelocity = vel;
}

void ParallaxLayer::draw(sf::Time dt) {
	auto oldPosition = getPosition();
	for (int i = -10; i < 10; ++i) {
		float x = oldPosition.x + getGlobalBounds().width*i;
		setPosition(x, oldPosition.y);
		context()->renderTarget->draw(*this);
	}
	setPosition(oldPosition);
}

void ParallaxLayer::setVelocityFactor(sf::Vector2f velocityFactor) {
	mVelocityFactor = velocityFactor;
}

Parallax::Parallax(Context* cntx)
	: AppElement(cntx) {
}

void Parallax::handleSignal(const Signal& signal) {
}

void Parallax::handleEvent(const sf::Event& event) {
}

void Parallax::update(sf::Time dt) {
	for (auto& layer : *this) {
		layer.update(dt);
	}
}

void Parallax::draw(sf::Time dt) {
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	for (auto& layer : *this) {
		layer.draw(dt);
	}
	context()->renderTarget->setView(view);
}
