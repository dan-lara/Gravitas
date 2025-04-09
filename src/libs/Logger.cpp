/**
 * @file Logger.cpp
 * @date 2025-03-22
 * @author Daniel FERREIRA LARA
 *
 * @brief Implementation of the Logger class.
 */

#include "include/Logger.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>

Logger::Logger(const std::string& filename, bool append) {
    fichierLog.open(filename, append ? std::ios::app : std::ios::trunc);
    if (!fichierLog) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier de log !" << std::endl;
    }
    fichierLog << "[" << getTimestamp() << "] [" << levelToString(Level::DEBUG) << "] === Début du journal ===" << std::endl;
}

void Logger::log(const std::string& message, const bool full) {
    if (!fichierLog.is_open()) return;

    if (fichierLog) {
        if(full)
            fichierLog << "[" << getTimestamp() << "] [" << levelToString(Level::INFO) << "] " << message << std::endl;
        else
            fichierLog << message << std::endl;
    }
}

void Logger::log(const std::string& message, Level level, const bool full) {
    if (!fichierLog.is_open()) return;

    if (fichierLog) {
        if(full)
            fichierLog << "[" << getTimestamp() << "] [" << levelToString(level) << "] " << message << std::endl;
        else
            fichierLog << message << std::endl;
    }
}

void Logger::log(const std::string& message, Level level, const bool full, const bool append) {
    std::ofstream fichierLog; 
    fichierLog.open("system.log", append ? std::ios::app : std::ios::trunc);
    if (!fichierLog) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier de log !" << std::endl;
    } else {
        if(full)
            fichierLog << "[" << getTimestamp() << "] [" << levelToString(level) << "] " << message << std::endl;
        else
            fichierLog << message << std::endl;
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR: return "ERROR";
        case Level::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

void Logger::fermer() {
    if (fichierLog.is_open()) {
        fichierLog.close();
    }
}

Logger::~Logger() {
    fermer();
}
