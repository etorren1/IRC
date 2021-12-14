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
        try
        {
            Channel * current = channels.at(channellist[i]);
            if (contains(user.getChannelList(), channellist[i]))
            {
                if (i == 0 && user.getActiveChannel() != channellist[i])
                    user.setActiveChannel(channellist[i]);
                std::cout << user.getNick() << " alreday on " << channellist[i] << "\n";
            }
            else
            {
                current->addUser(&user);
                user.addChannel(channellist[i]);
                if (i == 0 && user.getActiveChannel() != channellist[i])
                    user.setActiveChannel(channellist[i]);
                std::cout << user.getNick() << " connect to channel " << channellist[i] << "\n";
                showMEss(user, current, 1);
                if (current->getTopic().empty())
                    replyMEss(RPL_NOTOPIC, user, channellist[i]);
                else
                    replyMEss(RPL_TOPIC, user, channellist[i] + " :" + current->getTopic());
                std::string list = channellist[i] + " :";
                std::vector<User *> users = current->getUserList();
                for (size_t i = 0; i < users.size(); i++)
                    list += users[i]->getNick() + " ";
                replyMEss(RPL_NAMREPLY, user, list);
                replyMEss(RPL_ENDOFNAMES, user, channellist[i]);
            }
        }
        catch (std::exception & e)
        {
            if (i < channelpass.size())
                channels[channellist[i]] = new Channel(&user, channellist[i],  channelpass[i]);
            else
                channels[channellist[i]] = new Channel(&user, channellist[i]);
            user.addChannel(channellist[i]);
            user.imOper(channellist[i]);
            if (user.getActiveChannel() != channellist[i])
                user.setActiveChannel(channellist[i]);
            std::cout << user.getNick() << " created new channel " << channellist[i] << "\n";
            showMEss(user, channels[channellist[i]], 1);
            replyMEss(RPL_NOTOPIC, user, channellist[i]);
            replyMEss(RPL_NAMREPLY, user, channellist[i] + " :" + user.getNick());
            replyMEss(RPL_ENDOFNAMES, user, channellist[i]);
        }
    }
    return (0);
}

int Server::part( User & user )
{
    if (msg.midParams.size() > 1 || msg.midParams.empty())
        return(errorMEss(ERR_NEEDMOREPARAMS, user)); // ERR_NEEDMOREPARAMS
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
                std::cout << user.getNick() << " leave channel " << channellist[i] << "\n";
                if (current->getUserList().empty())
                {
                    channels.erase(channellist[i]);
                    delete current;
                }
            }
            else
                errorMEss(ERR_NOTONCHANNEL, user, channellist[i]);
        }
        else
            errorMEss(ERR_NOSUCHCHANNEL, user, channellist[i]);
    }
    return (0);
} 