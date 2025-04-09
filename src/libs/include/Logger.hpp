/**
 * @file Logger.hpp
 * @date 2025-03-22
 * @author Daniel FERREIRA LARA
 *
 * @brief Classe Logger
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>

class Logger {
private:
    std::ofstream fichierLog;    

public:
    enum class Level { INFO, WARNING, ERROR, DEBUG };

    // Constructeur
    Logger(const std::string& nomFichier = "system.log", bool append = true);
    ~Logger();

    void log(const std::string& message, const bool full = true);
    void log(const std::string& message, Level level, const bool full = true);
    static void log(const std::string& message, Level level, const bool full, const bool append);
    
    void fermer();

private:
    static std::string getTimestamp();
    static std::string levelToString(Level level);
};

#endif // LOGGER_HPP
