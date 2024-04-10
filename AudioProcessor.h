
#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H
#include <string>
#include <iostream>
#include "VolumeEQ.cpp"

class AudioProcessor {

    public:
        AudioProcessor(std::string song, int volume, VolumeEQ equalizer);

        void saveSong(std::string song);

        void downloadSong(std::string song);

        void addEQ(bool eq);

        void removeEQ(bool eq);

        void decreaseVolume(int val);

        void increaseVolume(int val);

        void increaseBass(int val);

        void decreaseBass(int val);

        void increaseTreble(int val);

        void decreaseTreble(int val);

    private:

        std::string song;

        int volume;

        VolumeEQ equalizer;

        bool showEQ;


};

#endif