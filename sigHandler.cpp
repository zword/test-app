#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <cstring>
#include "sigHandler.hpp"

#if 0
SigHandler::SigHandler(SingalIntFunc func)
{
}
 
SigHandler::~SigHandler()
{
}
#endif

void SigHandler::sigintFunc(int signo, siginfo_t *info, void *context)
{
	//SignalHandlerFunc(signo, info, context);
}

/**
* Set up the signal handlers for CTRL-C.
*/
void SigHandler::setSigIntHandler(SignalHandlerFunc h)
{
#if 1
	struct sigaction act;
 
	::memset(&act, '\0', sizeof(act));

	//act.sa_sigaction = SigHandler::exitSigHandler;
	//act.sa_sigaction = sigintFunc;
	act.sa_flags = SA_SIGINFO | SA_RESETHAND;

	if ( ::sigaction(SIGINT, &act, NULL) == -1 ) {
		std::cout<<"sigaction() failed: "<<strerror(errno)<<std::endl;
		return;
	}
#else
    if (::signal((int) SIGINT, SigHandler::exitSigHandler) == SIG_ERR)
    {
        //throw SignalException("!!!!! Error setting up signal handlers !!!!!");
    }
#endif
}
