#include <Log.hpp>

#include <iostream>
#include <cstdlib>


Log::Log(const std::string& str)
	: mMessage(str) {
}

void Log::fatal() const {
	std::cerr << "FATAL:" << mMessage << std::endl;
	exit(1);
}

void Log::warning() const {
	std::cerr << "WARNING:" << mMessage << std::endl;
}

void Log::info() const {
	std::cout << "INFO:" << mMessage << std::endl;
}
