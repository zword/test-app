#ifndef __LEDCMD_H__
#define __LEDCMD_H__

#include <string>
#include <memory>

class LedCmd;

class LedCmdFactory {
public:
	std::unique_ptr<LedCmd> create(const std::string &cmd);

private:
};

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
	virtual std::string get() = 0;
	virtual bool check_arg(const std::string &arg) const = 0;
	virtual bool set(const std::string &arg) = 0;

	bool current_cmd_type;
	std::string arg;
};

class LedStateCmd: public LedCmd {
private:
	virtual std::string get();
	virtual bool check_arg(const std::string &arg) const;
	virtual bool set(const std::string &arg);

};

class LedColorCmd: public LedCmd {
public:

private:
	virtual std::string get();
	virtual bool check_arg(const std::string &arg) const;
	virtual bool set(const std::string &arg);

};

class LedRateCmd: public LedCmd {
private:
	virtual std::string get();
	virtual bool check_arg(const std::string &arg) const;
	virtual bool set(const std::string &arg);

};

#endif