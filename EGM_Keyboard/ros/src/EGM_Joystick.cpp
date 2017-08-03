#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
#include <egm.h>
#include <ncurses.h>  // sudo apt-get install ncurses-dev
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

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {   
    for (int i = 0; i < 6; i++) {
        pos.joints[i] = feedback_joints[i] + ((double)joy->buttons[i]) * 10.0;
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "joy_to_EGM");
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
    
    if (!result) {
    sub = nh.subscribe<sensor_msgs::Joy>("/joy", 1000, &joyCallback); 
    while (ros::ok()) {
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

        // current joint position
        for (int i = 0; i < 6; i++) {
            feedback_joints[i] = (pRobotMessage->feedback().joints().joints(i)) * rad_deg;
        } 
        
        // create and send a sensor message
        EgmSensor *pSensorMessage = new EgmSensor();
        CreateSensorMessage(pSensorMessage, pos, joint_cmd);
        pSensorMessage->SerializeToString(&messageBuffer);

        // send a message to the robot
        n = sendto(sockfd, messageBuffer.c_str(), messageBuffer.length(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        if (n < 0) {
            printf("Error send message\n");
            }
        //delete pSensorMessage;
        ros::spinOnce();
    } 
}
    else 
        return -1;
    //close(sockfd);
}
