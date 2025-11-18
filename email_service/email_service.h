// email_service.h
#ifndef EMAIL_SERVICE_H
#define EMAIL_SERVICE_H

#include <string>
#include <curl/curl.h>

class EmailService {
private:
    std::string smtp_server;
    std::string smtp_port;
    std::string username;
    std::string password;
    std::string from_email;
    std::string to_email;

    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);

public:
    EmailService(const std::string& server, const std::string& port, 
                 const std::string& user, const std::string& pass,
                 const std::string& from, const std::string& to);
    
    bool sendKeyFoundEmail(const std::string& private_key, 
                          const std::string& address, 
                          double balance,
                          const std::string& found_by);
};

#endif