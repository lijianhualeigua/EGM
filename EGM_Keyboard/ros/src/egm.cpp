#include <egm.h>

void CreateSensorMessage(EgmSensor* pSensorMessage, pos &pos, bool &joint_cmd) {
    EgmHeader* header = new EgmHeader();
    header->set_mtype(EgmHeader_MessageType_MSGTYPE_CORRECTION);
    header->set_seqno(sequenceNumber++);
#ifdef _WIN32
    header->set_tm(GetTickCount());
#endif
    pSensorMessage->set_allocated_header(header);

    EgmPlanned *planned = new EgmPlanned();

    // Joint command 
    if (joint_cmd) {
        EgmJoints *pj = new EgmJoints();
        // First add desired number of joints (type: repeated)
        pj->add_joints(pos.joints[0]);
        pj->add_joints(pos.joints[1]);
        pj->add_joints(pos.joints[2]);
        pj->add_joints(pos.joints[3]);
        pj->add_joints(pos.joints[4]);
        pj->add_joints(pos.joints[5]);
        planned->set_allocated_joints(pj);
    }
    // Cartesian command
    else {
        EgmCartesian *pc = new EgmCartesian();
        pc->set_x(pos.cart_pose.cart[0]);
        pc->set_y(pos.cart_pose.cart[1]);
        pc->set_z(pos.cart_pose.cart[2]);

        /*EgmQuaternion *pq = new EgmQuaternion();
        pq->set_u0(1.0);
        pq->set_u1(0.0);
        pq->set_u2(0.0);
        pq->set_u3(0.0);*/

        // Euler angle
        EgmEuler *pe = new EgmEuler();
        pe->set_x(pos.cart_pose.orient[0]);
        pe->set_x(pos.cart_pose.orient[1]);
        pe->set_x(pos.cart_pose.orient[2]);

        EgmPose *pcartesian = new EgmPose();
        pcartesian->set_allocated_euler(pe);
        //pcartesian->set_allocated_orient(pq);
        pcartesian->set_allocated_pos(pc);
        planned->set_allocated_cartesian(pcartesian);
    }
    pSensorMessage->set_allocated_planned(planned);
}

// Display inbound robot message
void DisplayRobotMessage(EgmRobot *pRobotMessage) {
    if (pRobotMessage->has_header() && pRobotMessage->header().has_seqno() && pRobotMessage->header().has_tm() && pRobotMessage->header().has_mtype()) {
        printf("SeqNo=%d || Tm=%u || Type=%d\n", pRobotMessage->header().seqno(), pRobotMessage->header().tm(), pRobotMessage->header().mtype());
        printf("Joint=%6.2lf || %6.2lf || %6.2lf || %6.2lf || %6.2lf || %6.2lf \n", pRobotMessage->feedback().joints().joints(0)*rad_deg, pRobotMessage->feedback().joints().joints(1)*rad_deg,
            pRobotMessage->feedback().joints().joints(2)*rad_deg, pRobotMessage->feedback().joints().joints(3)*rad_deg,
            pRobotMessage->feedback().joints().joints(4)*rad_deg, pRobotMessage->feedback().joints().joints(5)*rad_deg);
    }
    else {
        printf("No header\n");
    }
}
