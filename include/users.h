#ifndef USERS_H
#define USERS_H
#include <iostream>

namespace SqlitoSeguro
{
    class Usuario
    {
        /*
        definir: constructor (crear), actualizar y verificar existencia
        */
        private:
            int m_id;
            std::string m_usr;
            int m_createdAt;


        public:
            //constructor
            Usuario(int id, const std::string& usr,const std::string& email, const std::string& pwd);

            //getters
            int GetId() const;
            std::string GetUsuario() const;
            std::string GetEmail() const;
            std::string Getpwd() const;

            //getters
            
            void SetUsuario() const;
            void SetEmail() const;
            void Setpwd() const;
    };
}

#endif
