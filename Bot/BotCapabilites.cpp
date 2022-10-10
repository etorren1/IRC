#include "Bot.hpp"

void Bot::processCommand( void )
{
    if (msg.cmd == "PRIVMSG") {
        std::string nick = msg.prefix.substr(1, msg.prefix.find("!") - 1);
		if (msg.trailing.find("!bot") == 1)
		{
			if (msg.trailing.find("/") != std::string::npos) {
				cmds = msg.trailing.substr(msg.trailing.find("/"));
			}
			else
				cmds = "error";	
			execute(nick);
		}
    }
	else if (msg.cmd == "PING") {
		msg.prefix.erase(msg.prefix.begin());
		sendMessage("PONG", " :" + msg.prefix);
	}
    else if (msg.cmd == "INVITE") {
		if (msg.midParams.size() > 2)
        	sendMessage("JOIN", " ", msg.midParams[1], " ", msg.midParams[2]);
		else
        	sendMessage("JOIN", " ", msg.midParams[1]);
		channels.push_back(msg.midParams[1]);
    }
}

void Bot::processReply( void )
{
    switch (msg.number) {
        case 331:
            sendMessage("TOPIC", " ", msg.midParams[1], " :Now this channel under Bot's control!");
            break ;
    }
}

void Bot::processError( void )
{
	;
}

void Bot::replyRequset( void )
{
	clearMessage();
	parseMessage();
	if (msg.type & COMMAND)
		processCommand();
	else if (msg.type & REPLY)
		processReply();
	else if (msg.type & ERROR)
		processError();
	messages.erase(messages.begin());
	if (!messages.empty())
		replyRequset();
}

void Bot::clearMessage( void )
{
	msg.number = 0;
	msg.cmd.clear();
	msg.prefix.clear();
	msg.trailing.clear();
	msg.midParams.clear();
}

void Bot::spam( void )
{
	// if (time(0) - pingTime > 30)
	// {
	// 	pingTime = time(0);
	// 	std::cout << "What a nice day!\n";
	// }
	if (time(0) - pingTime > 120)
	{
		pingTime = time(0);
		for (size_t i = 0; i < channels.size(); i++)
			sendMessage("PRIVMSG", " ", channels[i], " :Hi, u can ask me about IRC commands. Use '!bot /COMMAND' to more info.");
	}
}

void Bot::parseMessage( void )
{
	size_t it;
	msg.type = 0;
	if ((it = messages[0].rfind(":")) != std::string::npos && it != 0) {
		msg.trailing = messages[0].substr(it, messages[0].size() - it);
		messages[0].erase(it, messages[0].size() - it);
	}
	std::vector<std::string> args = split(messages[0], " ");
	if (args.size() < 2) {
		msg.type |= BROKEN;
		return ;
	}
	msg.prefix = args[0];
	if (isDigit(args[1])) {
		msg.number = atoi(args[1].c_str());
		msg.number > 400 ? msg.type |= ERROR : msg.type |= REPLY; 
	}
	else {
		msg.cmd = args[1];
		msg.type |= COMMAND;
	}
	for (it = 2; it < args.size(); it++ ) {
		msg.midParams.push_back(args[it]);
	}
}

void Bot::sendMessage( std::string const & cmd, std::string const & arg1, std::string const & arg2,\
					   std::string const & arg3, std::string const & arg4, std::string const & arg5)
{
	std::string mess = cmd + arg1 + arg2 + arg3 + arg4 + arg5 + "\r\n";
	int sd = send(srvSock, mess.c_str(), mess.size(), 0);
	std::cout << cmd << " " << mess.size() << " sended: " << sd << std::endl;
}
