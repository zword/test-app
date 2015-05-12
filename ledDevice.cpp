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
	int r = rand() % 2;

std::cout<<"r = "<<r<<std::endl;

	if ( !r )
		return false;

	return true;
}

bool
LedDevice::setState(const std::string &_state)
{
	if ( cmdSimulation() )
		return false;

	state = _state;
	return true;
}

bool
LedDevice::setColor(const std::string &_color)
{
	if ( cmdSimulation() )
		return false;

	color = _color;
	return true;
}

bool
LedDevice::setRate(const std::string &_rate)
{
	if ( !cmdSimulation() )
		return false;

	rate = _rate;
	return true;
}
