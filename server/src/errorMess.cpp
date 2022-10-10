#include "Server.hpp"

int Server::errorMEss(int err, User &user, const std::string &str) {
	std::string messg;
	messg = ":" + inf.serverName + " " + itos(err) + " " + user.getNick() + " ";
	switch (err) {
	case ERR_NOSUCHNICK:
		messg += str + " :No such nick/channel\n";
		break ;
	case ERR_NOSUCHSERVER:
		messg += msg.midParams[0] + " :No such server\n";
		break ;
	case ERR_NOSUCHCHANNEL:
		messg += str + " :No such channel\n";
		break;
	case ERR_CANNOTSENDTOCHAN:
		messg += str + " :Cannot send to channel\n";
		break;
	case ERR_WASNOSUCHNICK:
		messg += str + " :There was no such nickname\n";
		break;
	case ERR_NOORIGIN:
		messg += " :No origin specified\n";
		break;
	case ERR_NORECIPIENT:
		messg += ":No recipient given " + str + "\n";
		break;
	case ERR_NOTEXTTOSEND:
		messg += ":No text to send\n";
		break;
	case ERR_NOMOTD:
		messg += ":MOTD File is missing\n";
		break ;
	case ERR_NONICKNAMEGIVEN:
		messg += ":No nickname given\n";
		break ;
	case ERR_ERRONEUSNICKNAME:
		messg += user.getNick() + " :Erroneus nickname\n";
		break ;
	case ERR_NICKNAMEINUSE:
		messg += user.getNick() + " :Nickname is already in use\n";
		break ;
	case ERR_NICKCOLLISION:
		messg += user.getNick() +  " :Nickname collision KILL\n";
		break ;
	case ERR_NOTONCHANNEL:
		messg += str + " :You're not on that channel\n";
		break;
	case ERR_USERONCHANNEL:
		messg += str + " :is already on channel\n";
		break;
	case ERR_NOTREGISTERED:
		messg += ":You have not registered\n";
		break ;
	case ERR_NEEDMOREPARAMS:
		messg += msg.cmd + " :Not enough parameters\n";
		break ;
	case ERR_ALREADYREGISTRED:
		messg += ":You may not reregister\n";
		break ;
	case ERR_PASSWDMISMATCH:
		messg += ":Password incorrect\n";
		break ;
	case ERR_TOOMANYCHANNELS:
		messg += str + " :You have joined too many channels\n";
		break ;
	case ERR_KEYSET:
		messg += str + " :Channel key already set\n";
		break ;
	case ERR_CHANNELISFULL:
		messg += str + " :Cannot join channel (+l)\n";
		break ;
	case ERR_INVITEONLYCHAN:
		messg += str + " :Cannot join channel (+i)\n";
		break ;
	case ERR_BANNEDFROMCHAN:
		messg += str + " :Cannot join channel (+b)\n";
		break ;
	case ERR_BADCHANNELKEY:
		messg += str + " :Cannot join channel (+k)\n";
		break ;
	case ERR_UNKNOWNMODE:
		messg += str + " :is unknown mode char to me\n";
		break ;
	case ERR_NOPRIVILEGES:
		messg += " :Permission Denied- You're not an IRC operator\n";
		break ;
	case ERR_CHANOPRIVSNEEDED:
		messg += str + " :You're not channel operator\n";
		break ;
	case ERR_CANTKILLSERVER:
		messg += ":You cant kill a server!\n";
		break ;
	case ERR_NOOPERHOST:
		messg += ":No O-lines for your host\n";
		break ;
	case ERR_UMODEUNKNOWNFLAG:
		messg += ":Unknown MODE flag\n";
		break ;
	case ERR_USERSDONTMATCH:
		messg += ":Cant change mode for other users\n";
		break ;
	case 1000:
		messg += str + "\n";
		break ;
	default:
		messg += ":Unknown command\n";
	}
	send(user.getFd(), messg.c_str(), messg.size(), IRC_NOSIGNAL);
	std::cout << "\e[31mSend error to " << user.getNick() << "< socket " << user.getFd() << " >:\e[0m " << messg;
	return 1;
}