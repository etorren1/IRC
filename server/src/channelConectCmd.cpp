#include "Server.hpp"

int Server::join( User & user )
{
    if (msg.midParams.size() > 2 || msg.midParams.empty())
        return(errorMEss(ERR_NEEDMOREPARAMS, user));
    std::vector<std::string> channellist = split(msg.midParams[0], ",");
    std::vector<std::string> channelpass;
    if (msg.midParams.size() == 2)
        channelpass = split(msg.midParams[1], ",");
    for (size_t i = 0; i < channellist.size(); i++)
    {
        if (*(channellist[i].begin()) != '#')
            return(errorMEss(ERR_NOSUCHCHANNEL, user, channellist[i]));
        if ((int)user.getChannelList().size() >= maxChannels)
            return(errorMEss(ERR_TOOMANYCHANNELS, user, channellist[i]));
        try // join to channel
        {
            Channel * current = channels.at(channellist[i]);
            if (current->flags & INVITE && !current->isInvited(&user))
                return(errorMEss(ERR_INVITEONLYCHAN, user, channellist[i]));
            if (current->flags & KEY) {
                if (channelpass.size() <= i)
                    return(errorMEss(ERR_BADCHANNELKEY, user, channellist[i]));
                else if (current->getPass() != channelpass[i])
                    return(errorMEss(ERR_BADCHANNELKEY, user, channellist[i]));
            }
            if (current->isBanned(&user))
                return(errorMEss(ERR_BANNEDFROMCHAN, user, channellist[i]));
            if (current->flags & LIMITS && current->getCountUsers() >= current->getUserLimit())
                return(errorMEss(ERR_CHANNELISFULL, user, channellist[i]));
            if (contains(user.getChannelList(), channellist[i]))
                return 1;
            else
            {
                current->addUser(&user);
                user.addChannel(channellist[i]);
                if (current->isInvited(&user))
                    current->deinvUser(&user);
                showMEss(user, current, "JOIN " + channellist[i], true);
                if (current->getTopic().empty())
                    replyMEss(RPL_NOTOPIC, user, channellist[i]);
                else
                    replyMEss(RPL_TOPIC, user, channellist[i] + " :" + current->getTopic());
                std::string list = channellist[i] + " = :";
                std::vector<User *> users = current->getUserList();
                for (size_t i = 0; i < users.size(); i++)
                {
                    if (current->isOperator(users[i]))
                        list += "@" + users[i]->getNick() + " ";
                    else
                        list += users[i]->getNick() + " ";
                }
                replyMEss(RPL_NAMREPLY, user, list);
                replyMEss(RPL_ENDOFNAMES, user, channellist[i]);
            }
        }
        catch (std::exception & e) // create new channel
        {
            if (i < channelpass.size())
                channels[channellist[i]] = new Channel(&user, channellist[i], channelpass[i]);
            else
                channels[channellist[i]] = new Channel(&user, channellist[i]);
            user.addChannel(channellist[i]);
            user.imOper(channellist[i]);
            showMEss(user, channels[channellist[i]], "JOIN " + channellist[i], true);
            replyMEss(RPL_NOTOPIC, user, channellist[i]);
            replyMEss(RPL_NAMREPLY, user, channellist[i] + " = :@" + user.getNick());
            replyMEss(RPL_ENDOFNAMES, user, channellist[i]);
        }
    }
    return (0);
}

int Server::part( User & user )
{
    if (msg.midParams.empty())
        return(errorMEss(ERR_NEEDMOREPARAMS, user));
    std::vector<std::string> channellist = split(msg.midParams[0], ",");
    for (size_t i = 0; i < channellist.size(); i++)
    {
        if (*(channellist[i].begin()) == '#')
        {
            if (contains(user.getChannelList(), channellist[i]))
            {
                Channel * current = channels.at(channellist[i]);
                user.leaveChannel(channellist[i]);
                current->disconnectUser(&user);
                if (!current->getCountUsers())
                    closeChannel(current);
                showMEss(user, current);
            }
            else
                return(errorMEss(ERR_NOTONCHANNEL, user, channellist[i]));
        }
        else
            return(errorMEss(ERR_NOSUCHCHANNEL, user, channellist[i]));
    }
    return (0);
}

int	Server::invite( User & user )
{
    if (msg.midParams.size() < 2)
        return(errorMEss(ERR_NEEDMOREPARAMS, user));
    try { 
        User & to = getUserByNick(msg.midParams[0]);
        if (!contains(user.getChannelList(), msg.midParams[1]))
            return (errorMEss(ERR_NOTONCHANNEL, user, msg.midParams[1]));
        Channel * current = channels[msg.midParams[1]];
        if (current->flags & INVITE && !contains(user.getOpChannelList(), msg.midParams[1]))
            return (errorMEss(ERR_CHANOPRIVSNEEDED, user, msg.midParams[1]));
        if (contains(to.getChannelList(), msg.midParams[1]))
            return (errorMEss(ERR_USERONCHANNEL, user, msg.midParams[0] + " " + msg.midParams[1]));
        current->invUser(&to);
        replyMEss(RPL_INVITING, user, msg.midParams[1] + " " + msg.midParams[0]);
        showMEss(user, current);
        showMEss(user, to);
        if (to.getFlags() & AWAY)
            replyMEss(RPL_AWAY, user, msg.midParams[0] + " " + to.getAwayMess());
    }
    catch (std::exception & e) { return(errorMEss(ERR_NOSUCHNICK, user, msg.midParams[0])); }
    return (0);
}

int Server::kick( User & user )
{
    if (msg.midParams.size() < 2)
        return(errorMEss(ERR_NEEDMOREPARAMS, user));
    std::vector<std::string> channellist = split(msg.midParams[0], ",");
    std::vector<std::string> userlist = split(msg.midParams[1], ",");
    for (size_t i = 0; i < channellist.size(); i++)
    {
        try {
            Channel * current = channels.at(channellist[i]);
            User * rebel;
            if (!contains(user.getChannelList(), channellist[i]))
                errorMEss(ERR_NOTONCHANNEL, user, channellist[i]);
            else if (!current->isOperator(&user))
                errorMEss(ERR_CHANOPRIVSNEEDED, user, channellist[i]);
            else
                for (size_t j = 0; j < userlist.size(); j++)
                    try 
                    {
                        rebel = &getUserByNick(userlist[j]);
                        rebel->leaveChannel(channellist[i]);
                        current->disconnectUser(rebel);
                        showMEss(user, current, 1);
                    }
                    catch (std::exception & e) { ; }
        }
        catch (std::exception & e) { errorMEss (ERR_NOSUCHCHANNEL, user, channellist[i]); }
    }
    return (0);
}