#include "Server.hpp"

int Server::ison( User & user )	{
	if (msg.paramN == 0)
		return errorMEss(ERR_NEEDMOREPARAMS, user);
	std::string str;
	for (std::vector<std::string>::iterator it = msg.midParams.begin(); it != msg.midParams.end(); ++it) {
		for (std::vector<User*>::iterator i = userData.begin(); i != userData.end(); ++i)	{
			if (*it == (*i)->getNick())
				str += *it + " ";
		}
	}
	replyMEss(RPL_ISON, user, str);
	return 0;
}

int	Server::userhost( User & user) {
	if (msg.paramN > 5 || msg.paramN < 1)
		return errorMEss(ERR_NEEDMOREPARAMS, user);
	else	{
		std::string str;
		for (std::vector<std::string>::iterator it = msg.midParams.begin(); it != msg.midParams.end(); ++it) {
			for (std::vector<User*>::iterator i = userData.begin(); i != userData.end(); ++i)	{
				if (*it == (*i)->getNick()) {
					str += *it + "=";
					if ((*i)->getFlags() & OPERATOR) str += "*";
					if ((*i)->getFlags() & AWAY) str += "+";
					else str += "-";
					str += (*i)->getIp() + " ";
				}
			}
		}
		replyMEss(RPL_USERHOST, user, str);
	}
	return 0;
}

int Server::away( User & user ) {
	if (msg.trailing.size() > 0 && !(user.getFlags() & AWAY)) {
		user.setAwayMess(msg.trailing);
		user.setFlags(AWAY);
		return replyMEss (306, user);
	}
	else if (user.getFlags() & AWAY) {
		user.unsetFlags(AWAY);
		return replyMEss(305, user);
	}
	return 1;
}