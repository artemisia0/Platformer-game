#pragma once

#include <Pickup.hpp>
#include <Point.hpp>

#include <vector>
#include <memory>


class Slot : public AppElement,
			 public Point,
			 public std::vector<Pickup> {
public:
	Slot(Context*);

	APP_ELEMENT;

	void setActive(bool);


private:
	bool mIsActive;
};
