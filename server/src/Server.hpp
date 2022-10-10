#ifndef Server_hpp
#define Server_hpp

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include "User.hpp"
#include "Utils.hpp"
#include "SHA256.hpp"
#include "Channel.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cctype>
#include <ctime>

#define WORKING 0b10
#define RESTART 0b01
#define BUF_SIZE 512
#define CONF_PATH "conf/IRCconf.json"

typedef struct s_msg
{
	std::string prefx;
	std::string cmd;
	std::vector <std::string> midParams;
	std::string trailing;
	int paramN;
}				t_msg;

typedef struct s_servInf
{
	std::string	serverName;
	std::string	srvStartTime;
	std::string	srvVersion;
	std::string	adminName;
	std::string	adminEmail;
	std::map<std::string, std::string> oper;
}				t_servInf;

class Server {
	private:

		std::vector<struct pollfd>	userFds;
		std::vector<User*>			userData;
		std::vector<User*>			history;
		typedef int (Server:: * PType)( User &user );
    	std::map<std::string, PType>	commands;
		std::map<std::string, Channel *> channels;
		std::vector<std::string>	servInfo;
		t_msg 						msg;
		t_servInf					inf;
		time_t						inactveTime;
		time_t						responseTime;

		int					maxChannels;
		struct pollfd		srvPoll;
		int					srvFd;
		int					srvPort;
		std::string			srvPass;
		struct sockaddr_in	address;
		int					status;
		
		void 		connectUsers( void );
		void 		clientRequest( void );
		void 		disconnectClient( size_t const id );
		int  		readRequest( size_t const id );
		void 		executeCommand( size_t const id );
		void 		checkUserConnection( void );
		void 		consoleCommands( void );

		// parser
		int			parseMsg(size_t id);
		int			checkMsgFormat( size_t id);
		void		cleanMsgStruct();
		void		processWildcard();
		void		printStuct();
		int			parseConf();

		// commands
		void		initCommandMap( void );
    	void		execute(std::string const &, User &);
    	int			pass(User & user );
    	int			nick(User & user );
    	int			user(User & user );
    	int			oper(User & user );
    	int			quit(User & user );
		int			who( User & user );
		int			whois( User & user );
		int			whowas( User & user );
		int			join( User & user );
		int			part( User & user );
		int			list( User & user );
		int	 		names( User & user );
		int			topic( User & user );
		int			mode( User & user );
		int			version( User & user );
		int			time( User & user );
		int			admin( User & user );
		int			info( User & user );
		int			ison( User & user );
		int			userhost( User & user );
		int			kill( User & user );
		int			restart( User & user );
		int			rehash( User & user );
		int			invite( User & user );
		int			kick( User & user );
		int			motd( User & user );
		int			ping( User & user );
		int			pong( User & user );
		int			away( User & user );
		int			privmsg( User & user );
		int			notice( User & user );

		// server utils
		void		setChannelMode( Channel * channel, User & user );
		void		setUserMode( User & user );
		void		closeChannel( Channel * channel );
		User& 		getUserByNick( std::string nick );
		int			killUser( User & user );
    	int			connection( User & user );
		bool		notRegistr( User & user );
		bool 		validNick( void );
		bool		isChannel( std::string name );
		int			errorMEss( int err, User &user, const std::string &str = "" );
		int			replyMEss( int reply, User &user, const std::string &str = "" );
		void		showMEss( User const & from, Channel const * channel, bool andfrom = 0 );
		void 		showMEss( User const & from, User const & to, bool andfrom = 0 );
		void 		showMEss( User const & from, Channel const * channel, std::string str, bool andfrom = 0 );
		std::string checkTime();
		void		sendPrivMsg(User &fromUser, User &toUser, const std::string &str);
		void		awayRpl(User &user, User &awayUser);

		//trash
		void		printUserVector( std::vector<User*> users );

		Server( Server const & src );
		Server operator=( Server const & src ); 

	public:

		Server( std::string const & port, std::string const & pass);
		~Server();

		void	create( void );
		void	run( void );

};

#endif