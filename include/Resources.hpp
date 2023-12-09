#pragma once

#include <ResourceID.hpp>
#include <Log.hpp>

#include <map>
#include <string>
#include <memory>


namespace sf {
	class Texture;
	class Font;
}

template <typename E, typename T>
class Resources {
public:
	bool load(E, const std::string&);
	T* resource(E);


private:
	std::map<E, std::unique_ptr<T>> mTable;
};


using TextureResources = Resources<TextureID, sf::Texture>;
using FontResources = Resources<FontID, sf::Font>;

template <typename E, typename T>
bool Resources<E, T>::load(E ID, const std::string& filename) {
	T resource;
	if (not resource.loadFromFile(filename)) {
		return false;
	}
	mTable.insert(
		std::make_pair(ID, std::make_unique<T>(std::move(resource))));
	return true;
}

template <typename E, typename T>
T* Resources<E, T>::resource(E ID) {
	auto found = mTable.find(ID);
	if (found == mTable.end()) {
		Log("Failed to find resource, returning nullptr").warning();
		return nullptr;
	}
	return found->second.get();
}
