#include "Channel.hpp"
#include "Utils.hpp"

void Channel::addUser( User * user )
{
    users.push_back(user);
    countUsers++;
}

void Channel::disconnectUser( User * user )
{
    eraseUser(users, user);
    eraseUser(operators, user);
    eraseUser(invited, user);
    if (operators.empty())
    {
        operators.push_back(users[0]);
        channelMessage(user, "MODE " + name + " +o " + users[0]->getNick());
    }
    countUsers--;
}

void Channel::addBanMask( std::string & masc ) 
{ 
    banmascs.push_back(masc); 
}

void Channel::deleteBanMasc( std::string masc )
{
    eraseString(banmascs, masc);
}

unsigned int Channel::getCountVisible( void ) const
{
    unsigned int count = 0;
    for (size_t i = 0; i < users.size(); i++)
        if (!(users[i]->getFlags() & INVISIBLE))
            count++;
    return (count);
}

static int fits(std::string str, std::string masc)
{
    if (masc == "*")
        return (1);
    else
    {
        if (masc.find("*") != std::string::npos)
        {
            masc.erase(masc.find("*"));
            if (str.find(masc))
                return (1);
        }
        else
            if (str == masc)
                return (1);
    }
    return (0);
}

bool Channel::isBanned( User * user )
{
    if (contains(banmascs, "*!*@*"))
        return (true);
    for (size_t i = 0; i < banmascs.size(); i++)
    {
        size_t it;
        size_t jt;
        int    res = 0;
        if ((it = banmascs[i].find_last_of("!")) == std::string::npos)
            continue;
        if ((jt = banmascs[i].find_last_of("@")) == std::string::npos)
            continue;
        std::string prefix = banmascs[i].substr(0, it);
        std::string midfix = banmascs[i].substr(it + 1, jt - it - 1);
        std::string suffix = banmascs[i].substr(jt + 1, banmascs[i].size() - jt - 1);
        res += fits(user->getNick(), prefix);
        res += fits(user->getUser(), midfix);
        res += fits(user->getHost(), suffix);
        if (res == 3)
            return (true);
    }
    return (false);
}

void Channel::invUser( User * user ) { invited.push_back(user); }
void Channel::deinvUser( User * user ) { eraseUser(invited, user); }
bool Channel::isInvited( User * user )
{
    if (contains(invited, user))
        return (true);
    return (false);
}

void Channel::opUser( User * user ) { operators.push_back(user); }
void Channel::deopUser( User * user ) { eraseUser(operators, user); }
bool Channel::isOperator( User * user )
{
    if (contains(operators, user))
        return (true);
    return (false);
}

void Channel::voteUser( User * user ) { voters.push_back(user); }
void Channel::devoteUser( User * user ) { eraseUser(voters, user); }
bool Channel::isVoters( User * user )
{
    if (contains(voters, user))
        return (true);
    return (false);
}

void Channel::channelMessage( User * from, std::string const & str, bool andfrom )
{
    std::string mess = from->getPrefix() + " " + str + "\n";
    for(unsigned int i = 0; i < users.size(); i++)
        if (from->getNick() != users[i]->getNick() || andfrom)
            send(users[i]->getFd(), mess.c_str(), mess.size(), 0);
}

void Channel::setPass( std::string & pass ) { password = pass; }
void Channel::setUserLimit( unsigned int limit ) { userLimit = limit; }
void Channel::setTopic( std::string & _topic ) { topic = _topic; }
std::vector<User*> Channel::getUserList( void ) const { return (users); }
std::vector<User*> Channel::getOperList( void ) const { return (operators); }
std::vector<std::string> Channel::getBanMasc( void ) const { return (banmascs); }
std::string Channel::getName( void ) const { return (name); }
std::string Channel::getPass( void ) const { return(password); }
std::string Channel::getTopic( void ) const { return (topic); }
unsigned int Channel::getCountUsers( void ) const { return(countUsers); }
unsigned int Channel::getUserLimit( void ) const { return (userLimit); }

int Channel::isInChannel(User &user)
{
	std::vector<User *>::iterator beginIt = users.begin();
	std::vector<User *>::iterator endIt = users.end();
	while (beginIt != endIt)
	{
		if (*beginIt++ == &user)
			return (1);
	}
	return (0);
}

Channel::Channel(User * creater, std::string channelname, std::string pass)
{
    name = channelname;
    password = pass;
    users.push_back(creater);
    operators.push_back(creater);
    countUsers = 1;
    userLimit = 0;
    topic = "";
    flags = NO_MESS;
    if (!pass.empty())
        flags |= KEY;
} 

Channel::~Channel() {}