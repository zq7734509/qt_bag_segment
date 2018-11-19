#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QString>
#include <string>
#include <vector>
#include <iostream>
#include <QFileDialog>
#include <QThread>
#include <QCloseEvent>
#include "bagsegment.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent = 0);
    MainWindow(bagSegment *p,QWidget *parent = 0);		// 3.重载构造函数，传入新建类对象的指针，注意第二个缺省参数不要漏掉
    bagSegment *p_bs;
    QThread *p_thread;		// 5.声明QThread类对象指针
    ~MainWindow();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);
signals:
public slots:
    void getDirectoryPath();
    void getStartTime(QString);
    void getEndTime(QString);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
