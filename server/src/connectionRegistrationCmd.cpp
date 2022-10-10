#include "Server.hpp"

bool Server::validNick() {
	std::string str = msg.midParams[0];
	std::string::iterator it = str.begin();
	if (!std::isalpha(*it))
		return false;
	for ( ; it != str.end(); ++it)	{
		if (!std::isalnum(*it) && *it != '-' && (*it != '[' \
			&& *it != ']' && *it != '\\' && *it !=  '`' \
			&& *it != '^' && *it != '{' && *it !=  '}')) {
				return false;
			}
	}
	return true;
}

int Server::pass(User &user) {
	if (msg.midParams.size() == 1 || !msg.trailing.empty()) {
		if (user.getFlags() & REGISTRED)
			return errorMEss(ERR_ALREADYREGISTRED, user);
		else if (user.getPass().empty() == true) {
			if (msg.trailing.empty())
				user.setPass(msg.midParams[0]);
			else
				user.setPass(msg.trailing);
			if (user.getPass() != srvPass)
				return killUser(user);
			if (!user.getNick().empty() && !user.getUser().empty())
				user.setFlags(REGISTRED);
			return connection(user);
		}
	}
	else if (msg.midParams.size() < 1)
		return errorMEss(ERR_NEEDMOREPARAMS, user);
	return 0;
}

int Server::user(User &user){
	if (!((msg.midParams.size() == 3 && !msg.trailing.empty()) \
	|| (msg.midParams.size() == 4 && msg.trailing.empty())))
		return errorMEss(ERR_NEEDMOREPARAMS, user);
	else if (user.getFlags() & REGISTRED)
		return errorMEss(ERR_ALREADYREGISTRED, user);
	else if (user.getUser().empty() && user.getHost().empty() && \
			user.getServer().empty() && user.getReal().empty()) {
		user.setUser(msg.midParams[0]);
		user.setHost(msg.midParams[1]);
		user.setServer(msg.midParams[2]);
		if (msg.trailing.empty())
			user.setReal(msg.midParams[3]);
		else
			user.setReal(msg.trailing);
		if (!user.getPass().empty() && !user.getNick().empty())
			user.setFlags(REGISTRED);
		return connection(user);
	}
	return 0;
}

int Server::nick(User &user) {
	if (msg.midParams.size() == 1) {
		for (size_t i = 0; i < userData.size(); ++i) {
			if (userData[i]->getNick() == msg.midParams[0])
				return errorMEss(ERR_NICKCOLLISION, user);
		}
		if (user.getNick() == msg.midParams[0]) {
			return errorMEss(ERR_NICKNAMEINUSE, user);
		}
		else if (msg.midParams[0].size() > 9 || validNick() == false) {
			return errorMEss(ERR_ERRONEUSNICKNAME, user);
		}
		else {
			if (user.getNick().empty() == true ) {
				user.setNick(msg.midParams[0]);
				if (!user.getPass().empty() && !user.getUser().empty())
					user.setFlags(REGISTRED);
				return connection(user);
			}
			else if (user.getNick().empty() == false ) {
				history.push_back(new User(user));
				user.setNick(msg.midParams[0]);
			}
		}
	}
	else {
		if (msg.midParams.size() < 1 )
			return errorMEss(ERR_NONICKNAMEGIVEN, user);
	}
	return 0;
}

int Server::oper(User &user) {
	if (msg.midParams.size() < 2)
		return errorMEss(ERR_NEEDMOREPARAMS, user);
	for (std::map<std::string, std::string>::iterator it = inf.oper.begin(); it != inf.oper.end(); ++it)	{
		if (msg.midParams[0] == (*it).first) {
		 	if (sha256(msg.midParams[1]) == (*it).second) {
				user.setFlags(OPERATOR);
				return replyMEss(RPL_YOUREOPER, user);
			}
			else if (sha256(msg.midParams[1]) != (*it).second)
				return errorMEss(ERR_PASSWDMISMATCH, user);
		}
		if (it == inf.oper.end() && msg.midParams[0] == (*it).first)
			return errorMEss(ERR_NOOPERHOST, user);
	}
	return 0;
}

int Server::quit(User &user){
	history.push_back(new User(user));
	if (!msg.trailing.empty())
		(*(history.end() - 1))->setQuitMess(msg.trailing);
	return killUser(user);
}

int Server::motd(User &user) {
	std::ifstream infile("conf/ircd.motd");
	if (infile) {
		std::string message;
		replyMEss(RPL_MOTDSTART, user, message);
		while (std::getline(infile, message))
			replyMEss(RPL_MOTD, user, message);
		replyMEss(RPL_ENDOFMOTD, user, message);
	}
	else
		return errorMEss(ERR_NOMOTD, user);
	return 0;
}

int Server::connection(User &user) {
	if (user.getFlags() & REGISTRED){
		user.setTime();
		return motd(user);
	}
	return -1;
}

bool Server::notRegistr(User &user) {
	if (!(user.getFlags() & REGISTRED) && (msg.cmd != "USER" && msg.cmd != "PASS" && msg.cmd != "NICK" && msg.cmd != "QUIT")) {
		errorMEss(ERR_NOTREGISTERED, user);
		return true;
	}
	return false;
}