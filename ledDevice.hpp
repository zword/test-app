#ifndef __LEDDEVICE_H__
#define __LEDDEVICE_H__

#include <string>

/**
 * Эмуляция LED устройства
 */
class LedDevice {
public:
	LedDevice();
	static LedDevice *instance();

	// Полагаем, что get-функции всегда успешно завершаются
	std::string getState() const { return state; }
	std::string getColor() const { return color; }
	std::string getRate() const { return rate; }

	bool setState(const std::string &);
	bool setColor(const std::string &);
	bool setRate(const std::string &);

private:
	static LedDevice * _instance;
	std::string color;
	std::string state;
	std::string rate;

	// Эмуляция ошибки при установки значения состояния устройства
	bool cmdSimulation();
};

#endif