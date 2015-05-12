#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "fifoThread.hpp"
#include "ledCmd.hpp"

using namespace std;

//TODO: сделать один экземпляр приложения: http://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c-or-c

const string g_fifofile("/tmp/fifo_test");

int
setSigIntHandler(void (*hndlr)(int, siginfo_t *, void *))
{
	struct sigaction act;
 
	::memset(&act, '\0', sizeof(act));

	act.sa_sigaction = hndlr;
	act.sa_flags = SA_SIGINFO | SA_RESETHAND;

	if ( ::sigaction(SIGINT, &act, NULL) == -1 ) {
		std::cout<<"sigaction() failed: "<<strerror(errno)<<std::endl;
		return 1;
	}

	return 0;
}

void sigIntFunc(int, siginfo_t*, void*)
{
	cout << "hi" << endl;
	int pipe_fd;

	if ( (pipe_fd = ::open(g_fifofile.data(), O_WRONLY)) == -1 ) {
		cout<<"open("<<g_fifofile.data()<<") failed: "<<strerror(errno)<<endl;
		return;
	}

	if ( ::write(pipe_fd, std::string(FifoThread::exit_msg).append("\n").c_str(), FifoThread::exit_msg.length() + 1) == -1 ) {
		cout<<"write("<<FifoThread::exit_msg<<") failed: "<<strerror(errno)<<endl;
		return;
	}
#if 0
	if ( ::write(pipe_fd, "\n", 1) == -1 ) {
		cout<<"write("<<FifoThread::exit_msg<<") failed: "<<strerror(errno)<<endl;
		return;
	}
#endif
}

int
main()
{
	FifoThread fifo_thr(g_fifofile);

	setSigIntHandler(sigIntFunc);

	LedCmdFactory factory;

	while ( 1 ) {
		string cmd = fifo_thr.getMsg();

cout << "Request received ["<<cmd<<"]"<<endl;

		if ( !cmd.compare(FifoThread::err_msg) )
			continue;

		if ( !cmd.compare(FifoThread::exit_msg) )
			break;

		unique_ptr<LedCmd> led_cmd = factory.create(cmd);
		if ( led_cmd == nullptr ) {
			fifo_thr.sendReply(LedCmd::failed_msg);
			continue;
		}

		// Обработка запроса "оборудованием"
		string reply = led_cmd->doCmd();

		//sleep(5);

		fifo_thr.sendReply(reply);
	}

	return 0;
}

