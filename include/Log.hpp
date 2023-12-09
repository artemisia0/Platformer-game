#pragma once

#include <string>


class Log {
public:
	Log(const std::string& str=std::string());

	void fatal() const;
	void warning() const;
	void info() const;


private:
	std::string mMessage;
};
