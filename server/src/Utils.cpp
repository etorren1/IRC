#include "Server.hpp"

std::string	itos( int const & num )
{
	std::stringstream ss;
	ss << num;
	return(ss.str());
}

bool    contains( std::vector<std::string> vect, std::string str )
{
    if (find(vect.begin(), vect.end(), str) != vect.end())
        return (true);
    return (false);
}

bool    contains( std::vector<User*> vect, User * user )
{
    for (std::vector<User*>::iterator it = vect.begin(); it != vect.end(); it++)
        if ((*it)->getNick() == user->getNick())
            return (true);
    return(false);
}

void eraseString( std::vector<std::string> & vect, std::string name )
{
	if (vect.empty())
		return ;
	std::vector<std::string>::iterator it = vect.begin();
	while (*it != name)
	{
		it++;
		if (it == vect.end())
			return ;
	}
	vect.erase(it);
}

void eraseUser( std::vector<User*> & vect, User * user )
{
	if (vect.empty())
		return ;
	std::vector<User*>::iterator it = vect.begin();
	while ((*it)->getNick() != user->getNick())
	{
		it++;
		if (it == vect.end())
			return ;
	}
	vect.erase(it);
}

std::vector<std::string> split(std::string str, std::string delimiter)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> res;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (!str.empty())
        res.push_back(str);
    return (res);
}

bool isAlphaStr(std::string str)
{
	std::string::iterator beginIt;
	std::string::iterator endIt;

	beginIt = str.begin();
	endIt = str.end();

	for ( ; beginIt != endIt; ++beginIt)
	{
		if (!std::isalpha(*beginIt))
			return (1);
	}
	return(0);
}

bool isDigitStr(std::string str)
{
	std::string::iterator beginIt;
	std::string::iterator endIt;

	beginIt = str.begin();
	endIt = str.end();

	for ( ; beginIt != endIt; ++beginIt)
	{
		if (!std::isdigit(*beginIt))
			return (1);
	}
	return(0);
}

bool isNumStr(std::string str)
{
	std::string::iterator beginIt;
	std::string::iterator endIt;

	beginIt = str.begin();
	endIt = str.end();

	for ( ; beginIt != endIt; ++beginIt)
	{
		if (!(std::isdigit(*beginIt) || *beginIt == '-'))
			return (1);
	}
	return(0);
}

int checkWildcard(const char *str, const char *mask)
{
	const char *strIt = str;
	const char *maskIt = mask;

	while (*maskIt)
	{
		if (*maskIt == '?')
		{
			if (!*strIt)
				return (0);
			strIt++;
			maskIt++;
		}
		else if (*maskIt == '*')
		{
			if (checkWildcard(strIt, maskIt + 1))
				return (1);
			if (*strIt && checkWildcard(strIt + 1, maskIt))
				return(1);
			return (0);
		}
		else
		{
			if (*strIt++ != *maskIt++)
				return (0);
		}
	}
	return (!*strIt && !*maskIt);
}

bool onlyWildcard(std::string str)
{
	return (str.find_first_not_of(str[0]) == std::string::npos && str[0] == '*');
}