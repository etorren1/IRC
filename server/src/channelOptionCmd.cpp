#include "Server.hpp"

#define CHANNEL 1
#define USER 0

int Server::list( User & user )
{

    std::string list;
    if (msg.midParams.empty())
    {
        std::map<std::string, Channel*>::iterator it = channels.begin();
        replyMEss(RPL_LISTSTART, user);
        for ( ; it != channels.end(); it++)
        {
            if (!(it->second->flags & (PRIVATE|SECRET)) || contains(user.getChannelList(), it->first))
            {
                list = it->first + " " + itos(it->second->getCountVisible()) + " :" + it->second->getTopic();
                replyMEss(RPL_LIST, user, list);
            }
            else if (it->second->flags & PRIVATE && !(it->second->flags & SECRET))
            {
                list = it->first + " Prv";
                replyMEss(RPL_LIST, user, list);
            }
        }
        replyMEss(RPL_LISTEND, user);
    }
    else
    {
        std::vector<std::string> channellist = split(msg.midParams[0], ",");
        replyMEss(RPL_LISTSTART, user);
        for (size_t i = 0; i < channellist.size(); i++)
        {
            try {
                Channel * current = channels.at(channellist[i]);
                if (!(current->flags & (PRIVATE|SECRET)) || contains(user.getChannelList(), channellist[i]))
                {
                    list = channellist[i] + " " + itos(current->getCountVisible()) + " :" + current->getTopic();
                    replyMEss(RPL_LIST, user, list);
                }
                else if (current->flags & PRIVATE && !(current->flags & SECRET))
                {
                    list = channellist[i] + " Prv";
                    replyMEss(RPL_LIST, user, list);
                }
            }
            catch (std::exception & e) { ; }
        }
        replyMEss(RPL_LISTEND, user);
    }
    return (0);
}

int Server::names( User & user )
{
    std::string list;
    std::vector<User *> users;
    std::vector<User *>::iterator it = userData.begin();
    if (msg.midParams.empty())
    {
        for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if (!((it->second)->flags & (PRIVATE|SECRET) && !contains((it->second)->getUserList(), &user)))
            {
                list = it->first + " :";
                users = (it->second)->getUserList();
                for (size_t i = 0; i < users.size(); i++)
                {
                    if (user.getFlags() & OPERATOR || !((it->second)->flags & INVISIBLE)) {
                        if ((it->second)->isOperator(users[i]))
                            list += "@" + users[i]->getNick() + " ";
                        else
                            list += users[i]->getNick() + " ";
                    }
                }
                replyMEss(RPL_NAMREPLY, user, list);
                list = it->first;
                replyMEss(RPL_ENDOFNAMES, user, list);
            }
        }
        list = "* :";
        for ( ; it != userData.end(); it++)
            if ((*it)->getChannelList().empty())
                if (user.getFlags() & OPERATOR || !((*it)->getFlags() & INVISIBLE))
                    list += (*it)->getNick() + " ";
        replyMEss(RPL_NAMREPLY, user, list);
        replyMEss(RPL_ENDOFNAMES, user, "*");
    }
    else
    {
        std::vector<std::string> channellist = split(msg.midParams[0], ",");
        for (size_t i = 0; i < channellist.size(); i++)
        {
            if (*(channellist[i].begin()) == '#')
            {
                try {
                    Channel * current = channels.at(channellist[i]);
                    if (!(current->flags & (PRIVATE|SECRET) && !contains(current->getUserList(), &user)))
                    {
                        list = channellist[i] + " :";
                        users = current->getUserList();
                        for (size_t i = 0; i < users.size(); i++)
                        {
                            if (user.getFlags() & OPERATOR || !((*it)->getFlags() & INVISIBLE)) {
                                if (current->isOperator(users[i]))
                                    list += "@" + users[i]->getNick() + " ";
                                else
                                    list += users[i]->getNick() + " ";
                            }
                        }
                        replyMEss(RPL_NAMREPLY, user, list);
                        list = channellist[i];
                        replyMEss(RPL_ENDOFNAMES, user, list);
                    }
                }
                catch (std::exception & e) {
                    replyMEss(RPL_ENDOFNAMES, user, channellist[i]);
                }
            }
        }
    }
    return (0);
}

int Server::topic( User & user )
{
    if (msg.midParams.size() < 1)
        return (errorMEss(ERR_NEEDMOREPARAMS, user));
    if (!contains(user.getChannelList(), msg.midParams[0]))
        return (errorMEss(ERR_NOTONCHANNEL, user, msg.midParams[0]));
    Channel * current = channels.at(msg.midParams[0]);
    if (msg.midParams.size() == 1 && msg.trailing.empty())
    {
        if (current->getTopic().empty())
            replyMEss(331, user, msg.midParams[0]);
        else
            replyMEss(332, user, msg.midParams[0] + " :" + current->getTopic());
    }
    else
    {
        if (current->flags & TOPIC)
            if (!current->isOperator(&user))
                return(errorMEss(ERR_CHANOPRIVSNEEDED, user, msg.midParams[0]));
        std::string top;
        if (msg.trailing.empty())
            top = msg.midParams[1];
        else
            top = msg.trailing;
        current->setTopic(top);
        showMEss(user, current, true);
    }        
    return (0);
}

static int checkModeChar( std::string str, bool channel)
{
    if (channel) {
        if (str.find_first_not_of("+-opsitnmlbvk") != std::string::npos
        || str.size() != 2 || (str[0] != '-' && str[0] != '+'))
            return (1);
    }
    else {
        if (str.find_first_not_of("+-osiw") != std::string::npos
        || str.size() != 2 || (str[0] != '-' && str[0] != '+'))
            return (1);
    }
    return(0);
}

int Server::mode( User & user )
{
    if (msg.midParams.size() < 2)
        return(errorMEss(ERR_NEEDMOREPARAMS, user));
    if (*(msg.midParams[0].begin()) == '#') // channels mode
    {
        if (checkModeChar(msg.midParams[1], CHANNEL))
            return(errorMEss(ERR_UNKNOWNMODE, user, msg.midParams[1]));
        try {
            Channel * current = channels.at(msg.midParams[0]);
            if (!contains(user.getChannelList(), msg.midParams[0]))
                return(errorMEss(ERR_NOTONCHANNEL, user, msg.midParams[0]));
            else if (!current->isOperator(&user))
                return(errorMEss(ERR_CHANOPRIVSNEEDED, user, msg.midParams[0]));
            else
                setChannelMode(current, user);
        }
        catch (std::exception & e) {
            return(errorMEss(ERR_NOSUCHCHANNEL, user, msg.midParams[0]));
        }
    }  
    else // user mode
    {
        if (checkModeChar(msg.midParams[1], USER))
            return(errorMEss(ERR_UMODEUNKNOWNFLAG, user));
        else if (user.getNick() != msg.midParams[0])
            return(errorMEss(ERR_USERSDONTMATCH, user));
        else
            setUserMode(user);
    }
    return(0);
}

void    Server::setUserMode( User & user )
{
    char flag = msg.midParams[1][1];
    if (msg.midParams[1][0] == '-') {
        switch (flag) {
            case 'i': user.unsetFlags(INVISIBLE); break;
            case 'w': user.unsetFlags(WALLOPS); break;
            case 's': user.unsetFlags(SNOTICE); break;
            case 'o': user.unsetFlags(OPERATOR); break;
        }
    }
    else if (msg.midParams[1][0] == '+') {
        switch (flag) {
            case 'i': user.setFlags(INVISIBLE); break;
            case 'w': user.setFlags(WALLOPS); break;
            case 's': user.setFlags(SNOTICE); break;
            case 'o': return ;
        }
    }
    std::string modes = "[+";
    if (user.getFlags() & OPERATOR) modes += "o";
    if (user.getFlags() & INVISIBLE) modes += "i";
    if (user.getFlags() & SNOTICE) modes += "s";
    if (user.getFlags() & WALLOPS) modes += "w";
    replyMEss(RPL_UMODEIS, user, modes + "]");
}

void    Server::setChannelMode(Channel * channel, User & user )
{
    char flag = msg.midParams[1][1];
    if (msg.midParams[1][0] == '-') {
        switch (flag) {
            case 'o':
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                else if (contains(channel->getUserList(), &getUserByNick(msg.midParams[2]))) {
                    channel->deopUser(&getUserByNick(msg.midParams[2]));
                    getUserByNick(msg.midParams[2]).imNotOper(channel->getName());
                    break ; }
                else {
                    errorMEss(502, user); return; }
            case 'v':
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                else if (contains(channel->getUserList(), &getUserByNick(msg.midParams[2]))) {
                    channel->devoteUser(&getUserByNick(msg.midParams[2]));
                    break ; }
                else {
                    errorMEss(502, user); return; }
            case 'b': {
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                channel->deleteBanMasc(msg.midParams[2]);
                std::vector<std::string> mascs = channel->getBanMasc();
                if (!mascs.empty())
                    for (size_t i = 0; i < mascs.size(); i++)
                        replyMEss(RPL_BANLIST, user, msg.midParams[0] + " " + mascs[i]);
                replyMEss(RPL_ENDOFBANLIST, user, msg.midParams[0]);
                break; }
            case 'k': channel->flags &= ~KEY; break;
            case 'l': channel->flags &= ~LIMITS; break;
            case 'p': channel->flags &= ~PRIVATE; break;
            case 's': channel->flags &= ~SECRET; break;
            case 'i': channel->flags &= ~INVITE; break;
            case 't': channel->flags &= ~TOPIC; break;
            case 'n': channel->flags &= ~NO_MESS; break;
            case 'm': channel->flags &= ~MODERATE; break;
        }
    }
    else if (msg.midParams[1][0] == '+') {
        switch (flag){
            case 'o':
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                else if (contains(channel->getUserList(), &getUserByNick(msg.midParams[2]))) {
                    channel->opUser(&getUserByNick(msg.midParams[2]));
                    getUserByNick(msg.midParams[2]).imOper(channel->getName());
                    break ; }
                else {
                    errorMEss(502, user); return; }
            case 'v':
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                else if (contains(channel->getUserList(), &getUserByNick(msg.midParams[2]))) {
                    channel->voteUser(&getUserByNick(msg.midParams[2]));
                    break ; }
                else {
                    errorMEss(502, user); return; }
            case 'b': {
                if (msg.midParams.size() == 2) {
                    std::vector<std::string> mascs = channel->getBanMasc();
                    if (!mascs.empty())
                        for (size_t i = 0; i < mascs.size(); i++)
                            replyMEss(RPL_BANLIST, user, msg.midParams[0] + " " + mascs[i]);
                    replyMEss(RPL_ENDOFBANLIST, user, msg.midParams[0]);
                    return; }
                channel->addBanMask(msg.midParams[2]);
                std::vector<std::string> mascs = channel->getBanMasc();
                if (!mascs.empty())
                    for (size_t i = 0; i < mascs.size(); i++)
                        replyMEss(RPL_BANLIST, user, msg.midParams[0] + " " + mascs[i]);
                replyMEss(RPL_ENDOFBANLIST, user, msg.midParams[0]);
                break; }
            case 'k': channel->flags |= KEY;
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                channel->setPass(msg.midParams[2]);
                break;
            case 'l': channel->flags |= LIMITS;
                if (msg.midParams.size() < 3) {
                    errorMEss(461, user); return; }
                channel->setUserLimit(stoi(msg.midParams[2]));
                break;
            case 'p': channel->flags |= PRIVATE; break;
            case 's': channel->flags |= SECRET; break;
            case 'i': channel->flags |= INVITE; break;
            case 't': channel->flags |= TOPIC; break;
            case 'n': channel->flags |= NO_MESS; break;
            case 'm': channel->flags |= MODERATE; break;
        } 
    }
    std::string modes = channel->getName() + " [+";
    if (channel->flags & NO_MESS) modes += "n";
    if (channel->flags & PRIVATE) modes += "p";
    if (channel->flags & SECRET) modes += "s";
    if (channel->flags & INVITE) modes += "i";
    if (channel->flags & TOPIC) modes += "t";
    if (channel->flags & MODERATE) modes += "m";
    if (channel->flags & KEY) modes += "k";
    if (channel->flags & LIMITS) modes += "l(" + itos(channel->getUserLimit()) + ")";
    replyMEss(RPL_CHANNELMODEIS, user, modes + "]");
    showMEss(user, channel, 1);
}