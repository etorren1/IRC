#include "Server.hpp"

int	Server::version( User & user )	{
	if (msg.midParams.size() != 0 && msg.midParams[0] != inf.serverName)
		return errorMEss(ERR_NOSUCHSERVER, user);
	else if (msg.midParams.size() == 0 || (msg.midParams.size() != 0 && msg.midParams[0] == inf.serverName))
		replyMEss(RPL_VERSION, user);
	return 0;
}

std::string Server::checkTime()	{
    std::time_t result = std::time(0);
    char buffer[32];
    std::strncpy(buffer, std::ctime(&result), 26);
	std::string str(buffer);
	return str;
}

int	Server::time( User & user )	{
	if (msg.midParams.size() != 0 && msg.midParams[0] != inf.serverName)
		return errorMEss(ERR_NOSUCHSERVER, user);
	else if (msg.midParams.size() == 0 || (msg.midParams.size() != 0 && msg.midParams[0] == inf.serverName))	{
		std::string str = checkTime();
		replyMEss(RPL_TIME, user, str);
	}
	return 0;
}

int	Server::admin( User & user )	{
	if (msg.midParams.size() != 0 && msg.midParams[0] != inf.serverName)
		return errorMEss(ERR_NOSUCHSERVER, user);
	else if (msg.midParams.size() == 0 || (msg.midParams.size() != 0 && msg.midParams[0] == inf.serverName))	{
		replyMEss(RPL_ADMINME, user);
		replyMEss(RPL_ADMINLOC1, user);
		replyMEss(RPL_ADMINEMAIL, user);
	}
	return 0;
}

int	Server::info( User & user )	{
	if (msg.midParams.size() != 0 && msg.midParams[0] != inf.serverName)
		return errorMEss(ERR_NOSUCHSERVER, user);
	else if (msg.midParams.size() == 0 || (msg.midParams.size() != 0 && msg.midParams[0] == inf.serverName))	{
		replyMEss(RPL_INFO, user, "Server name - " + inf.serverName + "\n");
		replyMEss(RPL_INFO, user, "Server version - " + inf.srvVersion + "\n");
		replyMEss(RPL_INFO, user, "Patchlevel - 1\n");
		replyMEss(RPL_INFO, user, "Server start time - " + inf.srvStartTime);
		replyMEss(RPL_ENDOFINFO, user);
	}
	return 0;
}
