#pragma once

#include <SignalID.hpp>
#include <SignalData.hpp>


class Signal {
public:
	SignalID ID = SignalID::None;  // Signal type. 
	SignalData data = SignalData::None;  // Some signal property.
	int extra = -1;  // Extra field.
};
