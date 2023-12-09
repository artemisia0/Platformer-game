#pragma once

#include <Slot.hpp>


class Inventory : public AppElement, public std::vector<Slot> {
public:
	Inventory(Context*);
	virtual ~Inventory() = default;

	APP_ELEMENT;

	void add(Pickup);
	bool hasFreeSpaceFor(PickupID, int);
	Pickup* activePickup();
	void takePickup(Pickup);
	Slot* slotWithPickup(Pickup);
	int count(Pickup);


private:
	int mActiveSlotIndex;
	float mMouseWheelScroll;
};
