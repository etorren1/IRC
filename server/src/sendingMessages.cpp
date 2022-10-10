#include "Server.hpp"

static int checkMask(std::string str)
{
	std::string::iterator strIt;
	std::string::iterator strEndIt;
	if(str[0] != '#' && str[0] != '$')
		return(1);
	strIt = str.begin() + str.rfind(".");
	strEndIt = str.end();
	while (strIt != strEndIt)
	{
		if (*strIt == '*')
			return (1);
		strIt++;
	}
	return (0);
}

int Server::privmsg( User & user )
{
	std::string mess = msg.trailing;
	std::vector<std::string>::iterator paramIt = msg.midParams.begin();
	std::vector<std::string>::iterator endPramIt = msg.midParams.end();
	std::vector<User *>::iterator userIt = userData.begin();
	std::vector<User *>::iterator endUserIt = userData.end();
	std::map<std::string, Channel *>::iterator chnIt = channels.begin();
	std::map<std::string, Channel *>::iterator endChnIt = channels.end();

	std::vector<User *> chnUsers;
	std::vector<User *> Users;
	bool absenceFlag;

	if (msg.midParams.size() < 1)
		return (errorMEss(ERR_NORECIPIENT, user, "PRIVMSG"));
	if (mess.empty())
		return (errorMEss(ERR_NOTEXTTOSEND, user));
	while (paramIt != endPramIt)
	{
		absenceFlag = 0;
		//for channels:
		if ((*paramIt)[0] == '#' && (*paramIt).find('.') == std::string::npos)
		{
			while (chnIt != endChnIt)
			{
				if ((*paramIt) == chnIt->first)
				{
					if ((chnIt->second->flags & NO_MESS && !chnIt->second->isInChannel(user)) \
						|| (chnIt->second->flags & MODERATE && !chnIt->second->isOperator(&user) \
						&& !chnIt->second->isVoters(&user)))
						errorMEss(ERR_CANNOTSENDTOCHAN, user, chnIt->first);
					else
							showMEss(user, chnIt->second);
					absenceFlag = 1;
				}
				chnIt++;
			}
			if (absenceFlag == 0)
				errorMEss(ERR_NOSUCHNICK, user, *paramIt);
		}
		//for nicks, users, host masks: (server mask isn't checked)
		else
		{
			while (userIt != endUserIt)
			{
				if ((*userIt)->getNick() == *paramIt || (*userIt)->getUser() == *paramIt)
				{
					absenceFlag = 1;
					awayRpl(user, **userIt);
					showMEss(user, **userIt);
				}
				else if(checkMask(*paramIt) && ((*paramIt)[0] == '#' || (*paramIt)[0] == '$') \
						&& checkWildcard((*userIt)->getServer().c_str(), (*paramIt).c_str()))
				{
					absenceFlag = 1;
					awayRpl(user, **userIt);
					showMEss(user, **userIt);
				}
				else if(((*paramIt)[0] == '#' || (*paramIt)[0] == '$') \
					&& onlyWildcard((*paramIt).substr(1)) && (*userIt)->getFlags() & OPERATOR)
				{
					absenceFlag = 1;
					Users = userData;
					for (size_t i = 0; i < Users.size(); i++)
					{
						awayRpl(user, *(Users[i]));
						showMEss(user, *(Users[i]));
					}
				}
				userIt++;
			}
			if (absenceFlag == 0)
				errorMEss(ERR_NOSUCHNICK, user, *paramIt);
		}
		paramIt++;
	}
	return(0);
}

int Server::notice( User & user )
{
	std::string mess = msg.trailing;
	std::vector<std::string>::iterator paramIt = msg.midParams.begin();
	std::vector<std::string>::iterator endPramIt = msg.midParams.end();
	std::vector<User *>::iterator userIt = userData.begin();
	std::vector<User *>::iterator endUserIt = userData.end();
	std::map<std::string, Channel *>::iterator chnIt = channels.begin();
	std::map<std::string, Channel *>::iterator endChnIt = channels.end();

	std::vector<User *> chnUsers;
	std::vector<User *> Users;
	bool absenceFlag;

	if (msg.midParams.size() < 1)
		return (1);
	if (mess.empty())
		return (1);
	while (paramIt != endPramIt)
	{
		absenceFlag = 0;
		//for channels:
		if ((*paramIt)[0] == '#' && (*paramIt).find('.') == std::string::npos)
		{
			while (chnIt != endChnIt)
			{
				if ((*paramIt) == chnIt->first)
				{
					if (!((chnIt->second->flags & NO_MESS && !chnIt->second->isInChannel(user)) \
						|| (chnIt->second->flags & MODERATE && !chnIt->second->isOperator(&user) \
						&& !chnIt->second->isVoters(&user))))
						showMEss(user, chnIt->second);
					absenceFlag = 1;
				}
				chnIt++;
			}
		}
		//for nicks, users, host masks: (server mask isn't checked)
		else
		{
			while (userIt != endUserIt)
			{
				if ((*userIt)->getNick() == *paramIt || (*userIt)->getUser() == *paramIt)
					showMEss(user, **userIt);
				else if(checkMask(*paramIt) && ((*paramIt)[0] == '#' || (*paramIt)[0] == '$') \
						&& checkWildcard((*userIt)->getServer().c_str(), (*paramIt).c_str()))
					showMEss(user, **userIt);
				else if(((*paramIt)[0] == '#' || (*paramIt)[0] == '$') \
					&& onlyWildcard((*paramIt).substr(1)) && (*userIt)->getFlags() & OPERATOR)
				{
					Users = userData;
					for (size_t i = 0; i < Users.size(); i++)
						showMEss(user, *(Users[i]));
				}
				userIt++;
			}
		}
		paramIt++;
	}
	return(0);
}