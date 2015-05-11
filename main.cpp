#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>

#include "sigHandler.hpp"
#include "fifoThread.hpp"
#include "ledCmd.hpp"

using namespace std;

//TODO: сделать один экземпляр приложения: http://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c-or-c

string g_rcv_fifo_name("/tmp/rcv_fifo");

queue<string> g_commands;
condition_variable g_queuecheck;
mutex g_cmdQueueMutex;

std::thread::id main_thread_id = std::this_thread::get_id();
string g_fifofile;

string g_finish_msg("STOP_SERVER");

void SigIntFunc(int , siginfo_t*, void*)
{
	cout << "hi" << endl;
	int pipe_fd;

	if ( (pipe_fd = open(g_fifofile.data(), O_WRONLY)) == -1 ) {
		cout<<"open("<<g_fifofile.data()<<") failed: "<<strerror(errno)<<endl;
		return;
	}

	if ( write(pipe_fd, g_finish_msg.data(), g_finish_msg.length()) == -1 ) {
		cout<<"write(FINISH) failed: "<<strerror(errno)<<endl;
		close(pipe_fd);
	}

//	remove(g_fifofile.data());
}

int
main(int argc, char *argv[])
{
	FifoThread fifo_thr(argv[1]);

	SigHandler s_hdlr;

	//auto f = std::bind(&FifoThread::sigIntFunc, fifo_thr);
	//s_hdlr.setSigIntHandler(std::bind(&FifoThread::sigIntFunc, fifo_thr, _1));

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

		sleep(5);

		fifo_thr.sendReply(reply);
	}

	return 0;
}

