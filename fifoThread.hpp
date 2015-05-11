#ifndef __FIFOTHREAD_H__
#define __FIFOTHREAD_H__

#include <csignal>

#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>

using namespace std;

class FifoThread
{
public:
	FifoThread(const string &f);
	~FifoThread();

	string getMsg();
	void sendReply(const string &r) const;

	static const string exit_msg;
	static const string err_msg;

	void sigIntFunc(int, siginfo_t*, void*);

private:
	void run();
	string fifo;
	string fifo_reply;
	unique_ptr<thread> thr;

	queue<string> msg;
	condition_variable queue_check;
	mutex msg_queue_mutex;
};

#endif
