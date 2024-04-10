#ifndef VOLUMEEQ_H
#define VOLUMEEQ_H
#include <string>
#include <iostream>

class VolumeEQ {

    public: 

        VolumeEQ(int value1, int value2, int value3, int value4, int value5);

        int getBass();

        int getTreble();

        int getLow();

        int getMid();

        int getHigh();

        void setBass(int val);

        void setTreble(int val);

        void setLow(int val);

        void setMid(int val);

        void setHigh(int val);

    private:

        int Bass;

        int Treble;

        int Low;

        int Mid;

        int High;



};

#endif