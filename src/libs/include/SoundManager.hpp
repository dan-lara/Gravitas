/**
 * @file SoundManager.hpp
 * @author Gravitas
 *
 * @brief Definition of the SoundManager class.
 */

// #pragma once
#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

#define SOUND_PATH "src/assets/sounds/"

class SoundManager {
    sf::SoundBuffer launchBuffer, winBuffer, failBuffer;
    sf::Sound launchSound;
    sf::Sound winSound;
    sf::Sound failSound;

public:
    SoundManager();

    void playLaunch() { launchSound.play(); }
    void playWin()    { winSound.play(); }
    void playFail()   { failSound.play(); }
};

#endif // SOUND_MANAGER_HPP
