#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <thread>
#include <iostream>

#include "fifoThread.hpp"
#include "ledCmd.hpp"

using namespace std;

QImage MainWindow::blue_image = QImage("blue.png");
QImage MainWindow::green_image = QImage("green.png");
QImage MainWindow::red_image = QImage("red.png");
QImage MainWindow::grey_image = QImage("grey.png");

QImage MainWindow::led_on_image = blue_image;

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
}

void main2()
{
    std::cout <<"Hi"<<std::endl;

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

        // Client work in parallel testing
        //sleep(5);

        fifo_thr.sendReply(reply);
    }

    QApplication::quit();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timeout = 0;
    state = false;

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    image = led_on_image;

    //ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->clear();
    statusBar()->showMessage("off");

    std::thread thr(main2);
    thr.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::update()
{
    static int i = 0;

    if ( i++ % 2 ) {
        ui->label->setPixmap(QPixmap::fromImage(image));
        timer->start(timeout);
    } else {
        ui->label->setPixmap(QPixmap::fromImage(grey_image));
        timer->start(100);
    }
}

void
MainWindow::setTimeout(int t)
{
    if ( t == 0 ) {
            timer->stop();
            timeout = 0;
            ui->label->setPixmap(QPixmap::fromImage(image));
            return;
    }

    if ( timeout == 0 )
        timer->start(t);

    timeout = t;
}

void
MainWindow::setImage(const QImage &im)
{
    image = im;

    if ( timeout == 0 )
        ui->label->setPixmap(QPixmap::fromImage(image));
}

void
MainWindow::setState(bool _state)
{
    if ( (state == false) && (_state == true) ) {
        state = true;
        ui->label->setPixmap(QPixmap::fromImage(image));
        statusBar()->clearMessage();
        statusBar()->showMessage("on");
    }

    if ( (state == true) && (_state == false) ) {
        state = _state;
        ui->label->setPixmap(QPixmap::fromImage(grey_image));
        image = led_on_image;
        timer->stop();
        timeout = 0;
        statusBar()->clearMessage();
        statusBar()->showMessage("off");
    }
}

void
MainWindow::closing()
{
    kill(getpid(), SIGINT);
}
