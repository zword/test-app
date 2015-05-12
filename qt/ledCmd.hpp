#ifndef __LEDCMD_H__
#define __LEDCMD_H__

#include <string>
#include <memory>

#include "mainwindow.h"

class LedCmd;

/**
 * Фабрика по производству объектов-команд
 * @cmd - идентификатор команды. Команда имеет формат get/set-led-cmd
 * Для добавления новой команды необходимо добавить ветку if со сравнением по имени команды cmd. См. текст ф-ии
 */
class LedCmdFactory {
public:
	std::unique_ptr<LedCmd> create(const std::string &cmd);

private:
};

/**
 * Базовый класс команд
 * Наследники должны переопределить ф-ии get, set и check_arg
 */
class LedCmd {
public:
    virtual ~LedCmd() {}

    std::string doCmd();
	std::string getVal();
	std::string setVal(const std::string &arg);

	void setup(bool type, const std::string &cmd);

	static const std::string failed_msg;
	static const std::string success_msg;

private:
    // Возвращает состояние параметра LED
    virtual std::string get() = 0;
    // Проверяет устанавливаемое значение на корректность
    virtual bool check_arg(const std::string &arg) const = 0;
    // Установка состояния LED
    virtual bool set(const std::string &arg) = 0;

	bool current_cmd_type;
	std::string arg;
};

/**
 * Команда вкл/выкл устройства
 */
class LedStateCmd: public LedCmd {
private:
	virtual std::string get();
	virtual bool check_arg(const std::string &arg) const;
	virtual bool set(const std::string &arg);

};

/**
 * Команда чтения/установки светового индикатора устройства
 */
class LedColorCmd: public LedCmd {
public:

private:
	virtual std::string get();
	virtual bool check_arg(const std::string &arg) const;
	virtual bool set(const std::string &arg);

};

/**
 * Команда чтения/установки частоты мерцания светового индикатора устройства
 */
class LedRateCmd: public LedCmd {
private:
	virtual std::string get();
	virtual bool check_arg(const std::string &arg) const;
	virtual bool set(const std::string &arg);

};

#endif
