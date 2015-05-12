#include <cstdlib>
#include <iostream>

#include "ledDevice.hpp"

LedDevice *LedDevice::_instance = nullptr;

LedDevice::LedDevice()
{
	color = "green";
	state = "off";
	rate = "0";
}

LedDevice *
LedDevice::instance()
{
	if ( _instance == 0 )
		_instance = new LedDevice;

	return _instance;
}

bool
LedDevice::cmdSimulation()
{
	int r = rand() % 3;

	if ( !r )
		return false;

	return true;
}

bool
LedDevice::setState(const std::string &_state)
{
	state = _state;
	return true;
}

bool
LedDevice::setColor(const std::string &_color)
{
	if ( state.compare("off") == 0 )
		return false;


	if ( !cmdSimulation() )
		return false;

	color = _color;
	return true;
}

bool
LedDevice::setRate(const std::string &_rate)
{
	if ( state.compare("off") == 0 )
		return false;

	if ( !cmdSimulation() )
		return false;

	rate = _rate;
	return true;
}

