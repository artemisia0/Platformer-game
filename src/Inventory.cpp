#include <Inventory.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <cassert>


Inventory::Inventory(Context* cntx)
	: AppElement(cntx)
	, mActiveSlotIndex(0)
	, mMouseWheelScroll(0.f) {
}

void Inventory::handleSignal(const Signal& signal) {
}

void Inventory::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseWheelScrolled) {
		mMouseWheelScroll -= event.mouseWheelScroll.delta;
	}
	mActiveSlotIndex = int(mMouseWheelScroll+size())%int(size());
}

void Inventory::update(sf::Time dt) {
	for (auto& slot : *this) {
		slot.update(dt);
	}
}

void Inventory::draw(sf::Time dt) {
	auto view = context()->renderTarget->getView();
	context()->renderTarget->setView(
		context()->renderTarget->getDefaultView());
	sf::Vector2f slotPosition(32, 32);
	for (int i = 0; i != int(size()); ++i) {
		auto& slot = at(i);
		slot.setPosition(slotPosition);
		slotPosition.y += 64;
		slot.setActive(false);
		if (i == mActiveSlotIndex) {
			slot.setActive(true);
		}
		slot.draw(dt);
		if (i == int(size())/2 - 1) {
			slotPosition.x = 96;
			slotPosition.y = 32;
		}
	}
	context()->renderTarget->setView(view);
}

void Inventory::add(Pickup pickup) {
	for (auto& slot : *this) {
		if (slot.size() and slot.at(0).equals(pickup)) {
			slot.push_back(pickup);
			return;
		}
	}
	for (auto& slot : *this) {
		if (slot.empty()) {
			slot.push_back(pickup);
			return;
		}
	}
}

bool Inventory::hasFreeSpaceFor(PickupID ID, int val) {
	for (auto& slot : *this) {
		if (slot.size() and slot.at(0).ID() == ID
			and slot.at(0).genericEnumValue() == val) {
			return true;
		}
		if (slot.empty()) {
			return true;
		}
	}
	return false;
}

Pickup* Inventory::activePickup() {
	if (at(mActiveSlotIndex).size()) {
		return &at(mActiveSlotIndex).at(0);
	}
	return nullptr;
}

void Inventory::takePickup(Pickup pickup) {
	for (auto& slot : *this) {
		if (slot.size() and slot.at(0).equals(pickup)) {
			slot.pop_back();
			return;
		}
	}
}

Slot* Inventory::slotWithPickup(Pickup pickup) {
	for (auto& slot : *this) {
		if (slot.size() and slot.at(0).equals(pickup)) {
			return &slot;
		}
	}
	return nullptr;
}

int Inventory::count(Pickup pickup) {
	if (not slotWithPickup(pickup)) {
		return 0;
	}
	return slotWithPickup(pickup)->size();
}
