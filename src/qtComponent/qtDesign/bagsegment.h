#ifndef BAGSEGMENT_H
#define BAGSEGMENT_H


#include <QObject>	// 1.包含QObject
#include <boost/foreach.hpp>
#include <ros/ros.h>
#include <ros/time.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud2.h>
#include <vector>
#include <string>
#include <lcm2ros/Gps.h>
#define foreach_boost BOOST_FOREACH

using namespace std;

class bagSegment : public QObject		// 3.继承于QObject类
{
    Q_OBJECT		// 4.宏
    public:
        bagSegment(int,char**,const std::string);	// 5.构造函数，用来传入初始化节点的参数 ( int argc , char **argv , std::string node_name )
        double startTime,endTime;   //the start and end time point to cut the
        vector<string> fileNameVec;   //the vector of input rosbag fullpath
        string outputPath;      //the directory path of output bag

        rosbag::Bag *bag_in;
        rosbag::Bag *bag_out;
        rosbag::View *full_view;

        bool exit_loop;
    signals:
        void rosDowm();
        void progress(int);
        void statusSig(QString);
    public slots:
        void doActive();	// 3.执行spin循环的子线程执行函数
        void rosShutDown_slot();	// 4.收到主界面的关闭信号时，中止ros
};

#endif // BAGSEGMENT_H
