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
		std::string _hostname;
        std::string _servername;
		std::string _realname;
		
        int         _clientFd;
        int         _status;

    public:
        Client(std::string nick, std::string user, std::string ip, std::string hostname, int fd, int flag);
        //set
        void                set_nick(std::string nick);
        void                set_flag(int flag);
        void                set_servername(std::string ip);
        void                set_realname(std::string user);
		void 				set_hostname(std::string hostname);
        //get
        const std::string   get_nick() const;
        const std::string   get_realname() const;
        const std::string   get_servername() const;
		const std::string 	get_hostname() const;
        int                 get_fd() const;
        int                 get_status() const;
  
      
        bool operator<(const Client& c) const;
};

#endif