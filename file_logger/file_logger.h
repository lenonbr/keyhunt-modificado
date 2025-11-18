// file_logger.h
#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

class FileLogger {
private:
    std::string filename;
    
    std::string getCurrentDateTime();

public:
    FileLogger(const std::string& file = "KEYS_FOUND.txt");
    
    bool logFoundKey(const std::string& private_key, 
                    const std::string& address, 
                    double balance,
                    const std::string& found_by,
                    const std::string& range_info = "");
    
    bool logError(const std::string& error_message);
    bool logProgress(const std::string& progress_message);
};

#endif