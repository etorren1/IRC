#include "Server.hpp"

void Server::create( void ) {
    struct protoent	*pe;

    pe = getprotobyname("tcp");
    if ((srvFd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    int enable = 1;
    if (setsockopt(srvFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }
    srvPoll.fd = srvFd;
    srvPoll.events = POLLIN;
    srvPoll.revents = 0;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(srvPort);
    if (bind(srvFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(srvFd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    fcntl(srvFd, F_SETFL, O_NONBLOCK);
    fcntl(fileno(stdin), F_SETFL, O_NONBLOCK);
    /* 
    / F_SETFL устанавливет флаг O_NONBLOCK для подаваемого дескриптора 
    / O_NONBLOCK устанавливает  режим  неблокирования, 
    / что позволяет при ошибке вернуть чтение другим запросам 
    */
}

void Server::run( void ) {
    while(status & WORKING) {
        connectUsers();
        checkUserConnection();
        clientRequest();
        consoleCommands();
    }
    if (status & RESTART) {
        status = WORKING;
        run();
    }
}

void Server::consoleCommands( void )
{
    char buf[BUF_SIZE + 1];
    int bytesRead = 0;
    int rd;
    std::string text;
    while ((rd = read(fileno(stdin), buf, BUF_SIZE)) > 0)
    {
        buf[rd] = 0;
        text += buf;
        bytesRead += rd;
        std::cout << "text from stdin:" << text;
        if (text.find("\n") != std::string::npos) {
            text.erase(text.find("\n"), 1); 
            break;
        }
    }
    if (bytesRead > 0)
    {
        if (text == "STOP")
        {
            std::cout << YELLOW << "Shutdown server\n" << RESET;
            close(srvFd);
            size_t count = userData.size();
            for (size_t i = 0; i < count; i++)
                killUser(*userData[0]);
            count = history.size();
            for (size_t i = 0; i < count; i++)
                killUser(*history[0]);
            userData.clear();
            userFds.clear();
            channels.clear();
            status = 0;
        }
    }
}

void Server::connectUsers( void ) {
    int new_client_fd;
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);

    int ret = poll(&srvPoll, 1, 0);
    if (ret != 0) {
        if (srvPoll.revents & POLLIN) {
            if ((new_client_fd = accept(srvFd, (struct sockaddr*)&clientaddr, (socklen_t*)&addrlen)) > 0) {
                struct pollfd nw;

                nw.fd = new_client_fd;
                nw.events = POLLIN;
                nw.revents = 0;
                userData.push_back(new User(srvFd, nw.fd, clientaddr));
                userFds.push_back(nw);
                std::cout << "New client on " << new_client_fd << " socket." << "\n";
            }
            srvPoll.revents = 0;
        }
    }
}

void Server::clientRequest( void ) {
    int ret = poll(userFds.data(), userFds.size(), 0);
    if (ret != 0)    {
        for (size_t id = 0; id < userFds.size(); id++) {
            if (userFds[id].revents & POLLIN) {
                if (readRequest(id) <= 0)
                    killUser(*userData[id]);
                else if (!userData[id]->getBreakconnect())
                    executeCommand(id);
                userFds[id].revents = 0;
            }
        }
    }
}

static std::string compMsg(std::vector<std::string> vec) {
    std::string text = "";
    for (size_t i = 0; i < vec.size(); i++)
        text += vec[i] + "\n";
    return text;
}

int  Server::readRequest( size_t const id )
{
    char buf[BUF_SIZE + 1];
    int bytesRead = 0;
    int rd;
    std::string text;
    if (userData[id]->messages.size() > 0)
		text = compMsg(userData[id]->messages);
    if ((rd = recv(userFds[id].fd, buf, BUF_SIZE, 0)) > 0) {
        buf[rd] = 0;
        bytesRead += rd;
        text += buf;
    }
    while (text.find("\r") != std::string::npos)      // Удаляем символ возврата карретки
        text.erase(text.find("\r"), 1);               // из комбинации CRLF
    if (text.size() > 512)
        text.replace(510, 2, "\r\n");
    userData[id]->checkConnection(text);
    userData[id]->messages = split(text, "\n");
    return (bytesRead);
}

void Server::checkUserConnection() {
    for (size_t i = 0; i < userData.size(); ++i) {
        if (userData[i]->getFlags() & REGISTRED) {
            if ((userData[i]->timeChecker() - userData[i]->getLastMessTime()) > inactveTime) {
                std::string mess = ":" + inf.serverName + " PING :" + inf.serverName + "\n";
                send(userData[i]->getFd(), mess.c_str(), mess.size(), IRC_NOSIGNAL);
                userData[i]->setLastMessTime();
                userData[i]->setPingTime();
                userData[i]->setFlags(PING);
            }
            if ((userData[i]->getFlags() & PING) && (userData[i]->timeChecker() - userData[i]->getPingTime()) > responseTime) {
                userData[i]->setFlags(KILL);
                killUser(*userData[i]);
            }
        }
    }
}

void Server::execute(std::string const &com, User &user) {
    try    {
        (this->*(commands.at(com)))( user );
	    user.setLastMessTime();
    }
    catch(const std::exception& e) {
        errorMEss(0, user);
    }
}

void Server::executeCommand( size_t const id ) {
	
    cleanMsgStruct();

    if (!parseMsg(id) && notRegistr(*userData[id]) == false) // autorization

    execute(msg.cmd, *userData[id]);

    if (msg.cmd != "QUIT" && !(status & RESTART))
    {
        if (userData[id]->getNick().empty())
            std::cout << "< socket " << userFds[id].fd << " >: " << userData[id]->messages[0] << "\n";
        else
            std::cout << "< " << userData[id]->getNick() << " >: " << userData[id]->messages[0] << "\n";
        userData[id]->messages.erase(userData[id]->messages.begin());
        if (!userData[id]->messages.empty())
            executeCommand(id);
    }
}

void Server::initCommandMap( void ) {
    commands.insert(std::make_pair("PASS", &Server::pass));
    commands.insert(std::make_pair("NICK", &Server::nick));
    commands.insert(std::make_pair("USER", &Server::user));
    commands.insert(std::make_pair("OPER", &Server::oper));
    commands.insert(std::make_pair("QUIT", &Server::quit));
    commands.insert(std::make_pair("PRIVMSG", &Server::privmsg));
    commands.insert(std::make_pair("AWAY", &Server::away));
    commands.insert(std::make_pair("NOTICE", &Server::notice));
    commands.insert(std::make_pair("WHO", &Server::who));
    commands.insert(std::make_pair("WHOIS", &Server::whois));
    commands.insert(std::make_pair("WHOWAS", &Server::whowas));
    commands.insert(std::make_pair("JOIN", &Server::join));
    commands.insert(std::make_pair("MODE", &Server::mode));
    commands.insert(std::make_pair("TOPIC", &Server::topic));
    commands.insert(std::make_pair("INVITE", &Server::invite));
    commands.insert(std::make_pair("KICK", &Server::kick));
    commands.insert(std::make_pair("PART", &Server::part));
    commands.insert(std::make_pair("NAMES", &Server::names));
    commands.insert(std::make_pair("LIST", &Server::list));
    commands.insert(std::make_pair("PING", &Server::ping));
    commands.insert(std::make_pair("PONG", &Server::pong));
    commands.insert(std::make_pair("ISON", &Server::ison));
    commands.insert(std::make_pair("USERHOST", &Server::userhost));
    commands.insert(std::make_pair("VERSION", &Server::version));
    commands.insert(std::make_pair("INFO", &Server::info));
    commands.insert(std::make_pair("ADMIN", &Server::admin));
    commands.insert(std::make_pair("TIME", &Server::time));
    commands.insert(std::make_pair("REHASH", &Server::rehash));
    commands.insert(std::make_pair("RESTART", &Server::restart));
    commands.insert(std::make_pair("KILL", &Server::kill));


    // init server info-vector 
    servInfo.push_back("Server name - " + inf.serverName + "\n");
	servInfo.push_back("Server version - " + inf.srvVersion + "\n");
	servInfo.push_back("Patchlevel - 1\n");
	servInfo.push_back("Server start time - " + inf.srvStartTime);
}

int Server::killUser( User & user ) {
    std::string str = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp() + " Client exited!\n";
    send(user.getFd(), str.c_str(), str.size(), 0);
    close(user.getFd());
    if (user.getNick().empty())
        std::cout << CYAN << "Socket " << user.getFd() << " gone away.\n" << RESET;
    else
        std::cout << CYAN << user.getNick() << " gone away.\n" << RESET;
    std::vector<std::string> temp = user.getChannelList();
    for (size_t i = 0; i < temp.size(); i++) {
        channels[temp[i]]->disconnectUser(&user);
        user.leaveChannel(temp[i]);
        std::string tmp = "PART " + temp[i];
        showMEss(user, channels[temp[i]], tmp);
        if (!channels[temp[i]]->getCountUsers())
            closeChannel(channels[temp[i]]);
    }
    eraseUser(userData, &user);
    std::vector<struct pollfd>::iterator it = userFds.begin();
    for ( ; it != userFds.end(); it++) {
        if (user.getFd() == (*it).fd) {
            userFds.erase(it);
            break ;
        }
    }
    if (user.getFlags() & REGISTRED)
        delete &user;
    std::cout << GREEN << "User killed succes\n" << RESET;
    return (0);
}

void		Server::closeChannel( Channel * channel ) {
    channels.erase(channel->getName());
    delete channel;
}

bool		Server::isChannel( std::string name ) {
    try {
        channels.at(name);
        return (true);
    }
    catch(const std::exception& e) {
        return (false);
    }
}

User& 		Server::getUserByNick( std::string nick ) {
    std::vector<User*>::iterator it = userData.begin();
    for (; it != userData.end(); it++)
        if ((*it)->getNick() == nick)
            return(*(*it));
    throw std::range_error("No such nick");
}

void	Server::printUserVector(std::vector<User*> users) {
	std::vector<User *>::iterator itBegin = users.begin();
	std::vector<User *>::iterator itEnd = users.end();

	while (itBegin != itEnd)
	{
		std::cout << "nick :" << (*itBegin)->getNick() << "\n";
		std::cout << "user :" << (*itBegin)->getUser() << "\n";
		std::cout << "host :" << (*itBegin)->getHost() << "\n";
		std::cout << "real name :" << (*itBegin)->getReal() << "\n";
		itBegin++;
	}
}

void Server::awayRpl(User &user, User &awayUser)
{
	std::string message;
	if (awayUser.getFlags() & AWAY)
	{
		message = awayUser.getNick() + " " + awayUser.getAwayMess();
		replyMEss(RPL_AWAY, user, message);
	}
}

Server::Server( std::string const & _port, std::string const & _pass) {
    try  {
        if (_port.find_first_not_of("0123456789") != std::string::npos)
            throw std::invalid_argument("Port must contain only numbers");
        srvPort = atoi(_port.c_str());
        if (srvPort < 1000 || srvPort > 65555) // надо взять правильный рендж портов...
            throw std::invalid_argument("Port out of range");
    }
    catch ( std::exception & e) {
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
    parseConf();
    srvPass = _pass;
	msg.paramN = 0;
    status = WORKING;
    inf.srvStartTime = checkTime();
	inf.serverName = "IRC.1";
    inf.srvVersion = "v1.0.0";
    inactveTime = 120;
    responseTime = 60;
    maxChannels = 5;
    initCommandMap();
    std::cout << "Done!\n";
}

Server::~Server() {
    std::cout << "Destroyed.\n";
}
