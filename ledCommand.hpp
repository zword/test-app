#ifndef __LEDCMD_H__
#define __LEDCMD_H__

class LedCmd;

class LedCmdFactory {
public:
//	virtual ~LedCmdFactory();
	virtual LedCmd *create(const std::string cmd);

private:
};

class LedCmd {
public:
	//virtual ~LedCmd();
	static LedCmd *instance();

	virtual std::string get() = 0;
	virtual std::string set() = 0;
	virtual std::string doCmd() = 0;

	void setup(const std::string &cmd);
private:
	static LedCmd *_instance;

};

#endif