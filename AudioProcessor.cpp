#include "AudioProcessor.h"
#include <string>
#include <iostream>

class AudioProcessor {

    private:

        std::string song;

        int volume;

        VolumeEQ equalizer;

        bool showEQ;

    public:

        AudioProcessor(std::string song, int volume, VolumeEQ equalizer) : song(song), volume(volume), equalizer(equalizer), showEQ(false) {}

        void saveSong(std::string song) {

            this->song = song;

            std::cout << "The song '" << song << "'is now saved.\n";
        }

        void downloadSong(std::string song) {

            this->song = song;

            std::cout << "The song '" << song << "'is now downloaded.\n";
        }

        void addEQ(bool eq) {

            if (eq) {

                showEQ = true;

                std::cout << "Show Equalizer.\n";
            }

            else {

                std::cout << "Could not open Equalizer.\n";
            }
        }

        void removeEQ(bool eq) {

            if (eq) {

                showEQ = false;

                std::cout << "Equalizer closed.\n";
            }

            else {

                std::cout << "Could not close Equalizer.\n";
            }
        }

        void decreaseVolume(int val) {

            volume = volume - val;
        }

        void increaseVolume(int val) {

            volume = volume + val;
        }

        void increaseBass(int val) {

            int currentbass = equalizer.getBass();

            int newbass = currentbass + val;

            equalizer.setBass(newbass);
        }

        void decreaseBass(int val) {

            int currentbass = equalizer.getBass();

            int newbass = currentbass - val;

            equalizer.setBass(newbass);
            
        }

        void increaseTreble(int val) {

            int currenttreble = equalizer.getTreble();

            int newtreble = currenttreble + val;

            equalizer.setTreble(newtreble);

        }

        void decreaseTreble(int val) {

            int currenttreble = equalizer.getTreble();

            int newtreble = currenttreble - val;

            equalizer.setTreble(newtreble);


        }



    

};