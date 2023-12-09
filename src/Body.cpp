#include <Body.hpp>
#include <GameState.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


Body::Body(Context* cntx, BodyType type)
	: AppElement(cntx)
	, mVelocityQueue{}
	, mPosition(40000.f, -2000.f)
	, mSize(64.f, 64.f)
	, mMarkedForRemoval(false)
	, mHaveJustCollided(false)
	, mType(type) {
}

BodyType Body::type() {
	return mType;
}

void Body::markForRemoval() {
	mMarkedForRemoval = true;
}

bool Body::markedForRemoval() {
	return mMarkedForRemoval;
}

bool Body::haveJustCollided() {
	return mHaveJustCollided;
}

void Body::handleSignal(const Signal& signal) {
}

void Body::handleEvent(const sf::Event& event) {
}

void Body::update(sf::Time dt) {
	if (distance(context()->player->position(), position()) > 2000) {
		markForRemoval();
	}
	
	mHaveJustCollided = false;
	
	for (auto& v : mVelocityQueue) {
		v *= 0.96f;
	}
	while (mVelocityQueue.size()
		   and std::abs(mVelocityQueue.front().x) < 1.f
		   and std::abs(mVelocityQueue.front().y) < 1.f) {
		mVelocityQueue.pop_front();
	}
	
	moveOnXAxis(dt);
	moveOnYAxis(dt);
}

sf::Vector2f Body::velocity() {
	sf::Vector2f sum(0.f, 0.f);
	for (const auto& vel : mVelocityQueue) {
		sum += vel;
	}
	return sum;
}

void Body::moveOnXAxis(sf::Time dt) {
	float deltaX = velocity().x*dt.asSeconds();
	mPosition.x += deltaX;
	if (collides()) {
		mHaveJustCollided = true;
		mPosition.x -= deltaX;
	}	
}

void Body::moveOnYAxis(sf::Time dt) {
	float deltaY = velocity().y*dt.asSeconds();
	mPosition.y += deltaY;
	if (collides()) {
		mHaveJustCollided = true;
		mPosition.y -= deltaY;
	}

	if (not standingOnGround()) {
		setVelocity(sf::Vector2f(0.f, 10.f));
	}
}

bool Body::standingOnGround() {
	mPosition.y += 10.f;
	bool result = false;
	if (collides()) {
		result = true;
	}
	mPosition.y -= 10.f;
	
	return result;
}

void Body::draw(sf::Time dt) {
	/*
	  sf::RectangleShape rect(mSize);
	  rect.setPosition(mPosition);
	  rect.setFillColor(sf::Color(0, 255, 0, 128));
	  context()->renderTarget->draw(rect);
	*/
}

sf::Vector2f Body::position() {
	return mPosition;
}

sf::Vector2f Body::size() {
	return mSize;
}

sf::FloatRect Body::bounds() {
	return sf::FloatRect(mPosition, mSize);
}

void Body::setPosition(sf::Vector2f v) {
	mPosition = v;
}

void Body::setSize(sf::Vector2f v) {
	mSize = v;
}

void Body::setVelocity(sf::Vector2f v) {
	mVelocityQueue.push_back(v);
}

bool Body::collides() {
	return context()->gameState->collides(*this);
}

bool Body::collides(Body& body) {
	return body.bounds().intersects(bounds());
}

bool Body::equals(Body& body) {
	return body.mVelocityQueue == mVelocityQueue
		and body.mPosition == mPosition
		and body.mSize == mSize;
}

bool Body::almostCollides(Body& body) {
	auto initialPosition = position();
	constexpr float delta = 4.f;
	bool flag = false;

	setPosition(sf::Vector2f(position().x+delta, position().y));
	flag |= collides(body);
	setPosition(initialPosition);

	setPosition(sf::Vector2f(position().x-delta, position().y));
	flag |= collides(body);
	setPosition(initialPosition);

	setPosition(sf::Vector2f(position().x, position().y-delta));
	flag |= collides(body);
	setPosition(initialPosition);

	setPosition(sf::Vector2f(position().x, position().y+delta));
	flag |= collides(body);
	setPosition(initialPosition);

	return flag;
}

Body* Body::bodyThatIAlmostCollideWith(int ID) {
	for (int i = 0, lim = context()->gameState->bodies()->size();
		 i != lim; ++i) {
		Character* character = dynamic_cast<Character*>
			(context()->gameState->bodies()->at(i).get());
		if (not character) {
			continue;
		}
		if (character->type() != BodyType::NPC
			and character->type() != BodyType::Player) {
			continue;
		}
		if (character->almostCollides(*this)
			and character->ID() != ID) {
			return static_cast<Body*>(character);
		}
	}
	return nullptr;
}
