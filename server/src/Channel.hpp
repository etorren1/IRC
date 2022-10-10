#ifndef Channel_hpp
#define Channel_hpp

#include <iostream>
#include "User.hpp"
#include "Utils.hpp"
#include <vector>

#define KEY         0x01   //k - установка на канал ключа (пароля).
#define PRIVATE     0x02   //p - флаг приватности канала;
#define SECRET      0x04   //s - флаг секретности канала;
#define INVITE      0x08   //i - флаг канала invite-only;
#define TOPIC       0x10   //t - при установке этого флага, менять топик могут только операторы;
#define NO_MESS     0x20   //n - запрещает сообщения на канал от посторонних клиентов;
#define MODERATE    0x40   //m - модерируемый канал;
#define LIMITS      0x80   //l - установка ограничения на количество пользователей;


class User;

class Channel
{
    private:
        std::string             name;
        std::string             topic;
        std::vector<User*>      users;
        std::vector<User*>      operators;
        std::vector<User*>      invited;
        std::vector<User*>      voters;
        std::vector<std::string> banmascs;
        std::string             password;
        unsigned int            countUsers;
        unsigned int            userLimit;

        Channel();
        Channel( Channel const & _ot );
        Channel operator=( Channel const & _ot );
    public:

        unsigned int            flags;
        void addUser( User * user );
        bool isBanned( User * user );
        void opUser( User * user );
        void deopUser( User * user );
        bool isOperator( User * user );
        void invUser( User * user );
        void deinvUser( User * user );
        bool isInvited( User * user );
        void voteUser( User * user );
        void devoteUser( User * user );
        bool isVoters( User * user );
        void disconnectUser( User * user );
        void addBanMask( std::string & masc );
        void deleteBanMasc( std::string masc );
        void channelMessage( User * from, std::string const & str, bool andfrom = false );
        std::string getName( void ) const;
        std::string getTopic( void ) const;
        std::string getPass( void ) const;
        unsigned int getCountUsers( void ) const;
        unsigned int getCountVisible( void ) const;
        unsigned int getUserLimit( void ) const;
        std::vector<std::string> getBanMasc( void ) const;
        std::vector<User*> getUserList( void ) const;
		std::vector<User*> getOperList( void ) const;
        void setUserLimit( unsigned int limit );
        void setTopic( std::string & topic );
        void setPass( std::string & pass );
		int isInChannel(User &user);

        Channel(User * creater, std::string name, std::string pass = "");
        ~Channel();
};

#endif