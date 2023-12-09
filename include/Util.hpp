#pragma once

#include <cmath>

#include <Context.hpp>


class Parallax;

template <typename T>
void centerOrigin(T* object) {
	auto rect = object->getLocalBounds();
	object->setOrigin(rect.left + rect.width/2,
					  rect.top  + rect.height/2);
}

template <typename A, typename B>
float distance(const A& a, const B& b) {
	return sqrt(pow(a.x-b.x, 2.0) + pow(a.y-b.y, 2.0));
}

std::unique_ptr<Parallax> defaultParallax(Context*);

