#include <iostream>
#include "aafm2.h"
using namespace std;



Player::Player(QString sequence)
{
    str = sequence;
}


void Player::play()
{
    for (int i = 0; i < str.size(); ++i)
    {
        if (str.at(i) == '0')
            cout  << "play 1400Hz \n" << endl;
        if (str.at(i) == '1')
            cout  << "play 2100Hz \n " << endl;
    }



}

