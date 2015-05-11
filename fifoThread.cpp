#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>

#include "fifoThread.hpp"

const string FifoThread::exit_msg = "STOP_SERVER";
const string FifoThread::err_msg = "GET_MSG_ERROR";

FifoThread::FifoThread(const std::string &f):
	fifo(f)
{
	thr = unique_ptr<thread>(new thread(bind(&FifoThread::run, this)));
}

FifoThread::~FifoThread()
{
	thr->join();

	remove(fifo.c_str());
}

void
FifoThread::run()
{
	cout<<"Run: fifo = ["<<fifo<<"]"<<endl;

#if 0
	ifstream f;

	f.open(pipefile);
	if (!f.is_open()) {
		cout<<"open("<<pipefile<<") failed: "<<strerror(errno)<<endl;
		return;
	}

	//while ( !f.bad() ) {
	while ( f.read(cmd, 100) ) {
		//getline(f, cmd); 
		cout << "line [" << cmd << "]" << endl;
		lock_guard<mutex> lock(g_cmdQueueMutex);
	}

	if ( f.eof() )
		cout << "EOF" << endl;

	f.close();
#else
	int pipe_fd;
	ssize_t read_bytes;
	char cmd[100];

	if ( mkfifo(fifo.c_str(), S_IRUSR | S_IWUSR) == -1 ) {
		cout<<"mkfifo("<<fifo.c_str()<<") failed: "<<strerror(errno)<<endl;

		unique_lock<mutex> lock(msg_queue_mutex);
		msg.push(exit_msg);
		queue_check.notify_one();

		return;
	}

	int reopen = 1;
	while ( 1 ) {
		// Открыть заново для клиентов, когда все предыдущие закончили работу
		if ( reopen ) {
			close(pipe_fd);

cout << "Opening ["<<fifo.c_str()<<"]"<<endl;
			if ( (pipe_fd = open(fifo.c_str(), O_RDONLY)) == -1 ) {
				cout<<"open("<<fifo.c_str()<<") failed: "<<strerror(errno)<<endl;
				return;
			}

			reopen = 0;
		}
cout << "OK. readnig"<<endl;

		if ( (read_bytes = read(pipe_fd, (void *)cmd, 100)) == -1 ) {
			cout<<"read("<<fifo.c_str()<<") failed: "<<strerror(errno)<<endl;
			break;
		}

		// Все клиенты закончили работу
		if ( read_bytes == 0 ) {
			cout<<"All clients finished their jobs"<<endl;
			reopen = 1;
			continue;
		}

		// Убираем '\n'
		cmd[read_bytes - 1] = '\0';
		cout<<"Read cmd from pipe ["<<cmd<<"], "<<read_bytes<<" bytes"<<endl;

		unique_lock<mutex> lock(msg_queue_mutex);
		msg.push(cmd);
		queue_check.notify_one();

		// Завершаем работу
		if ( exit_msg.compare(cmd) == 0 )
			break;
	}

	close(pipe_fd);
	if ( remove(fifo.c_str()) == -1 )
		cout<<"remove("<<fifo.c_str()<<") failed: "<<strerror(errno)<<endl;
#endif
}

/**
 * Формат сообщения: "ИдентификаторКаналаОтвета Команда"
 */
string
FifoThread::getMsg()
{
	unique_lock<mutex> lock(msg_queue_mutex);

	if ( !msg.size() ) {
		cout << "Waiting requests..." << endl;
		queue_check.wait(lock);
	}

	string req = msg.front();

	// Конец работы
	if ( exit_msg.compare(req) == 0 )
		return exit_msg;

	msg.pop();

	//cout << "Request received: " << req << endl;
	cout << "Queue size = " << msg.size() << endl;

	size_t index = req.find_first_of(' ');
	if ( index == string::npos ) {
		cout << "Error: wrong request format: " << req << endl;
		return err_msg;
	}

	fifo_reply = "/tmp/" + req.substr(0, index);
	//cout << "FIFO to answer ["<< fifo_reply << "]" << endl;

	return req.erase(0, index + 1);
}

void
FifoThread::sendReply(const string &reply) const
{
	int fd_reply;
	if ( (fd_reply = open(fifo_reply.c_str(), O_WRONLY)) == -1 ) {
		if ( errno != ENOENT )	// Клиент завершился и удалил фифо
			cout<<"open("<<fifo_reply.c_str()<<") failed: "<<strerror(errno)<<endl;
		return;
	}

	cout << "Sending reply: [" << reply << "]" << endl;
	if ( write(fd_reply, reply.c_str(), reply.size()) == -1 ) {
		cout<<"write("<<fifo_reply.c_str()<<") failed: "<<strerror(errno)<<endl;
		//close(fd_reply);
	}

	// Посылаем eof
	close(fd_reply);
}

void
FifoThread::sigIntFunc(int, siginfo_t*, void*)
{

}
