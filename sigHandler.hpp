#ifndef __SIGHANDLER_H__
#define __SIGHANDLER_H__

#include <csignal>

class SigHandler
{
public:
//	SigHandler();
//	~SigHandler();

//	typedef void (*SingalIntFunc)(int _ignored, siginfo_t*, void*);

	typedef std::function<void (int _ignored, siginfo_t*, void*)> SignalHandlerFunc;

	void setSigIntHandler(SignalHandlerFunc);

private:
	void sigintFunc(int, siginfo_t*, void*);
};

#endif