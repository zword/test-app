#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *getUi() const { return ui; }
    void setImage(const QImage &im);
    void setTimeout(int t);
    void setState(bool st);

    static QImage blue_image;
    static QImage green_image;
    static QImage red_image;
    static QImage grey_image;
    static QImage led_on_image;

public slots:
    void update();
    void closing();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int timeout;
    QImage image;
    bool state;
};

#endif // MAINWINDOW_H
