#include "EGM.h"

unsigned int EGM::sequenceNumber = 0;

int main(int argc, char** argv) {
    static int portNumber = 6510;
    EGM egm;

    std::cout << "Commands: c = set Cartesian pose command, j = set joint pose command" << std::endl;
#ifdef _WIN32
    char c = _getch();
#elif __linux__
    // This will clear the screen!
    initscr();
    char c = (char)getch();
    nodelay(stdscr, TRUE);
    endwin();
#endif

    switch (c) {
    case 'c':
        std::cout << "Please specify the Cartesian position (mm) for the robot." << std::endl;
        std::cin >> egm.cart_pose.cart[0] >> egm.cart_pose.cart[1] >> egm.cart_pose.cart[2];
        std::cout << "Please specify the Euler angle orientation (degree) for the robot." << std::endl;
        std::cin >> egm.cart_pose.orient[0] >> egm.cart_pose.orient[1] >> egm.cart_pose.orient[2];
        break;
    case 'j':
        std::cout << "Please specify the joint pose (degree) for the robot." << std::endl;
        std::cin >> egm.joints[0] >> egm.joints[1] >> egm.joints[2] >> egm.joints[3] >> egm.joints[4] >> egm.joints[5];
        egm.joint_cmd = true;
        break;
    }
    
    egm.UDP_Com(portNumber);
    return 0;
}