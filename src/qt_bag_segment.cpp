#include "qtComponent/qtDesign/mainwindow.h"
#include <QApplication>
#include "bagsegment.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    bagSegment rc(argc,argv,"qt_bag_segment");
    MainWindow w(&rc);
    w.show();
    
    return a.exec();
}
