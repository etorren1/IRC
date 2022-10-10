#include "User.hpp"
#include "Utils.hpp"

User::User(int serverSocket, int mySocket, struct sockaddr_in address) {
    username = "";
    nickname = "";
    password = "";
    realname = "";
    hostname = "";
    quitMess = "";
    servername = "";
    flags = 0;
    registred = 0;
    breakconnect = false;
    srvFd = serverSocket;
    fd = mySocket;
    sockaddr = address;
    userRegistTime = 0;
    lastMessTime = 0;
    char ip[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &sockaddr.sin_addr, ip, INET_ADDRSTRLEN );
    ipv4 = ip;
}

User::User( User const & src ) {
	*this = src;
}

User &User::operator=( User const & src ) {
	if (this == &src)
		return(*this);
	username = src.username;
	nickname = src.nickname;
	password = src.password;
	realname = src.realname;
	hostname = src.hostname;
	servername = src.servername;
	quitMess = src.quitMess;
	channels = src.channels;
	fd = src.fd;
	srvFd = src.srvFd;
	registred = src.registred;
	breakconnect = src.breakconnect;
	messages = src.messages;
	return(*this);
}

User::~User() {}

std::string const & User::getNick( void ) const { return nickname; }
std::string const & User::getUser( void ) const { return username; }
std::string const & User::getPass( void ) const { return password; }
std::string const & User::getIp( void ) const { return ipv4; }
std::string const & User::getReal( void ) const { return realname ; }
std::string const & User::getHost( void ) const { return hostname ; }
std::string const & User::getServer( void ) const { return servername ; }
std::string const User::getPrefix( void ) const
{
    std::string prefix = ":" + nickname + "!" + username + "@" + ipv4;
    return (prefix);
}
std::string const & User::getQuitMess( void ) const { return quitMess ; }
std::string const & User::getAwayMess( void ) const { return awayMess ; }
std::string const & User::getKillComment( void ) const { return killComment ; }
int const & User::getFd( void ) const { return fd ; }
bool const & User::getBreakconnect( void ) const { return breakconnect ; }
char const & User::getFlags( void ) const { return flags; }
time_t const & User::getRegistTime( void ) const { return userRegistTime; }
time_t const & User::getLastMessTime( void ) const { return lastMessTime; }
time_t const & User::getPingTime( void ) const { return pingTime; }
struct sockaddr_in & User::getSockAddr( void ) { return sockaddr ; }

void User::setNick( std::string const & nick ) { nickname = nick; }
void User::setUser( std::string const & name ) { username = name; }
void User::setPass( std::string const & pass ) { password = pass; }
void User::setReal( std::string const & real ) { realname = real; }
void User::setHost( std::string const & host ) { hostname = host; }
void User::setServer( std::string const & server ) { servername = server; }
void User::setQuitMess( std::string const & mess ) { quitMess = mess; }
void User::setAwayMess( std::string const & mess ) { awayMess = mess; }
void User::setFlags( char const & flag ) { flags |= flag; }
void User::setKillComment( char const & comment ) { killComment = comment; }
void User::unsetFlags( char const & flag ) { flags &= ~flag; }
void User::setTime() { userRegistTime = timeChecker(); };
void User::setLastMessTime() { lastMessTime = timeChecker(); };
void User::setPingTime() { pingTime = timeChecker(); };

void User::imOper( std::string const & name ) { opchannels.push_back(name); }
void User::imNotOper( std::string const & name ) { eraseString(opchannels, name); }
void User::addChannel(std::string & name) { channels.push_back(name); }
std::vector<std::string> User::getChannelList( void ) const { return channels ; }
std::vector<std::string> User::getOpChannelList( void ) const { return opchannels ; }

void User::leaveChannel(std::string & name) {
    eraseString(channels, name);
    eraseString(opchannels, name);
}

void User::checkConnection( std::string const & mess ) {
    if (mess.find_last_of("\n") != mess.size() - 1)
        breakconnect = true;
    else if (breakconnect)
        breakconnect = false;
}

bool User::empty() {
    int i;
    i = username.empty() \
    + nickname.empty() \
    + password.empty() \
    + realname.empty() \
    + hostname.empty() \
    + servername.empty() \
    + channels.empty();
    if (i == 8)
        return (1);
    return (0);
}

time_t User::timeChecker( ) { 
    time_t result = time(0);
    return (intmax_t)result;
}