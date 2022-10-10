#include "Bot.hpp"

void Bot::execute( std::string & nick ) {
	try    {
    	(this->*(commands.at(cmds)))();
		while (reply.size())	{
			sendMessage("PRIVMSG ", nick, " ", reply[0]);
			reply.erase(reply.begin());
		}
	}
	catch(const std::exception& e)		{
		sendMessage("PRIVMSG ", nick, " :Ilmira dont teach me execute that command. Sorry");
	}
	cmds.clear();
}

void Bot::initMap() {
	commands.insert(std::make_pair("/HELP", &Bot::help));
	commands.insert(std::make_pair("/PASS", &Bot::passCmd));
	commands.insert(std::make_pair("/NICK", &Bot::nickCmd));
	commands.insert(std::make_pair("/USER", &Bot::userCmd));
	commands.insert(std::make_pair("/OPER", &Bot::operCmd));
	commands.insert(std::make_pair("/QUIT", &Bot::quitCmd));
	commands.insert(std::make_pair("/JOIN", &Bot::joinCmd));
	commands.insert(std::make_pair("/PART", &Bot::partCmd));
	commands.insert(std::make_pair("/MODE", &Bot::modeCmd));
	commands.insert(std::make_pair("/TOPIC", &Bot::topicCmd));
	commands.insert(std::make_pair("/NAMES", &Bot::namesCmd));
	commands.insert(std::make_pair("/LIST", &Bot::listCmd));
	commands.insert(std::make_pair("/INVITE", &Bot::inviteCmd));
	commands.insert(std::make_pair("/KICK", &Bot::kickCmd));
	commands.insert(std::make_pair("/VERSION", &Bot::versionCmd));
	commands.insert(std::make_pair("/TIME", &Bot::timeCmd));
	commands.insert(std::make_pair("/ADMIN", &Bot::adminCmd));
	commands.insert(std::make_pair("/INFO", &Bot::infoCmd));
	commands.insert(std::make_pair("/PRIVMSG", &Bot::privmsgCmd));
	commands.insert(std::make_pair("/NOTICE", &Bot::noticeCmd));
	commands.insert(std::make_pair("/WHO", &Bot::whoCmd));
	commands.insert(std::make_pair("/WHOIS", &Bot::whoisCmd));
	commands.insert(std::make_pair("/WHOWAS", &Bot::whowasCmd));
	commands.insert(std::make_pair("/KILL", &Bot::killCmd));
	commands.insert(std::make_pair("/PING", &Bot::pingCmd));
	commands.insert(std::make_pair("/PONG", &Bot::pongCmd));
	commands.insert(std::make_pair("/AWAY", &Bot::awayCmd));
	commands.insert(std::make_pair("/REHASH", &Bot::rehashCmd));
	commands.insert(std::make_pair("/RESTART", &Bot::restartCmd));
	commands.insert(std::make_pair("/USERHOST", &Bot::userhostCmd));
	commands.insert(std::make_pair("/ISON", &Bot::isonCmd));
	commands.insert(std::make_pair("/COMMAND", &Bot::command));
}

int Bot::help() {
	std::string str = ":";
	reply.push_back(str + " You need to register to start working on the server.\
You need to complete the mandatory commands - \e[1mPASS\e[0m, \e[1mNICK\e[0m and \e[1mUSER\e[0m. You cann't starrt working in the server\
until these commands are executed. Call one of the commands /PASS, /NICK or /USER to learn how to execute\
these commands. If you want to see a lost of all commands enter  /COMMAND");
	//std::cout << reply[0].size() << std::endl;
	return 0;
}


int Bot::command() {
		std::string str = ":";
	reply.push_back(str + "List of all IRC commands:");
	reply.push_back(str + RED + " 1. Connection Registration:" + RESET + " - /PASS;- /NICK;- /USER;- /OPER;- /QUIT;");
	reply.push_back(str + RED + " 2. Channel operations:" + RESET + " - /JOIN;- /PART;- /MODE;- /TOPIC;- /NAMES;- /LIST;- /INVITE;- /KICK;");
	reply.push_back(str + RED + " 3. Server queries and commands:"  + RESET + " - /VERSION;- /TIME;- /ADMIN;- /INFO;");
	reply.push_back(str + RED + " 4. Sending messages:"  + RESET + " - /PRIVMSG;- /NOTICE;");
	reply.push_back(str + RED + " 5. User based queries:"  + RESET + " - /WHO;- /WHOIS;- /WHOWAS;");
	reply.push_back(str + RED + " 6. Miscellaneous messages:"  + RESET + " - /KILL;- /PING;- /PONG;");
	reply.push_back(str + RED + " 7. Optionals:" + RESET + " - /AWAY;- /REHASH;- /RESTART;- /USERHOST;- /ISON.");
	return 0;
}

int Bot::passCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mPASS\e[0m   Parameters: <password>" );
	reply.push_back(str + RESET + "The PASS command is used to set a 'connection password'.The\
optional password can and MUST be set before any attempt to register the\
connection is made. Currently this requires that user send a PASS command\
before sending the NICK/USER combination.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mPASS\e[0m secretpasswordhere");
	return 0;
}

int Bot::nickCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mNICK\e[0m   Parameters: <nickname>" );
	reply.push_back(str + RESET + "\e[1mNICK\e[0m message is used to give user a nickname or change the previous one.\
If a \e[1mNICK\e[0m message arrives at a server which already knows about\
an identical nickname for another client, a nickname collision occurs.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mNICK\e[0m Wiz, :WiZ \e[1mNICK\e[0m Kilroy" );
	return 0;
}

int Bot::userCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mUSER\e[0m   Parameters: <username> <hostname> <servername> <realname>" );
	reply.push_back(str + RESET + "This command is used at the beginning of a connection to specify the\
username, hostname, real name and initial user modes of the connecting client. <realname> may contain spaces, and thus\
must be prefixed with a colon.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mUSER\e[0m guest tolmoon tolsun :Ronnie Reagan" );
	return 0;
}

int Bot::operCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mOPER\e[0m   Parameters: <user> <password>");
	reply.push_back(str + RESET + "\e[1mOPER\e[0m " + "authenticates a user as an IRC operator on that server/network.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mOPER\e[0m foo bar" );
	return 0;
}

int Bot::quitCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mQUIT\e[0m   Parameters: [<Quit message>]" );
	reply.push_back(str + RESET + "A client session is ended with a quit message.  The server must close\
the connection to a client which sends a \e[1mQUIT\e[0m" + " message. If a \"Quit \
Message\" is given, this will be sent instead of the default message, the nickname.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mQUIT\e[0m :Gone to have lunch" );
	return 0;
}

int Bot::joinCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mJOIN\e[0m   Parameters: <channel>{,<channel>} [<key>{,<key>}]" );
	reply.push_back(str + RESET + "Makes the client join the channels in the comma-separated list <channels>, specifying the passwords, if needed, in the comma-separated\
list <keys>. If the channel(s) do not exist then they will be created.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mJOIN\e[0m #foobar,	\e[1mJOIN\e[0m #foo fubar, \e[1mJOIN\e[0m #foo,#bar, :WiZ \e[1mJOIN\e[0m #Twilight_zone" );
	return 0;
}

int Bot::partCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command:	\e[1mPART\e[0m   Parameters: <channel>{,<channel>}" );
	reply.push_back(str + RESET + "The \e[1mPART\e[0m message causes the client sending the message to be removed\
from the list of active users for all given channels listed in the parameter string.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mPART\e[0m #twilight_zone, \e[1mPART\e[0m #oz-ops, #group5" );
	return 0;
}

int Bot::modeCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mMODE\e[0m");
	reply.push_back(str + RESET + " The \e[1mMODE\e[0m command is a dual-purpose command in IRC.");
	reply.push_back(str + CYAN + "      Channel modesParameters: <channel> {[+|-]|o|p|s|i|t|n|b|v}[<limit>][<user>][<ban mask>]");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1mMODE\e[0m  #Finnish +im, \e[1mMODE\e[0m #Finnish +o Kilroy, \e[1mMODE\e[0m #Finnish +v Wiz, \e[1mMODE\e[0m #eu-opers +l 10");
	reply.push_back(str + CYAN + "      User modes Parameters: <nickname> {[+|-]|i|w|s|o}");
	reply.push_back(str + GREEN + " Example: " + RESET + "	\e[1m:MODE\e[0m WiZ -w:Angel, \e[1mMODE\e[0m Angel +i, \e[1mMODE\e[0m WiZ -o");
	return 0;
}

int Bot::topicCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mTOPIC\e[0m   Parameters: <channel> [<topic>]" );
	reply.push_back(str + RESET + "The \e[1mTOPIC\e[0m message is used to change or view the topic of a channel.\
The topic for channel <channel> is returned if there is no <topic> given.  If the <topic> parameter is present, the topic for that\
channel will be changed, if the channel modes permit this action.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	:Wiz \e[1mTOPIC\e[0m #test :New topic, \e[1mTOPIC\e[0m #test :another topic, \e[1mTOPIC\e[0m #test");
	return 0;
}

int Bot::namesCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mNAMES\e[0m   Parameters: [<channel>{,<channel>}]" );
	reply.push_back(str + RESET + "By using the " + "\e[1mNAMES\e[0m" + " command, a user can list all nicknames that are\
visible to them on any channel that they can see. Channel names which they can see are those which aren't private (+p) or secret (+s)\
or those which they are actually on.If no <channel> parameter is given, a list of all channels and their occupants is returned.");
	reply.push_back(str + GREEN + " Example: " + RESET + "	" + "\e[1mNAMES\e[0m " + "#twilight_zone,#42, \e[1mNAMES\e[0m" );
	return 0;
}

int Bot::listCmd() {
	std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mLIST\e[0m   Parameters: [<channel>{,<channel>} [<server>]]");
	reply.push_back(str + RESET + "		Lists all channels on the server. If the comma-separated list\
<channels> is given, it will return the channel topics. If <server> is given, the command will be forwarded to <server> for evaluation. ");
	reply.push_back(str + GREEN + " Example: " + RESET + " " + "\e[1mLIST\e[0m " + "#twilight_zone,#42, \e[1mLIST\e[0m");
	return 0;
}

int Bot::inviteCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mINVITE\e[0m   Parameters: <nickname> <channel>");
	reply.push_back(str + RESET + "Invites <nickname> to the channel <channel>. <channel> does not have to exist, but if it does, only members of the channel are allowed\
to invite other clients. If the channel mode i is set, only channel operators may invite other clients.");
	reply.push_back(str + GREEN + " Example: " + RESET + " :Angel \e[1mINVITE\e[0m Wiz #Dust, \e[1mINVITE\e[0m Wiz #Twilight_Zone");
	return 0;
}

int Bot::kickCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mKICK\e[0m   Parameters: <channel> <user> [<comment>]");
	reply.push_back(str + RESET + "Forcibly removes <client> from <channel>. This command may only be issued by channel operators.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mKICK\e[0m #Finnish John :Speaking English, :WiZ \e[1mKICK\e[0m #Finnish John");
	return 0;
}

int Bot::versionCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mVERSION\e[0m   Parameters: [<server>]" );
	reply.push_back(str + RESET + "Returns the version of <server>, or the current server if omitted.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mVERSION\e[0m IRC.1, \e[1mVERSION\e[0m" );
	return 0;
}

int Bot::timeCmd() {
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mTIME\e[0m   Parameters: [<server>]" );
	reply.push_back(str + RESET + "Returns the local time on the current server, or <server> if specified.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mTIME\e[0m IRC.1, \e[1mTIME\e[0m" );
	return 0;
}

int Bot::adminCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mADMIN\e[0m   Parameters: [<server>]");
	reply.push_back(str + RESET + "The admin message is used to find the name of the administrator of\
the given server, or current server if <server> parameter is omitted.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mADMIN\e[0m IRC.1, \e[1mADMIN\e[0m" );
	return 0;
}

int Bot::infoCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mINFO\e[0m   Parameters: [<server>]");
	reply.push_back(str + RESET + "The " + "\e[1mINFO\e[0m" + " command is required to return information which describes\
the server: its version, when it was compiled, the patchlevel, when it was started");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mINFO\e[0m IRC.1, \e[1mINFO\e[0m");
	return 0;
}

int Bot::privmsgCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mPRIVMSG\e[0m   Parameters: <receiver>{,<receiver>} <text to be sent>");
	reply.push_back(str + RESET + "Sends <text to be sent> to <receiver>, which is usually a user or channel.");
	reply.push_back(str + GREEN + " Example: " + RESET + " :Angel \e[1mPRIVMSG\e[0m Wiz :Hello are you receiving this message ?, \e[1mPRIVMSG\e[0m Angel :yes I'm receiving it !receiving it !'u>(768u+1n), \
\e[1mPRIVMSG\e[0m jto@tolsun.oulu.fi :Hello !, \e[1mPRIVMSG\e[0m #*.edu :NSFNet is undergoing work, expect interruptions" );
	return 0;
}

int Bot::noticeCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mNOTICE\e[0m   Parameters: <nickname> <text>" );
	reply.push_back(str + RESET + "This command works similarly to \e[1mPRIVMSG\e[0m, except automatic replies\
must never be sent in reply to \e[1mNOTICE\e[0m messages.");
	return 0;
}

int Bot::whoCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mWHO\e[0m   Parameters: [<name> [<o>]]");
	reply.push_back(str + RESET + "Returns a list of users who match <name>. If the flag \"o\" is given, the server will only return information about IRC operators.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mWHO\e[0m *.fi, \e[1mWHO\e[0m jto* o" );
	return 0;
}

int Bot::whoisCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mWHOIS\e[0m   Parameters: <nickmask>[,<nickmask>[,...]]");
	reply.push_back(str + RESET + "Returns information about the comma-separated list of nicknames masks <nicknames>.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mWHOIS\e[0m wiz" );
	return 0;
}

int Bot::whowasCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mWHOWAS\e[0m   Parameters: <nickname> [<count>]");
	reply.push_back(str + RESET + "Used to return information about a nickname that is no longer in use (due to client disconnection, or nickname changes).");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mWHOWAS\e[0m Wiz");
	return 0;
}

int Bot::killCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mKILL\e[0m   Parameters: <nickname> <comment>");
	reply.push_back(str + RESET + "Forcibly removes <nickname> from the network. This command may only be issued by IRC operators.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mKILL\e[0m David");
	return 0;
}

int Bot::pingCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mPING\e[0m   Parameters: <server1>");
	reply.push_back(str + RESET + "Tests the presence of a connection. A \e[1mPING\e[0m message resultsin a \e[1mPONG\e[0m reply.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mPING\e[0m IRC.1, \e[1mPING\e[0m WiZ");
	return 0;
}

int Bot::pongCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mPONG\e[0m   Parameters: <daemon> [<daemon2>]");
	reply.push_back(str + RESET + "This command is a reply to the \e[1mPING\e[0m command and works in much the same way.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mPONG\e[0m IRC.1" );
	return 0;
}

int Bot::awayCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mAWAY\e[0m   Parameters: [message]");
	reply.push_back(str + RESET + "Provides the server with a message to automatically send in reply to a \e[1mPRIVMSG\e[0m directed at the user, but \
not to a channel they are on. If <message> is omitted, the away status is removed.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mAWAY\e[0m :Gone to lunch:WiZ, \e[1mAWAY\e[0m" );
	return 0;
}

int Bot::rehashCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mREHASH\e[0m   Parameters: None");
	reply.push_back(str + RESET + "Causes the server to re-read and re-process its configuration file(s). This command can only be sent by IRC operators.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mREHASH\e[0m");
	return 0;
}

int Bot::restartCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mRESTART\e[0m   Parameters: None");
	reply.push_back(str + RESET + "The restart message can only be used by an operator to force a server restart itself. This message is optional since it may be viewed as a\
risk to allow arbitrary people to connect to a server as an operator and execute this command, causing (at least) a disruption to service.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mRESTART\e[0m" );
	return 0;
}

int Bot::userhostCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mUSERHOST\e[0m   Parameters: <nickname>{<space><nickname>}");
	reply.push_back(str + RESET + "The \e[1mUSERHOST\e[0m command takes a list of up to 5 nicknames, each\
separated by a space character and returns a list of information about the nicknames specified.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mUSERHOST\e[0m Wiz Michael Marty p");
	return 0;
}

int Bot::isonCmd(){
		std::string str = ":";
	reply.push_back(str + CYAN + "      Command: \e[1mISON\e[0m   Parameters: <nickname>{<space><nickname>}");
	reply.push_back(str + RESET + "Queries the server to see if the clients in the space-separated\
list <nicknames> are currently on the network. The server returns only the nicknames that are on the network in a space-separated\
list. If none of the clients are on the network the server returns an empty list.");
	reply.push_back(str + GREEN + " Example: " + RESET + " \e[1mISON\e[0m phone trillian WiZ jarlek Avalon Angel Monstah" );
	return 0;
}

void Bot::showInfo() {
		std::string str = ":";
	reply.push_back(str + "	Welcome to Chat Bot! This bot is made to help you work with IRC: \
the bot shows you what commands are implemented and what arguments must be entered in order for\
the server to understand and correctly process your request.\
We aren't to code a client and this IRC don't handle server to server communication.\
If you want to see the list of commands supported in this IRC enter /HELP.");
}