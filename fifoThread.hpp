#ifndef __FIFOTHREAD_H__
#define __FIFOTHREAD_H__

#include <csignal>

#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>

using namespace std;

/**
 * Класс потока приёма сообщений из канала, а также посылка ответных сообщений.
 * Вся логика теста здесь.
 */
class FifoThread
{
public:
	FifoThread(const string &f);
	~FifoThread();

	string getMsg(); // Выбрать сообщние из очереди
	void sendReply(const string &r) const; // Отправить ответ в канал

	static const string exit_msg;
	static const string err_msg;

private:
	void run();	// Ф-ия потока приёма сообщений
	string fifo;	// Имя фифо-файла приёма (общий для всех клиентов)
	string fifo_reply; // Имя фифо-файла отправки ответа (для каждого клиента свой)
	unique_ptr<thread> thr;

	queue<string> msg; // Очередь сообщений
	// Синхронизация
	condition_variable queue_check;
	mutex msg_queue_mutex;
};

#endif
