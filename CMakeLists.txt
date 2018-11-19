cmake_minimum_required(VERSION 2.8.3)
project(qt_bag_segment)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)
set(EXECUTABLE_OUTPUT_PATH ~/catkin_ws/devel/lib/${PROJECT_NAME})

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  lcm2ros
  roscpp
  sensor_msgs
  std_msgs
  rosbag
)

find_package(Qt5  REQUIRED COMPONENTS  Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
  src/qt_bag_segment.cpp
  src/qtComponent/qtDesign/mainwindow.cpp
  src/qtComponent/qtDesign/bagsegment.cpp
)
set(FORMS
  src/qtComponent/qtDesign/mainwindow.ui
)
include_directories(
  include
  #需要将新建类所对应的头文件路径包含进来
  src/qtComponent/qtDesign
  ${catkin_INCLUDE_DIRS}
)
add_executable(qt_bag_segment
  ${SOURCES} ${FORMS}
)
add_dependencies(qt_bag_segment ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(qt_bag_segment
  ${catkin_LIBRARIES} Qt5::Widgets
)