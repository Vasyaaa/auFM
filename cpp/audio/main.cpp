#include <QCoreApplication>

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;
//player = new QMediaPlayer;

void gen_sig_sin(vector<int> * signal, float freq, int d_freq, int amp, float len)
{

float count = len * d_freq;
    for (int i=0; i<=count; i++)
    {
        float val = amp * (sin(2 * M_PI *freq * (i*1.0/d_freq)));
        signal->push_back (val);
    }

}




//player->play();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    vector<int> signal;

    gen_sig_sin(&signal, 100, 48000, 100, 0.1);

    for (int i=0; i < signal.size(); i++)
        cout  << signal[i] << endl;


    return a.exec();
}
