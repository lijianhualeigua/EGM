cmake_minimum_required(VERSION 2.8.3)
project(moveit_egm)

find_package(catkin REQUIRED COMPONENTS
  moveit_msgs
  roscpp
  std_msgs
  trajectory_msgs
)

find_package(Boost REQUIRED system filesystem date_time thread)

find_package(Protobuf REQUIRED)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS} ${PROTOBUF_INCLUDE_DIRS})

include_directories(SYSTEM ${Boost_INCLUDE_DIR})

add_executable(moveit_egm src/moveit_egm.cpp src/egm.pb.cc src/egm.cpp)

target_link_libraries(moveit_egm ${catkin_LIBRARIES} ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES})

