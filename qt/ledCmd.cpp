#include <iostream>

#include "ledCmd.hpp"
#include "ledDevice.hpp"

const std::string LedCmd::failed_msg = "FAILED";
const std::string LedCmd::success_msg = "OK ";

/**
 *	Команда в формате "_action_-led-_cmd_ _arg_"
 * 	_action_: get/set
 */
std::unique_ptr<LedCmd>
LedCmdFactory::create(const std::string &cmd)
{
	std::unique_ptr<LedCmd> led_cmd;
	std::string cmd_type, arg("");

	// Если есть пробел, считаем, что команда set
	size_t space_index = cmd.find(' ');
	bool cmd_setter = ( space_index == std::string::npos ) ? false : true;

	int cmd_index = cmd.find_last_of('-') + 1;

	if ( cmd_setter ) {
		cmd_type = cmd.substr(cmd_index, space_index - cmd_index);
		arg = cmd.substr(space_index + 1);
	} else {
		cmd_type = cmd.substr(cmd_index);
	}

	if ( !cmd_type.compare("state") ) {
		led_cmd = std::unique_ptr<LedStateCmd>(new LedStateCmd);
	} else if ( !cmd_type.compare("color") ) {
		led_cmd = std::unique_ptr<LedColorCmd>(new LedColorCmd);
	} else if ( !cmd_type.compare("rate") ) {
		led_cmd = std::unique_ptr<LedRateCmd>(new LedRateCmd);
	} else
		return nullptr;

	led_cmd->setup(cmd_setter, arg);

	return led_cmd;
}

void
LedCmd::setup(bool type, const std::string &_arg)
{
	current_cmd_type = type;
	arg = _arg;

//std::cout<<"setup ["<<current_cmd_type<<"]"<<std::endl;

}

std::string
LedCmd::doCmd()
{
//std::cout<<"doCmd ["<<current_cmd_type<<"]"<<std::endl;

	if ( current_cmd_type == true )
		return setVal(arg);
	else
		return getVal();
}

std::string
LedCmd::setVal(const std::string &arg)
{
	if ( check_arg(arg) == false ) {
		//std::cout<<"Check failed"<<std::endl;
		return failed_msg;
	}

	if ( set(arg) == false )
		return failed_msg;

	return success_msg;
}

std::string
LedCmd::getVal()
{
	return get();
}

std::string
LedStateCmd::get()
{
	return success_msg + LedDevice::instance()->getState();
}

bool
LedStateCmd::set(const std::string &arg)
{
    return LedDevice::instance()->setState(arg);
}

bool
LedStateCmd::check_arg(const std::string &arg) const
{
	if ( !arg.compare("off") )
		return true;

	if ( !arg.compare("on") )
		return true;

	return false;
}

std::string
LedColorCmd::get()
{
	return success_msg + LedDevice::instance()->getColor();
}

bool
LedColorCmd::set(const std::string &arg)
{
	return LedDevice::instance()->setColor(arg);
}

bool
LedColorCmd::check_arg(const std::string &arg) const
{
	if ( !arg.compare("blue") )
		return true;

	if ( !arg.compare("green") )
		return true;

	if ( !arg.compare("red") )
		return true;

	return false;
}

std::string
LedRateCmd::get()
{
	return success_msg + LedDevice::instance()->getRate();
}

bool
LedRateCmd::set(const std::string &arg)
{
	return LedDevice::instance()->setRate(arg);
}

bool
LedRateCmd::check_arg(const std::string &arg) const
{
	int rate = std::stoi(arg);

	if ( rate < 0 || rate > 5 )
		return false;

	return true;
}
