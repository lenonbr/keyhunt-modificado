// file_logger.cpp
#include "file_logger.h"
#include <iostream>

FileLogger::FileLogger(const std::string& file) : filename(file) {}

std::string FileLogger::getCurrentDateTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool FileLogger::logFoundKey(const std::string& private_key, 
                           const std::string& address, 
                           double balance,
                           const std::string& found_by,
                           const std::string& range_info) {
    
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "❌ Erro ao abrir arquivo: " << filename << std::endl;
        return false;
    }
    
    file << "╔══════════════════════════════════════════════════════════════╗\n";
    file << "║                    🎉 CHAVE ENCONTRADA! 🎉                   ║\n";
    file << "╠══════════════════════════════════════════════════════════════╣\n";
    file << "║ Data/Hora: " << getCurrentDateTime() << std::string(38 - getCurrentDateTime().length(), ' ') << "║\n";
    file << "║ Endereço: " << address << std::string(46 - address.length(), ' ') << "║\n";
    file << "║ Saldo: " << balance << " BTC" << std::string(42 - std::to_string(balance).length(), ' ') << "║\n";
    file << "║ Encontrado por: " << found_by << std::string(41 - found_by.length(), ' ') << "║\n";
    
    if (!range_info.empty()) {
        file << "║ Range: " << range_info << std::string(48 - range_info.length(), ' ') << "║\n";
    }
    
    file << "╠══════════════════════════════════════════════════════════════╣\n";
    file << "║                      🔑 CHAVE PRIVADA 🔑                     ║\n";
    file << "║ " << private_key << " ║\n";
    file << "╚══════════════════════════════════════════════════════════════╝\n\n";
    
    file.close();
    
    std::cout << "💾 Chave salva em: " << filename << std::endl;
    return true;
}

bool FileLogger::logError(const std::string& error_message) {
    std::ofstream file("hunter_errors.log", std::ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentDateTime() << "] ERRO: " << error_message << "\n";
        file.close();
        return true;
    }
    return false;
}

bool FileLogger::logProgress(const std::string& progress_message) {
    std::ofstream file("hunter_progress.log", std::ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentDateTime() << "] " << progress_message << "\n";
        file.close();
        return true;
    }
    return false;
}