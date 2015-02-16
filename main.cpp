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

void test(UM7_LT_packet const &p){
    cout << "main:   " << hex;
    for (int i = 0; i < p.length; ++i) {
        cout << " " << int(p.data[i]);
    }
    cout << dec << endl;
}

int main(){




    UM7_LT um7(4);

    UM7_LT_packet lol;

    um7.turnOnThreadedRead();
    while (true){
        if (um7.takeLastPacket(lol)){
            test(lol);
            free(lol.data);
        }
    }

    return 0;
}
