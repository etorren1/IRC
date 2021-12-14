#include "Server.hpp"

#define RPL_ADMINME 256 		//" :Admin name - "													// 256
#define RPL_ADMINLOC1 257		//"Location - Kazan, Republic of Tatarstan, Russian Federation\n" 	// 257
#define RPL_ADMINEMAIL 259		//":Admin email - "												// 259
#define RPL_USERHOST 302		//":" [<reply>{<space><reply>}] <reply> ::= <nick>['*'] '=' <'+'|'-'><hostname>"
#define RPL_ISON 303			//":"													// 303
#define RPL_WHOISUSER 311		//"<nick> <user> <host> * :<real name>"								// 311
#define RPL_WHOISSERVER 312		//"<nick> <server> :<server info>" 								// 312
#define RPL_WHOISOPERATOR 313	//" :is an IRC operator\n"										// 313
#define RPL_WHOISIDLE 317		//" <integer> :seconds idle\n"										// 317
#define RPL_ENDOFWHOIS 318		//" :End of /WHOIS list\n"											// 318
#define RPL_LISTSTART 321		//"Channel :Users  Name\n"											// 321
#define RPL_LIST 322			//"<channel> <# visible> :<topic>"										// 322
#define RPL_NOTOPIC 331			//":No topic is set\n" 												// 331 
#define RPL_TOPIC 332			//"<channel> :<topic>\n"			 									// 332  
#define RPL_VERSION 351			//" :RFC 1459  | May 1993\n"											// 351
#define RPL_NAMREPLY 353		//"<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]" 					// 353
#define RPL_ENDOFNAMES 366		//":End of /NAMES list\n" 											// 366 
#define RPL_INFO 371			//":"																	// 371
#define RPL_MOTD 372			//":- "																	// 372
#define RPL_ENDOFINFO 374		//":End of /INFO list\n"											// 374
#define RPL_MOTDSTART 375		//" Message of the day - \n"										// 375
#define RPL_ENDOFMOTD 376		//":End of /MOTD command\n"											// 376
#define RPL_YOUREOPER 381		//":You are now an IRC operator\n"									// 381
#define RPL_REHASHING 382		//":Rehashing"														// 382
#define RPL_TIME 391			//" :Local time - "														// 391


#if __APPLE__
	#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
	#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif
            //    - При ответе на MOTD-сообщение и MOTD-файл найден, файл
                //  отбражается строка к строке с каждой строкой, не длше80
                //  символов, используя RPL_MOTD-формат ответов. Их следует
                //  разместить между RPL_MOTDSTART (перед RPL_MOTD) и
                //  RPL_ENDOFMOTD (после).

int Server::replyMEss(int reply, User &user, const std::string &str) {
	std::string mess;
	std::stringstream ss;
	ss << reply;
	mess = ":" + inf.serverName + " " + ss.str() + " " + user.getNick() + " ";
	switch (reply)
	{
	case 256:
		mess += inf.serverName + " :Admin name - " + inf.adminName + "\n";
		break ;
	case 257:
		mess += "Location - Kazan, Republic of Tatarstan, Russian Federation\n";
		break ;
	case 259:
		mess += ":Admin email - " + inf.adminEmail + "\n";
		break ;
	case 302:
		mess += ":" + str + "\n";
		break ;
	case 303:
		mess += ":" + str + "\n";
		break ;
	case 311:
		mess += user.getNick() + " " + user.getUser() + " " + user.getHostn() + " " + user.getRealn() + "\n";
		break ;
	case 312:
		mess += user.getNick() + " " + user.getServern() + ":";
		break ;
	case 313:
		mess += user.getNick() + " " + " :is an IRC operator\n";
		break ;
	case 317:
		mess += user.getNick() + " <integer> :seconds idle\n";
		break ;
	case 318:
		mess += user.getNick() + " :End of /WHOIS list\n";
		break ;
	case 321:
		mess = "Channel :Users  Name\n";
		break ;
	case 322:
		mess = "<channel> <# visible> :<topic>";
		break ;
	case 331:
		mess += str + " :No topic is set\n" ;
		break ;
	case 332:
		mess += str + "\n";
		break ;
	case 351:
		mess += inf.srvVersion + ".1 " + inf.serverName + " :RFC 1459  | May 1993\n";
		break ;
	case 352:	//нужно добавить канал вместо звездочки
		mess += str;
		break ;
	case 353:
		mess += str + "\n";
		break ;
	case 366:
		mess += str + " :End of /NAMES list\n";
		break ;
	case 371:
		mess += ":" + str;
		break ;
	case 372:
		mess += ":- " + str + "\n";
		break ;
	case 374:
		mess += ":End of /INFO list\n";
		break ;
	case 375:
		mess += ":- " + inf.serverName + " Message of the day - \n";
		break ;
	case 376:
		mess += ":End of /MOTD command\n"	;
		break ;
	case 381:
		mess += ":You are now an IRC operator\n";
		break;
	case 382:
		mess += CONF_NAME;
		mess += ":Rehashing\n";
		break ;
	case 391:
		mess += inf.serverName + " :Local time - " + str;
	}
	send(user.getFd(), mess.c_str(), mess.size(), 0);
	return 0;
}