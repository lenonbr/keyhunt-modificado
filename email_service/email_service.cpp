// email_service.cpp
#include "email_service.h"
#include <iostream>
#include <sstream>
#include <vector>

// Callback para libcurl
size_t EmailService::read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    std::string *upload_data = (std::string*)userp;
    size_t data_size = upload_data->size();
    
    if(data_size) {
        size_t copy_size = std::min(size * nmemb, data_size);
        memcpy(ptr, upload_data->c_str(), copy_size);
        upload_data->erase(0, copy_size);
        return copy_size;
    }
    return 0;
}

EmailService::EmailService(
    const std::string& server, const std::string& port, 
    const std::string& user, const std::string& pass,
    const std::string& from, const std::string& to
)
    : smtp_server(server), smtp_port(port), username(user), 
      password(pass), from_email(from), to_email(to) {}

bool EmailService::sendKeyFoundEmail(
    const std::string& private_key, 
                                   const std::string& address, 
                                   double balance,
                                   const std::string& found_by
) {
    
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    
    std::cout << "📧 Tentando enviar email..." << std::endl;
    
    curl = curl_easy_init();
    if(curl) {
        // Configurar servidor SMTP
        std::string url = "smtp://" + smtp_server + ":" + smtp_port;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_email.c_str());
        
        recipients = curl_slist_append(recipients, to_email.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        
        // Autenticação
        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
        
        // Para servidores que requerem SSL/TLS
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Desativar verificação SSL para testes
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        // Construir o email
        std::stringstream email_text;
        email_text << "From: BTC Hunter <" << from_email << ">\r\n"
                   << "To: " << to_email << "\r\n"
                   << "Subject: 🎉 CHAVE ENCONTRADA! - " << balance << " BTC\r\n"
                   << "\r\n"
                   << "🌟 PARABÉNS! UMA CHAVE PRIVADA FOI ENCONTRADA! 🌟\r\n"
                   << "=============================================\r\n"
                   << "\r\n"
                   << "💰 Saldo: " << balance << " BTC\r\n"
                   << "📍 Endereço: " << address << "\r\n"
                   << "🔑 Chave Privada: " << private_key << "\r\n"
                   << "👤 Encontrado por: " << found_by << "\r\n"
                   << "🕒 Data/Hora: " << __DATE__ << " " << __TIME__ << "\r\n"
                   << "\r\n"
                   << "⚠️  ATENÇÃO: Esta é uma chave real! Tome as devidas precauções de segurança.\r\n"
                   << "\r\n"
                   << "Happy Hunting! 🚀\r\n";
        
        std::string email_data = email_text.str();
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &email_data);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        
        // Timeout de 30 segundos
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        
        std::cout << "📤 Enviando email via " << smtp_server << "..." << std::endl;
        
        // Enviar email
        res = curl_easy_perform(curl);
        
        // Limpar
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
        
        if(res != CURLE_OK) {
            std::cerr << "❌ Erro ao enviar email: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
        
        std::cout << "✅ Email de notificação enviado com sucesso!" << std::endl;
        return true;
    }
    
    std::cerr << "❌ Falha ao inicializar CURL" << std::endl;
    return false;
}