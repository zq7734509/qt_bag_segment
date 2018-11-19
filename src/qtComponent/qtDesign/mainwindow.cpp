#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMimeData>
//#include <QFile>
#include <QUrl>


MainWindow::MainWindow(bagSegment *p,QWidget *parent) :	// 1.重载构造函数，传入新建类对象的指针，注意第二个缺省参数不要漏掉
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    p_bs(p)	// 2.在构造的过程中对成员变量进行赋值
{
    ui->setupUi(this);
    p_thread = new QThread(this);		// 3.定义新线程，当前界面为父界面
    connect(p_thread,SIGNAL(started()),p_bs,SLOT(doActive()));		// 5.线程开始时，执行子线程中需要执行的函数
    ui->textBrowser->insertPlainText("Please drag the rosbag here ...\n");
    this->setAcceptDrops(true);
    ui->progressBar->setValue(0);
    connect(ui->pushButton_out,SIGNAL(clicked(bool)),this,SLOT(getDirectoryPath()));
    connect(ui->lineEdit_start,SIGNAL(textChanged(QString)),this,SLOT(getStartTime(QString)));
    connect(ui->lineEdit_end,SIGNAL(textChanged(QString)),this,SLOT(getEndTime(QString)));
    connect(ui->pushButton_start,SIGNAL(clicked(bool)),p_thread,SLOT(start()));
    connect(p_bs,SIGNAL(rosDowm()),this,SLOT(close()));
    connect(p_bs,SIGNAL(progress(int)),ui->progressBar,SLOT(setValue(int)));
    connect(p_bs,SIGNAL(statusSig(QString)),ui->label_status_2,SLOT(setText(QString)));
    p_bs->moveToThread(p_thread);		// 8.将新建类对象指针移交给线程进行管理
}

MainWindow::~MainWindow()
{
    p_thread->quit();
    p_thread->wait();
    delete ui;

}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)   //get the file path by drag and drop the file in the window
{
    QString name = event->mimeData()->urls().first().toLocalFile();
    p_bs->fileNameVec.push_back(name.toStdString());
    ui->textBrowser->insertPlainText(name+"\n");
}
void MainWindow::getDirectoryPath()     //get the output directory path by click the button
{
    QString file_path = QFileDialog::getExistingDirectory(this,"please select the path to save the output rosbag...","./");
    ui->textBrowser_2->setPlainText(file_path);
    p_bs->outputPath = file_path.toStdString();
}
void MainWindow::getStartTime(QString str)  //get the start time point to cut
{
    p_bs->startTime = str.toDouble();

}
void MainWindow::getEndTime(QString str)    //get the end time point to cut
{
    p_bs->endTime = str.toDouble();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    p_bs->rosShutDown_slot();
    p_bs->exit_loop = true;
}
