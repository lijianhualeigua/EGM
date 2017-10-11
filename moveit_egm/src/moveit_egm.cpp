#include "ros/ros.h"
#include "std_msgs/String.h"
#include <moveit_msgs/DisplayTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <moveit_egm/egm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <unistd.h> 

char protoMessage[1400];
double feedback_joints[6];
std::string messageBuffer;
bool joint_cmd = true;
pos pos;
std::vector<trajectory_msgs::JointTrajectoryPoint> pts;
bool executed = true;
double tol = 0.1;

void moveitCallback(boost::shared_ptr<moveit_msgs::DisplayTrajectory const> traj) { 
	int count = 0;
	while (count < traj->trajectory[0].joint_trajectory.points.size()) {
		pts.push_back(traj->trajectory[0].joint_trajectory.points[count]);
		count++;
	}
}

int ct = 0;
void takeoutposition() {
	for (int i = 0; i < 6; i++) {
		pos.joints[i] = pts[ct].positions[i] * 180 / 3.1415926;
		std::cout << pos.joints[i];
	}
	ct++;
	if (ct == pts.size())
		ct = pts.size() - 1;
}

int main(int argc, char** argv) {   
	ros::init(argc, argv, "moveit_to_EGM");
	ros::NodeHandle nh;
	ros::Subscriber sub;
	int sockfd;
	socklen_t len;
	int n;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot create socket");
		return 0;
	}

	struct sockaddr_in serverAddr, clientAddr;
	memset(&serverAddr, sizeof(serverAddr), 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(portNumber);

	int result = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); 
	boost::shared_ptr<moveit_msgs::DisplayTrajectory const> pt;
	if (!result) {
		while (!pt) {
			pt = ros::topic::waitForMessage<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", ros::Duration(5));
		}
		moveitCallback(pt);

		while (ros::ok()) {
			if (executed)
				takeoutposition();
			executed = false;
			// receive and display message from robot
			len = sizeof(clientAddr);
			n = recvfrom(sockfd, protoMessage, 1400, 0, (struct sockaddr *)&clientAddr, &len);
			if (n < 0) {
				printf("Error receive message\n");
				continue;
			}
			// parse inbound message
			EgmRobot *pRobotMessage = new EgmRobot();
			pRobotMessage->ParseFromArray(protoMessage, n);
			DisplayRobotMessage(pRobotMessage);
			//delete pRobotMessage;

			//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

			// current joint position
			for (int i = 0; i < 6; i++) {
				feedback_joints[i] = (pRobotMessage->feedback().joints().joints(i)) * rad_deg;
			} 

			if (abs(feedback_joints[0] - pos.joints[0]) < tol && abs(feedback_joints[1] - pos.joints[1]) < tol && abs(feedback_joints[2] - pos.joints[2]) < tol && abs(feedback_joints[3] - pos.joints[3]) < tol && abs(feedback_joints[4] - pos.joints[4]) < tol && abs(feedback_joints[5] - pos.joints[5]) < tol)
				executed = true;
			// create and send a sensor message
			EgmSensor *pSensorMessage = new EgmSensor();
			CreateSensorMessage(pSensorMessage, pos, joint_cmd);
			pSensorMessage->SerializeToString(&messageBuffer);

			// send a message to the robot
			n = sendto(sockfd, messageBuffer.c_str(), messageBuffer.length(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));

			//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			//std::cout << "time delay is: " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << std::endl;
			if (n < 0) {
				printf("Error send message\n");
			}
			//delete pSensorMessage;
			ros::spinOnce();
			//ros::Duration(0.1).sleep();
		} 
	}
	else 
		return -1;
	//close(sockfd);
}

