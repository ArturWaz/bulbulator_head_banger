/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <UM7_LT.h>
#include <Vector.h>
#include <Matrix.h>

using namespace std;

template <typename T>
void test(Vector<T> &v){
    cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    cout << v.getRows() << ", " << v.getColumns() << endl << endl;
}

template <typename T>
void test(Matrix<T> &m){
    for (int i = 0; i < m.getRows(); ++i) {
        for (int j = 0; j < m.getColumns(); ++j) {
            cout << m(i,j) << ", ";
        }
        cout << endl;
    }
    cout << m.getRows() << ", " << m.getColumns() << endl << endl;
}

int main(){

    Vector<double> v(3);
    v(0) = 5;
    v(1) = 4;
    v(2) = 2;

    Matrix<double> m(3,1);
    m(0,0) = 1; //m(0,1) = 2; m(0,2) = 3;
    m(1,0) = 2; //m(1,1) = -6; m(1,2) = 7;
    m(2,0) = 6; //m(2,1) = ; m(2,2) = ;

    m.transpose();

    Matrix<double> m1(1,3);
    m1(0,0) = 1; m1(0,1) = 3; m1(0,2) = 5;
//    m1(1,0) = 7; m1(1,1) = 5; m1(1,2) = 9;
//    m1(2,0) = 4; m1(2,1) = 2; m1(2,2) = 8;

    m1.transpose();

    Matrix<double> m2(v);

//    m2 = m2*m1;

    m2.transpose();

    test(m);
    test(m1);
    test(m2);




//    test(m3);

//    UM7_LT test(16);
//
//    UM7_LT_packet lol;
//
//    test.turnOnThreadedRead();
//    while (true){
//        test.takeLastPacket(lol);
//    }

    return 0;
}
