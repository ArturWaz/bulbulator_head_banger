#include "UM7LT.h"





UM7LT::UM7LT(int portNumber): _base_UM7LT(portNumber){

}

UM7LT::~UM7LT() {

}


void showPacket_UM7_LT_trololo(UM7_LT_packet const &p){
    std::cout << "late:   " << std::hex;
    for (int i = 0; i < p.length; ++i) {
        std::cout << " " << int(p.data[i]);
    }
    std::cout << std::dec << std::endl;
}



LastAcquiredData UM7LT::getNewData() {
    LastAcquiredData ret;
    UM7_LT_packet packet;
    if (!getLastPacket(packet)) return NONE;

    const double pi = 3.141592653589793238462643383279502884197169;
    const double gravityConstant = 9.8; // recommended: 9.8, page: 4, from: http://www.chrobotics.com/docs/AN-1008-SensorsForOrientationEstimation.pdf
    union uIntIntUnion { uint16_t ui; int16_t i; };
    union doubleIntUnion { uint32_t ui; float f; };
    uIntIntUnion tmpUI[5];
    doubleIntUnion tmpUF[5];

//    showPacket_UM7_LT_trololo(packet);

    switch (packet.address) {
        case (0x61): // Gyro processed data
            tmpUF[0].ui = (packet.data[0]<<24) | (packet.data[1]<<16) | (packet.data[2]<<8) | packet.data[3];
            tmpUF[1].ui = (packet.data[4]<<24) | (packet.data[5]<<16) | (packet.data[6]<<8) | packet.data[7];
            tmpUF[2].ui = (packet.data[8]<<24) | (packet.data[9]<<16) | (packet.data[10]<<8) | packet.data[11];
            tmpUF[4].ui = (packet.data[12]<<24) | (packet.data[13]<<16) | (packet.data[14]<<8) | packet.data[15];
            gyroscope.push_back(TimeVector3D((tmpUF[0].f*pi)/180,(tmpUF[1].f*pi)/180,(tmpUF[2].f*pi)/180,double(tmpUF[4].f)));
            ret = GYRO;
            break;
        case (0x65): // Accelerometer processed data
            tmpUF[0].ui = (packet.data[0]<<24) | (packet.data[1]<<16) | (packet.data[2]<<8) | packet.data[3];
            tmpUF[1].ui = (packet.data[4]<<24) | (packet.data[5]<<16) | (packet.data[6]<<8) | packet.data[7];
            tmpUF[2].ui = (packet.data[8]<<24) | (packet.data[9]<<16) | (packet.data[10]<<8) | packet.data[11];
            tmpUF[4].ui = (packet.data[12]<<24) | (packet.data[13]<<16) | (packet.data[14]<<8) | packet.data[15];
            accelerometer.push_back(TimeVector3D(tmpUF[0].f*gravityConstant,tmpUF[1].f*gravityConstant,tmpUF[2].f*gravityConstant,double(tmpUF[4].f)));
            ret = ACC;
            break;
        case (0x6D): // Quaternion data
            tmpUI[0].ui = (packet.data[0]<<8) | packet.data[1];
            tmpUI[1].ui = (packet.data[2]<<8) | packet.data[3];
            tmpUI[2].ui = (packet.data[4]<<8) | packet.data[5];
            tmpUI[3].ui = (packet.data[6]<<8) | packet.data[7];
            tmpUF[0].ui = (packet.data[8]<<24) | (packet.data[9]<<16) | (packet.data[10]<<8) | packet.data[11];
            quaternions.push_back(TimeQuaternion(double(tmpUI[0].i)/29789.09091,double(tmpUI[1].i)/29789.09091,double(tmpUI[2].i)/29789.09091,double(tmpUI[3].i)/29789.09091,double(tmpUF[0].f)));
            ret = QUAT;
            break;
        case (0x70): // Euler angles data
            tmpUI[0].ui = (packet.data[0]<<8) | packet.data[1];
            tmpUI[1].ui = (packet.data[2]<<8) | packet.data[3];
            tmpUI[2].ui = (packet.data[4]<<8) | packet.data[5];
            tmpUF[0].ui = (packet.data[16]<<24) | (packet.data[17]<<16) | (packet.data[18]<<8) | packet.data[19];
            eulerAngles.push_back(TimeVector3D((double(tmpUI[0].i)*pi)/(91.02222*180),(double(tmpUI[1].i)*pi)/(91.02222*180),(double(tmpUI[2].i)*pi)/(91.02222*180),double(tmpUF[0].f)));
            ret = EULER;
            break;
        default:
            ret = NONE;
            break;
    }
    return ret;
}
