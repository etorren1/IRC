#include "Bot.hpp" 

void Bot::create( void )
{
	struct protoent	*pe;

    pe = getprotobyname("tcp");
    if ((srvSock = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
	srvPoll.fd = srvSock;
    srvPoll.events = POLLIN;
    srvPoll.revents = 0;
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(srvPort);
	if(inet_pton(AF_INET, srvHost.c_str(), &srvAddr.sin_addr) <= 0) {
        perror("inet_pton error occured");
        exit(EXIT_FAILURE);
    }
    if( connect(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) < 0) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }
	std::cout << GREEN << "Connected\n" << RESET;
}

void Bot::registration( void )
{
	std::string reg;
	reg = "PASS :" + srvPass + "\r\nNICK " + nick + "\r\nUSER " + user + "\r\n";
	send(srvSock, reg.c_str(), reg.size(), 0);
}

void Bot::run( void )
{
	registration();
	while (true)
	{
		if (handleRequest() & BREAKCONNECT) {
			breakTime = time(0);
			break ;
		}
		spam();
	}
	if (motd)
		reconnect();
	std::cout << "exit\n";
}

void Bot::reconnect( void )
{
	struct protoent	*pe;

	close(srvSock);
    pe = getprotobyname("tcp");
	std::cout << YELLOW << "Reconnecting...\n" << RESET;
    if ((srvSock = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
	srvPoll.fd = srvSock;
    srvPoll.events = POLLIN;
    srvPoll.revents = 0;
	if( connect(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) < 0) {
        sleep(5);
		if (time(0) - breakTime > 31) {
			std::cerr << RED << "Reconnect failed: Time out\n" << RESET;
			exit(EXIT_FAILURE);
		}
        reconnect();
    }
	std::cout << GREEN << "Reconnecting succses\n" << RESET;
	run();
}

int Bot::handleRequest( void ) {
    int ret = poll(&srvPoll, 1, 0);
    if (ret != 0)    {
		if (srvPoll.revents & POLLIN) {
			if (readRequest() <= 0)
				return (BREAKCONNECT);
			else
				replyRequset();
			srvPoll.revents = 0;
		}
    }
	return (0);
}

int Bot::readRequest( void ) {
	char buf[BUF_SIZE + 1];
	int rd;
	int bytesRead = 0;
	std::string text;
    if (messages.size() > 0)
		text = messages.front();
	while ((rd = recv(srvPoll.fd, buf, BUF_SIZE, 0)) > 0) {
		buf[rd] = 0;
		text += buf;
		bytesRead += rd;
		if (text.find("\n") != std::string::npos)
			break;
	}
	while (text.find("\r") != std::string::npos)      // Удаляем символ возврата карретки
        text.erase(text.find("\r"), 1);               // из комбинации CRLF
    messages = split(text, "\n");
	return (bytesRead);
}

Bot::Bot( std::string const & host, std::string const & port, std::string const & pass ) {
	try  {
        if (port.find_first_not_of("0123456789") != std::string::npos)
            throw std::invalid_argument("Port must contain only numbers");
        srvPort = atoi(port.c_str());
        if (srvPort < 1000 || srvPort > 65555) // надо взять правильный рендж портов...
            throw std::invalid_argument("Port out of range");
    }
	catch ( std::exception & e) {
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
	initMap();
	pingTime = 0;
	srvPass = pass;
	srvHost = host;
	if (host == "localhost")
		srvHost = "127.0.0.1";
	motd = false;
	nick = "termUI";
	user = "andrev bot.21.school.ru ecole.42.fr :Andrev Bot";
}

Bot::~Bot() {
}
