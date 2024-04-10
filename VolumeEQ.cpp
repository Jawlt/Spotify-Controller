/**
 * @author Ojas Hunjan
 * @date 2024-03-31
 * This class is used to represent the object for the equalizer and the specific audio frequencies 
*/
#include "VolumeEQ.h"//include header file
#include <string>//include string package
#include <iostream>//include iostream package

/**
 * constructor class for EQ object
*/
class VolumeEQ {

    private://private variables

        int Bass;//Bass measure

        int Treble;//Treble measure

        int Low;//Low measure

        int Mid;//Mid measure

        int High;//High measure

    public://public variables

        VolumeEQ(int value1, int value2, int value3, int value4, int value5) {//set values to the equalizer frequencies

            Bass = value1;

            Treble = value2;

            Low = value3;

            Mid = value4;

            High = value5;
        }

        int getBass() {//getters and setters for equalizers

            return Bass;
        }

        int getTreble() {

            return Treble;
        }

        int getLow() {

            return Low;
        }

        int getMid() {

            return Mid;
        }

        int getHigh() {

            return High;
        }

        void setBass(int val) {

            Bass = val;
        }

        void setTreble(int val) {

            Treble = val;
        }

        void setLow(int val) {

            Low = val;
        }

        void setMid(int val) {

            Mid = val;
        }

        void setHigh(int val) {

            High = val;
        }


};

