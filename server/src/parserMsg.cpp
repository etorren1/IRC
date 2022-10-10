  #include "Server.hpp"
#include <sstream>

int Server::parseMsg(size_t id)
{
	std::string cmdStr = userData[id]->messages[0];
	std::string parsed;	//plase where we put each token after separating this exact token from string
	std::stringstream input(cmdStr);
	std::vector<std::string>::iterator trailingIt;
	std::vector<std::string>::iterator cutIt;

	//check message length
	if (cmdStr.size() > 512)
	{
		std::cout << "message length exceeded allowed value of 512 symbols\n";
		return (1);
	}

	//the loop below split string to tokens separated by space
	while (std::getline(input, parsed, ' '))
	{
		if (!parsed.empty())
		{
			if (msg.prefx.empty() && parsed[0] == ':' && cmdStr[0] == ':')
				msg.prefx = parsed;
			else if (msg.cmd.empty())
				msg.cmd = parsed;
			else
				msg.midParams.push_back(parsed);
		}
	}
	//std::cout << "before trailing check:\n";
	// printStuct();
	// std::cout << "\n";

	//identifying if trailing is presented amond mid parameters
	trailingIt = msg.midParams.begin();
	while (trailingIt != msg.midParams.end())
	{
		if ((*trailingIt)[0] == ':')
			break;
		trailingIt++;
	}

	//saving iterator in vector to the trailing part
	cutIt = trailingIt;

	//rewriting all midparams from vector to trailing attribute (starting from first token with ':')
	while (trailingIt != msg.midParams.end())
	{
		msg.trailing += " " + *trailingIt;
		trailingIt++;
	}

	//removing one spase and ":" symbol at the begining of trailing
	if (!msg.trailing.empty())
	{
		msg.trailing.erase(msg.trailing.begin(), msg.trailing.begin() + 2);
	}

	// removing trailing part from mid params array
	if (!msg.midParams.empty())
		msg.midParams.erase(cutIt, msg.midParams.end());

	msg.paramN = msg.midParams.size() + !msg.trailing.empty();

	//std::cout << "parsed structure:\n";
	// printStuct();
	//std::cout << "\n";

	//check tokents validity
	if (checkMsgFormat(id))
		return(1);

	return (0);
}

int Server::checkMsgFormat( size_t id )
{
	std::string message;
	//rfc1459 2.3 "the command, and the command parameters (of which there may be up to 15)"
	if ((msg.midParams.size() > 14 && !msg.trailing.empty()) || \
	(msg.midParams.size() > 15 && msg.trailing.empty()))
	{
		message = "number of parameters (including trailing) exceeded allowed value of 15\n";
		send(userData[id]->getFd(), message.c_str(), message.size(), 0);
		return (1);
	}
	//rfc1459 2.3.1 "<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>"
	if (msg.cmd.empty() && (!msg.prefx.empty() || !msg.midParams.empty() || !msg.trailing.empty()))
	{
		message = "invalid format of message\n";
		send(userData[id]->getFd(), message.c_str(), message.size(), 0);
		return (1);
	}
	//rfc1459 2.3.1 "<command>  ::= <letter> { <letter> } | <number> <number> <number>"
	if (!(!isAlphaStr(msg.cmd) || (!isDigitStr(msg.cmd) && msg.cmd.size() == 3)))
	{
		message = "invalid format of command\n";
		send(userData[id]->getFd(), message.c_str(), message.size(), 0);
		return (1);
	}
	return (0);
}

void Server::cleanMsgStruct()
{
	msg.prefx.clear();
	msg.cmd.clear();
	msg.midParams.clear();
	msg.trailing.clear();
	msg.paramN = 0;
}

void Server::printStuct()
{
	std::vector <std::string>::iterator beginIt;
	std::vector <std::string>::iterator endIt;
	beginIt = msg.midParams.begin();
	endIt = msg.midParams.end();

	std::cout << "prefx      : |" << msg.prefx << "|\n";
	std::cout << "cmd        : |" << msg.cmd << "|\n";
	for ( ; beginIt != endIt; ++beginIt)
		std::cout << "midParam   : |" << *beginIt << "|\n";
	std::cout << "trailing   : |" << msg.trailing << "|\n";
	std::cout << "paramN     : |" << msg.paramN << "|\n";
}