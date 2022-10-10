#ifndef BOT_HPP
#define BOT_HPP

#include <stdexcept>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <ctime>
#include <vector>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include "Utils.hpp"

#define BUF_SIZE 	512
#define BREAKCONNECT 1
#define REPLY   	0b0001
#define ERROR   	0b0010
#define COMMAND 	0b0100
#define BROKEN  	0b1000

typedef struct s_msg
{
	std::string 	prefix;
	std::string 	cmd;
	unsigned int	number;
	std::vector <std::string> midParams;
	std::string 	trailing;
	unsigned int 	type;
}				t_msg;

class Bot {
	private:
		std::string 				cmds;
		std::vector<std::string>	reply;
		typedef int ( Bot:: *Type )( );
	    std::map<std::string, Type>	commands;
		std::string					nick;
		std::string					user;
		std::vector<std::string> 	messages;
		std::vector<std::string> 	channels;

		std::string 		srvPass;
		std::string 		srvHost;
		int					srvPort;
		int					srvSock;
		struct pollfd		srvPoll;
		struct sockaddr_in 	srvAddr;

		t_msg				msg;
		time_t          	breakTime;

		void	registration( void );
		void	reconnect( void );
		int		handleRequest( void );
		void	replyRequset( void );
		int		readRequest( void );
		void	parseMessage( void );
		void	clearMessage( void );

		void	processCommand( void );
		void	processReply( void );
		void	processError( void );
		void	sendMessage( std::string const & cmd, std::string const & arg1 = "", std::string const & arg2 = "",\
					   std::string const & arg3 = "", std::string const & arg4 = "", std::string const & arg5 = "");
		void	initMap();
		void	execute( std::string & nick );
		void	showInfo();

		// commands 
		int command();
		int help();
		int passCmd();
		int nickCmd();
		int userCmd();
		int operCmd();
		int quitCmd();
		int joinCmd();
		int partCmd();
		int modeCmd();
		int topicCmd();
		int namesCmd();
		int listCmd();
		int inviteCmd();
		int kickCmd();
		int versionCmd();
		int timeCmd();
		int adminCmd();
		int infoCmd();
		int privmsgCmd();
		int noticeCmd();
		int whoCmd();
		int whoisCmd();
		int whowasCmd();
		int killCmd();
		int pingCmd();
		int pongCmd();
		int awayCmd();
		int rehashCmd();
		int restartCmd();
		int userhostCmd();
		int isonCmd();

		//trash
		void 			spam( void );
		time_t          pingTime;

		Bot();
	public:
		void	create();
		void	run();

		Bot( std::string const & host, std::string const & port, std::string const & pass );
		~Bot();
};

#endif