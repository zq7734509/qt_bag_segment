#include "bagsegment.h"
#include "iostream"
#include <QThread>
#include <QString>
bagSegment::bagSegment(int argc , char **argv , const std::string name)
{
    ros::init(argc, argv, name);		// 1.初始化ros节点，name为唯一
    ros::start();		// 2.开始ros节点，如果不执行将无法注册节点
    ros::NodeHandle n;		// 3.定义nodehandle
    exit_loop = false;
}
void bagSegment::doActive()		// 5.子线程执行函数实现
{
    bag_out = new rosbag::Bag(outputPath+"/output.bag",rosbag::bagmode::Write);

    for(int i=0; i<(int)fileNameVec.size(); i++)
    {
        emit statusSig("Openning bag ...");
        bag_in = new rosbag::Bag(fileNameVec[i], rosbag::bagmode::Read);

        full_view = new rosbag::View;
        full_view->addQuery(*bag_in);
        //get the ros time of the start and end of the bag
        ros::Time start_time_ros = full_view->getBeginTime();
        ros::Time end_time_ros = full_view->getEndTime();

        //make the period time between the start time and the cut start time
        ros::Duration duration_start_ros(startTime);
        //make the period time between the start time and the cut end time
        ros::Duration duration_end_ros(endTime);

        //get the ros time of the cut start and cut end
        ros::Time t_current_start_ros = start_time_ros + duration_start_ros;
        ros::Time t_current_end_ros = start_time_ros + duration_end_ros;
        if (t_current_end_ros.toSec() > end_time_ros.toSec()) {
            cout << "the end time is out of the bag" << endl;
            t_current_end_ros = end_time_ros;
          }


        foreach_boost (rosbag::MessageInstance const m, *full_view) {
            ros::Time const &time = m.getTime();
            string const &topic = m.getTopic();
            if (time.toSec() - t_current_start_ros.toSec() < 0)
              continue;
            else {
              if (time.toSec() - t_current_end_ros.toSec() <= 0) {
                //cout << time.toSec() - t_current_start_ros.toSec() << " / " << end_time-start_time << endl;
                emit progress((time.toSec() - t_current_start_ros.toSec()) / (t_current_end_ros.toSec() - t_current_start_ros.toSec()) * 100 + 1);
                //QString status = QString::number((time.toSec() - t_current_start_ros.toSec()),10) +"/"+ QString::number((endTime - startTime),10)+" , "+QString::number(i+1,10);
                QString status = QString::fromStdString(std::to_string(i+1)+" / "+std::to_string((int)fileNameVec.size())+" bags");
                emit statusSig(status);
                if (topic == "/lidar0/points_raw") {
                  sensor_msgs::PointCloud2ConstPtr laser_ptr =
                      m.instantiate<sensor_msgs::PointCloud2>();
                  bag_out->write("/lidar0/points_raw", time, *laser_ptr);
                } else if (topic == "/camera_front_center/image_raw") {
                  sensor_msgs::ImageConstPtr image_ptr =
                      m.instantiate<sensor_msgs::Image>();
                  bag_out->write("/camera_front_center/image_raw", time, *image_ptr);
                } else if (topic == "/gps") {
                  lcm2ros::GpsPtr gps_ptr = 
                      m.instantiate<lcm2ros::Gps>();
                  bag_out->write("/gps", time, *gps_ptr);
                }
              } else
                break;
            }
            if(exit_loop)
              break;
          }

          bag_in->close();

          bag_in = NULL;
          full_view = NULL;
    }

      bag_out->close();
      cout << "finish" << endl;
    //ros::spin();
    emit rosDowm();
}
void bagSegment::rosShutDown_slot()        // 8.收到主界面的关闭信号时，停止ros，子线程执行函数自动退出
{
    if(ros::isStarted()) {
         ros::shutdown(); // explicitly needed since we use ros::start();
         ros::waitForShutdown();
       }
}
