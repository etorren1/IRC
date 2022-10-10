#include "Server.hpp"
#include <iostream>

static std::string extract_value(std::string str)
{
	std::string tmp;
	tmp = std::string(str.begin() + str.find(':') + 3, str.begin() + str.rfind('\"'));
	return(tmp);
}

int		Server::parseConf()
{
	std::vector<std::string> strs;
	std::ifstream input(CONF_PATH);
	std::string tmp;

	if (input.is_open()) {
		while(getline (input, tmp))
			strs.push_back(tmp);
	}
	else {
		std::cout << "Unable to open IRCconf.json file\n";
		return (1);
	}

	inf.serverName = extract_value(strs[1]);
	inf.srvVersion = extract_value(strs[2]);
	inf.adminName = extract_value(strs[3]);
	inf.adminEmail = extract_value(strs[4]);
	inf.oper.insert(std::make_pair("fhyman", extract_value(strs[6])));
	inf.oper.insert(std::make_pair("etorren", extract_value(strs[7])));
	inf.oper.insert(std::make_pair("kmeeseek", extract_value(strs[8])));

	// std::cout << inf.serverName << "\n";
	// std::cout << inf.srvVersion << "\n";
	// std::cout << inf.adminName << "\n";
	// std::cout << inf.adminEmail << "\n";
	// for (std::map<std::string, std::string>::iterator it = inf.oper.begin(); it != inf.oper.end(); ++it )
	// 	std::cout << (*it).first << " " << (*it).second << "\n";
	return (0);
}


