#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Define.hpp"

#define S1_CONNECT 0
#define S2_PASSWORD 1
#define S3_NICK 2
#define COMPLETED 3
#define DUPDUP 4

class Client {
    private:
        std::string _nickname;
        std::string _username;
        std::string _ip;
        int         _clientFd;
        int         _status;

    public:
        Client(std::string nick, std::string user, std::string ip, int fd, int flag);
        /* set */
        void                set_nick(std::string nick);
        void                set_flag(int flag);
        void                set_ip(std::string ip);
        void                set_user(std::string user);
        
		/* getter */
        const std::string   get_nick() const;
        const std::string   get_user() const;
        const std::string   get_ip() const;
        int                 get_fd() const;
        int                 get_status() const;
  
      
        bool operator<(const Client& c) const;
};

#endif