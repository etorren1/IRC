#include "Server.hpp"

void Server::showMEss( User const & from, Channel const * channel, bool andfrom )
{
    std::string mess = from.getPrefix() + " " + msg.cmd;
    for (size_t i = 0; i < msg.midParams.size(); i++)
        mess += " " + msg.midParams[i];
    if (msg.trailing.empty())
        mess += "\n";
    else
        mess += " :" + msg.trailing + "\n";
    std::vector<User *> users = channel->getUserList();
    for(unsigned int i = 0; i < users.size(); i++)
        if (from.getNick() != users[i]->getNick() || andfrom)
            send(users[i]->getFd(), mess.c_str(), mess.size(), 0);
}

void Server::showMEss( User const & from, User const & to, bool andfrom )
{
    std::string mess = from.getPrefix() + " " + msg.cmd;
    for (size_t i = 0; i < msg.midParams.size(); i++)
        mess += " " + msg.midParams[i];
    if (msg.trailing.empty())
        mess += "\n";
    else
        mess += " :" + msg.trailing + "\n";
    send(to.getFd(), mess.c_str(), mess.size(), 0);
    if (andfrom)
        send(from.getFd(), mess.c_str(), mess.size(), 0);
}

void Server::showMEss( User const & from, Channel const * channel, std::string str, bool andfrom )
{
    std::string mess = from.getPrefix() + " " + str + "\n";
    std::vector<User *> users = channel->getUserList();
    for(unsigned int i = 0; i < users.size(); i++)
        if (from.getNick() != users[i]->getNick() || andfrom)
            send(users[i]->getFd(), mess.c_str(), mess.size(), 0);
}