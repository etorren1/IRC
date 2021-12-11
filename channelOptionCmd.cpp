#include "Server.hpp"

int Server::list( User & user )
{
    if (msg.midParams.empty())
    {
        std::string list;
        for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            list += it->first + " ";
        }
        list+= "\n";
        send(user.getFd(), list.c_str(), list.size(), 0);
    }
    return (0);
}

int Server::names( User & user )
{
    std::string list;
    std::vector<User *> users;
    if (msg.midParams.empty())
    {
        for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            list = it->first + " :";
            users = (it->second)->getUserList();
            for (size_t i = 0; i < users.size(); i++)
                list += users[i]->getNick() + " ";
            replyMEss(353, user, list);
            list = it->first;
            replyMEss(366, user, list);
        }
    }
    else
    {
        std::vector<std::string> channellist = split(msg.midParams[0], ",");
        for (size_t i = 0; i < channellist.size(); i++)
        {
            if (*(channellist[i].begin()) == '#')
            {
                list = channellist[i] + " :";
                users = channels[channellist[i]]->getUserList();
                for (size_t i = 0; i < users.size(); i++)
                    list += users[i]->getNick() + " ";
                replyMEss(353, user, list);
                list = channellist[i];
                replyMEss(366, user, list);
            }
        }
    }
    return (0);
}

int Server::topic( User & user )
{
    if (msg.midParams.size() < 1)
        errorMEss(461, user); // ERR_NEEDMOREPARAMS
    else
    {
        if (contains(user.getChannelList(), msg.midParams[0]))
        {
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
                std::string top;
                for (size_t i = 1; i < msg.midParams.size(); i++)
                    top += msg.midParams[i] + " ";
                top += msg.trailing;
                current->setTopic(top);
            }
        }
        else
            errorMEss(442, user, msg.midParams[0]); // ERR_NOTONCHANNEL
    }
    return (0);
}

int Server::mode( User & user )
{
    // if (msg.midParams.size() < 2 && msg.trailing.empty())
    //     errorMEss(461, user); // ERR_NEEDMOREPARAMS
    // else
    // {
    //     if (*(msg.midParams[0].begin()) == '#') // channels mode
    //     {
    //         if (size_t it = msg.midParams[1].find_first_not_of("+-opsitnmlbvk") != std::string::npos)
    //             errorMEss(472, user, msg.midParams[1].substr(it, 1)); // ERR_UNKNOWNMODE
    //         try {
    //             Channel * current = channels.at(msg.midParams[0]);
    //             if (!contains(user.getChannelList(), msg.midParams[0]))
    //                 errorMEss(442, user, msg.midParams[0]); // ERR_NOTONCHANNEL
    //             else
    //             {
    //                 if (msg.midParams[1].find("k") != std::string::npos &&\
    //                         msg.midParams[1].find("+") != std::string::npos &&\
    //                             !current->getPass().empty())
    //                             errorMEss(467, user, msg.midParams[0]); // ERR_KEYSET
    //                 else
    //                 {
    //                     current->changeFlags(msg.midParams);
    //                 }
    //             }
    //         }
    //         catch (std::exception & e) {
    //             errorMEss(403, user, msg.midParams[0]); // ERR_NOSUCHCHANNEL
    //         }
    //     }
    //     else // user mode
    //     {
    //         ;
    //     }
    // }
    return(0);
}