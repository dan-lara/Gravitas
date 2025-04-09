/**
 * @file SoundManager.cpp
 * @author Gravitas
 *
 * @brief SoundManager class implementation
 */

#include "include/SoundManager.hpp"

SoundManager:: SoundManager()
    : launchSound(launchBuffer),
    winSound(winBuffer),
    failSound(failBuffer){
        std::string sound_path = SOUND_PATH;
        if (!launchBuffer.loadFromFile(sound_path + "launch.wav")) std::cerr << "Failed to load launch.wav\n";
        if (!winBuffer.loadFromFile(sound_path + "win.wav")) std::cerr << "Failed to load win.wav\n";
        if (!failBuffer.loadFromFile(sound_path + "fail.wav")) std::cerr << "Failed to load fail.wav\n";
}

