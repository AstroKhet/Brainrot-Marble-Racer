#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>


class AudioPlayer {
public:
    sf::Music music;
    std::string singer;
    std::vector<sf::Music*> activelyPlaying;

    AudioPlayer() {}

    void setSinger(std::string character, float t) {
        if (character != singer){
            singer = character;
            music.stop();
            music.openFromFile(getAudioPath(singer));
            music.play();
            music.setPlayingOffset(sf::seconds(fmod(t, 45.f)));
        }
    }

    void playAudio(std::string audioName) {
        sf::Music* audio = new sf::Music;
        audio->openFromFile(getAudioPath(audioName));
        audio->play();

        activelyPlaying.push_back(audio);
    }

    void update() {
        for (auto it = activelyPlaying.begin(); it != activelyPlaying.end(); ) {
            if ((*it)->getStatus() != sf::Music::Playing) {
                delete *it;
                it = activelyPlaying.erase(it);
            } 
            else it++;
        }
    }
    std::string getAudioPath(std::string character) {
        return "audio/" + character + ".wav";
    }

};


#endif
