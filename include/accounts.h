#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>

namespace SqlitoSeguro
{
    class Cuentas
    /*
    solo declarar las clases, toda la definicion  se implementa en el .cpp de la clase
    */
    {
        private:
            int m_Id;
            int m_usrId;
            std::string m_username;
            std::string m_service;
            std::string m_url;
            std::string m_login;
            std::string m_pwd;
            int m_createdAt;
        public:
            //constructor
            Cuentas(int usr_id, std::string nombre, const std::string& url,const std::string& login, const std::string& pwd);

            //getters
            std::string GetNombre() const;
            std::string GetSitio() const;
            std::string GetLogin() const;
            std::string Getpwd() const;

            //getters
            
            void SetNombre() const;
            void SetSitio() const;
            void SetLogin() const;
            void Setpwd() const;
    };
}

#endif
