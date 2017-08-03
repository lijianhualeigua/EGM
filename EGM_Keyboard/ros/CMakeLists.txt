cmake_minimum_required(VERSION 2.8.3)
project(egm)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

find_package(Protobuf REQUIRED)

include_directories(include ${catkin_INCLUDE_DIRS} ${PROTOBUF_INCLUDE_DIRS})

add_executable(egm src/EGM_Joystick.cpp src/egm.pb.cc src/egm.cpp)
target_link_libraries(egm ${catkin_LIBRARIES} ${PROTOBUF_LIBRARIES})
