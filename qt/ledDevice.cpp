#include <cstdlib>
#include <iostream>

#include "ledDevice.hpp"
#include "mainwindow.h"

extern MainWindow *w;

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

    state.compare("on") ? w->setState(false) : w->setState(true);

	return true;
}

bool
LedDevice::setColor(const std::string &_color)
{
    if ( state.compare("off") == 0 )
        return false;

    if ( cmdSimulation() )
		return false;

	color = _color;

    if ( color.compare("blue") == 0 )
        w->setImage(MainWindow::blue_image);
    if ( color.compare("red") == 0 )
        w->setImage(MainWindow::red_image);
    if ( color.compare("green") == 0 )
        w->setImage(MainWindow::green_image);

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

    std::stoi(rate) ? w->setTimeout(1000/std::stoi(rate)) : w->setTimeout(0);

	return true;
}
