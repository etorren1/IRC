#include "Bot.hpp"

void Bot::execute( std::string & nick ) {
	try    {
    	(this->*(commands.at(cmds)))();
		while (reply.size())
		{
			sendMessage("PRIVMSG", nick, reply[0]);
			reply.erase(reply.begin());
		}
		// reply.clear();
	}
	catch(const std::exception& e)		{
		sendMessage("PRIVMSG", nick, ":Ilmira dont teach me execute that command. Sorry");
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
	reply.push_back(str + CYAN + "   You need to register to start working on the server.\
You need to complete the mandatory commands - " + RESET + "\e[1mPASS\e[0m, \
\e[1mNICK\e[0m " + CYAN + "and " + RESET + "\e[1mUSER\e[0m. " + CYAN + "You cann't starrt working in the server\
until these commands are executed. Call one of the\
commands " + PURPLE + "/PASS, /NICK" + CYAN + " or " + PURPLE + "/USER" + CYAN + " to learn how to execute\
these commands.   If you want to see a lost of all commands enter " + PURPLE + "/COMMAND" + RESET);
	return 0;
}

int Bot::command() {
		std::string str = ":";
	reply.push_back(str + "List of all IRC commands:" + YELLOW + "1. Connection Registration:" +
	CYAN +	"- /PASS;- /NICK;- /USER;- /OPER;- /QUIT;" + YELLOW + "2. Channel operations:" +
	CYAN + "- /JOIN;- /PART;- /MODE;- /TOPIC;- /NAMES;- /LIST;- /INVITE;- /KICK;" +
	YELLOW + "3. Server queries and commands:" + CYAN + "- /VERSION;- /TIME;- /ADMIN;- /INFO;" +
	YELLOW + "4. Sending messages:" + CYAN + "- /PRIVMSG;- /NOTICE;" + YELLOW +
	"5. User based queries:" + CYAN + "- /WHO;- /WHOIS;- /WHOWAS;" + YELLOW + 
	"6. Miscellaneous messages:" + CYAN + "- /KILL;- /PING;- /PONG;" + YELLOW +
	 "7. Optionals:" + CYAN + "- /AWAY;- /REHASH;- /RESTART;- /USERHOST;- /ISON." + RESET);
	return 0;
}

int Bot::passCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mPASS\e[0m   Parameters: <password>" + CYAN +
"The " + RESET + "PASS " + CYAN + "command is used to set a 'connection password'.The\
optional password can and MUST be set before any attempt to register the\
connection is made.  Currently this requires that user send a " + RESET + "PASS " + CYAN + "command\
before sending the " + RESET + "NICK/USER " + CYAN + "combination." + RESET);
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mPASS\e[0m " + RESET + "secretpasswordhere");
	return 0;
}

int Bot::nickCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mNICK\e[0m   Parameters: <nickname>\e[1mNICK\e[0m " + CYAN +
"message is used to give user a nickname or change the previous one.\
If a " + RESET + "\e[1mNICK\e[0m " + CYAN + "message arrives at a server which already knows about\
an identical nickname for another client, a nickname collision occurs." + RESET);
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mNICK\e[0m " + RESET + "Wiz" + GREEN + ":WiZ \e[1mNICK\e[0m " + RESET + "Kilroy");
	return 0;
}

int Bot::userCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mUSER\e[0m   Parameters: <username> <hostname> <servername> <realname>"\
	+ CYAN + "This command is used at the beginning of a connection to specify the\
username, hostname, real name and initial user modes of the\
connecting client. <realname> may contain spaces, and thus\
must be prefixed with a colon." + RESET);
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mUSER\e[0m " + RESET + "guest tolmoon tolsun :Ronnie Reagan");
	return 0;
}

int Bot::operCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mOPER\e[0m   Parameters: <user> <password>"\
	"\e[1mOPER\e[0m " + CYAN + "authenticates a user as an IRC operator on that server/network.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mOPER\e[0m " + RESET + "foo bar");
	return 0;
}

int Bot::quitCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mQUIT\e[0m   Parameters: [<Quit message>]" + CYAN +
"A client session is ended with a quit message.  The server must close\
the connection to a client which sends a " + RESET + "\e[1mQUIT\e[0m" + CYAN + " message. If a \"Quit \
Message\" is given, this will be sent instead of the default message,\
the nickname.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mQUIT\e[0m " + RESET + ":Gone to have lunch");
	return 0;
}

int Bot::joinCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mJOIN\e[0m   Parameters: <channel>{,<channel>} [<key>{,<key>}]" + CYAN +
   "Makes the client join the channels in the comma-separated list\
<channels>, specifying the passwords, if needed, in the comma-separated\
list <keys>. If the channel(s) do not exist then they will be created.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mJOIN\e[0m " + RESET + "#foobar" + 
GREEN + "\e[1mJOIN\e[0m " + RESET + "#foo fubar" + GREEN + "\e[1mJOIN\e[0m " +
RESET + "#foo,#bar:WiZ " +  GREEN + "\e[1mJOIN\e[0m " + RESET + "#Twilight_zone");
	return 0;
}

int Bot::partCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mPART\e[0m   Parameters: <channel>{,<channel>}" + CYAN +
	"The " + RESET + "\e[1mPART\e[0m" + CYAN + " message causes the client sending the message to be removed\
from the list of active users for all given channels listed in the\
parameter string.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mPART\e[0m " + RESET + "#twilight_zone" 
	+ GREEN + "\e[1mPART\e[0m " + RESET + "#oz-ops, #group5");
	return 0;
}

int Bot::modeCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mMODE\e[0m" + CYAN + "The " + RESET + "\e[1mMODE\e[0m" + CYAN + 
" command is a dual-purpose command in IRC.  It allows both\
usernames and channels to have their mode changed.");
	reply.push_back(str + "      Channel modesParameters: <channel> {[+|-]|o|p|s|i|t|n|b|v}[<limit>][<user>][<ban mask>]" +
CYAN + "The " + RESET + "\e[1mMODE\e[0m" + CYAN + " command is provided so that channel operators may change the\
characteristics of `their' channel.  It is also required that servers\
be able to change channel modes so that channel operators may be created.\
The various modes available for channels are as follows:" +
RESET + " \e[1mo\e[0m" + CYAN + " - give/take channel operator privileges;"
RESET + " \e[1mp\e[0m" + CYAN + " - private channel flag;"
RESET + " \e[1ms\e[0m" + CYAN + " - secret channel flag;"
RESET + " \e[1mi\e[0m" + CYAN + " - invite-only channel flag;"
RESET + " \e[1mt\e[0m" + CYAN + " - topic settable by channel operator only flag;"
RESET + " \e[1mn\e[0m" + CYAN + " - no messages to channel from clients on the outside;"
RESET + " \e[1mm\e[0m" + CYAN + " - moderated channel;"
RESET + " \e[1ml\e[0m" + CYAN + " - set the user limit to channel;"
RESET + " \e[1mb\e[0m" + CYAN + " - set a ban mask to keep users out;"
RESET + " \e[1mv\e[0m" + CYAN + " - give/take the ability to speak on a moderated channel;"
RESET + " \e[1mk\e[0m" + CYAN + " - set a channel key (password).");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mMODE\e[0m " + RESET + "#Finnish +im" + 
GREEN + "\e[1mMODE\e[0m " + RESET + "#Finnish +o Kilroy" + GREEN + "\e[1mMODE\e[0m " + RESET + "#Finnish +v Wiz" +
GREEN + "\e[1mMODE\e[0m " + RESET + "#eu-opers +l 10");
	reply.push_back(str + "      User modesParameters: <nickname> {[+|-]|i|w|s|o}" +
CYAN + "The user " + RESET + "\e[1mMODE\e[0m" + CYAN + "s are typically changes which affect either how the\
client is seen by others or what 'extra' messages the client is sent.\
A user " + RESET + "\e[1mMODE\e[0m" + CYAN + " command may only be accepted if both the sender of the\
message and the nickname given as a parameter are both the same.\
The available modes are as follows:" + 
RESET + " \e[1mi\e[0m" + CYAN + " - marks a users as invisible;"
RESET + " \e[1ms\e[0m" + CYAN + " - marks a user for receipt of server notices;"
RESET + " \e[1mw\e[0m" + CYAN + " - user receives wallops;"
RESET + " \e[1mo\e[0m" + CYAN + " - operator flag.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1m:MODE\e[0m " + RESET + "WiZ -w:Angel " + 
GREEN + "\e[1mMODE\e[0m " + RESET + "Angel +i" + GREEN + "\e[1mMODE\e[0m " + RESET + "WiZ -o");
	return 0;
}

int Bot::topicCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mTOPIC\e[0m   Parameters: <channel> [<topic>]" + CYAN +
	"The " + RESET + "\e[1mTOPIC\e[0m" + CYAN + " message is used to change or view the topic of a channel.\
The topic for channel <channel> is returned if there is no <topic>\
given.  If the <topic> parameter is present, the topic for that\
channel will be changed, if the channel modes permit this action.");
	reply.push_back(str + PURPLE + "Example:" + RESET + ":Wiz " + GREEN + "\e[1mTOPIC\e[0m " + RESET + "#test :New topic" 
	+ GREEN + "\e[1mTOPIC\e[0m " + RESET + "#test :another topic" + GREEN + "\e[1mTOPIC\e[0m " + RESET + "#test");
	return 0;
}

int Bot::namesCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mNAMES\e[0m   Parameters: [<channel>{,<channel>}]" + CYAN +
"By using the " + RESET + "\e[1mNAMES\e[0m" + CYAN + " command, a user can list all nicknames that are\
visible to them on any channel that they can see. Channel names\
which they can see are those which aren't private (+p) or secret (+s)\
or those which they are actually on.If no <channel> parameter is given, a list of all channels and their\
occupants is returned.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mNAMES\e[0m " + RESET + "#twilight_zone,#42" 
	+ GREEN + "\e[1mNAMES\e[0m" + RESET);
	return 0;
}

int Bot::listCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mLIST\e[0m   Parameters: [<channel>{,<channel>} [<server>]]" +
	CYAN + "Lists all channels on the server. If the comma-separated list\
<channels> is given, it will return the channel topics. If <server>\
is given, the command will be forwarded to <server> for evaluation. ");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mLIST\e[0m " + RESET + "#twilight_zone,#42" 
	+ GREEN + "\e[1mLIST\e[0m" + RESET);
	return 0;
}

int Bot::inviteCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mINVITE\e[0m   Parameters: <nickname> <channel>" 
	+ CYAN + "Invites <nickname> to the channel <channel>. <channel> does not have\
to exist, but if it does, only members of the channel are allowed\
to invite other clients. If the channel mode i is set, only channel\
operators may invite other clients.");
	reply.push_back(str + PURPLE + "Example:" + RESET + ":Angel " + GREEN
	+ "\e[1mINVITE\e[0m " + RESET + "Wiz #Dust " 
	+ GREEN + "\e[1mINVITE\e[0m" + RESET + "Wiz #Twilight_Zone");
	return 0;
}

int Bot::kickCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mKICK\e[0m   Parameters: <channel> <user> [<comment>]" + CYAN 
+ "Forcibly removes <client> from <channel>. This command may only be issued by channel operators.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mKICK\e[0m" + RESET + " #Finnish John :Speaking English" +
	RESET + ":WiZ " + GREEN + "\e[1mKICK\e[0m " + RESET + "#Finnish John");
	return 0;
}

int Bot::versionCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mVERSION\e[0m   Parameters: [<server>]" + CYAN 
+ "Returns the version of <server>, or the current server if omitted.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mVERSION\e[0m" + RESET + " IRC.1" +
	GREEN + "\e[1mVERSION\e[0m" + RESET);
	return 0;
}

int Bot::timeCmd() {
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mTIME\e[0m   Parameters: [<server>]" + CYAN 
	+ "Returns the local time on the current server, or <server> if specified.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mTIME\e[0m" + RESET + " IRC.1" +
	GREEN + "\e[1mTIME\e[0m" + RESET);
	return 0;
}

int Bot::adminCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mADMIN\e[0m   Parameters: [<server>]" + CYAN 
	+ "The admin message is used to find the name of the administrator of\
the given server, or current server if <server> parameter is omitted.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mADMIN\e[0m" + RESET + " IRC.1" +
	GREEN + "\e[1mADMIN\e[0m" + RESET);
	return 0;
}

int Bot::infoCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mINFO\e[0m   Parameters: [<server>]" + CYAN 
	+ "The " + RESET + "\e[1mINFO\e[0m" + CYAN + " command is required to return information which describes\
the server: its version, when it was compiled, the patchlevel, when\
it was started");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mINFO\e[0m" + RESET + " IRC.1" +
	GREEN + "\e[1mINFO\e[0m" + RESET);
	return 0;
}

int Bot::privmsgCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mPRIVMSG\e[0m   Parameters: <receiver>{,<receiver>} <text to be sent>" + CYAN 
	+ "Sends <text to be sent> to <receiver>, which is usually a user or channel.");
	reply.push_back(str + PURPLE + "Example:" + RESET + ":Angel " + GREEN + "\e[1mPRIVMSG\e[0m" 
	+ RESET + "Wiz :Hello are you receiving this message ?" +
	GREEN + "\e[1mPRIVMSG\e[0m" + RESET + " Angel :yes I'm receiving it !receiving it !'u>(768u+1n)" + 
	GREEN + "\e[1mPRIVMSG\e[0m" + RESET + " jto@tolsun.oulu.fi :Hello !" +
	GREEN + "\e[1mPRIVMSG\e[0m" + RESET + " #*.edu :NSFNet is undergoing work, expect interruptions");
	return 0;
}

int Bot::noticeCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mNOTICE\e[0m   Parameters: <nickname> <text>" + CYAN
	+ "This command works similarly to " + RESET + "\e[1mPRIVMSG\e[0m" + CYAN + ", except automatic replies\
must never be sent in reply to " + RESET + "\e[1mNOTICE\e[0m" + CYAN + " messages." + RESET);
	return 0;
}

int Bot::whoCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mWHO\e[0m   Parameters: [<name> [<o>]]" + CYAN
	+ "Returns a list of users who match <name>. If the flag \"o\" is\
given, the server will only return information about IRC operators.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mWHO\e[0m" + RESET + " *.fi" +
	GREEN + "\e[1mWHO\e[0m" + RESET + " jto* o");
	return 0;
}

int Bot::whoisCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mWHOIS\e[0m   Parameters: <nickmask>[,<nickmask>[,...]]" + CYAN
	+ "Returns information about the comma-separated list of nicknames masks\
<nicknames>.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mWHOIS\e[0m" + RESET + " wiz");
	return 0;
}

int Bot::whowasCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mWHOWAS\e[0m   Parameters: <nickname> [<count>]" + CYAN
	+ "Used to return information about a nickname that is no longer in\
use (due to client disconnection, or nickname changes).");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mWHOWAS\e[0m" + RESET + " Wiz");
	return 0;
}

int Bot::killCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mKILL\e[0m   Parameters: <nickname> <comment>" + CYAN
	+ "Forcibly removes <nickname> from the network. This command\
may only be issued by IRC operators.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mKILL\e[0m" + RESET + " David");
	return 0;
}

int Bot::pingCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mPING\e[0m   Parameters: <server1>" + CYAN
	+ "Tests the presence of a connection. A " + RESET + "\e[1mPING\e[0m" 
	+ CYAN + " message resultsin a " + RESET + "\e[1mPONG\e[0m" + CYAN + " reply.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mPING\e[0m" + RESET + " IRC.1" +
	GREEN + "\e[1mPING\e[0m" + RESET + " WiZ");
	return 0;
}

int Bot::pongCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mPONG\e[0m   Parameters: <daemon> [<daemon2>]" + CYAN
	+ "This command is a reply to the " + RESET + "\e[1mPING\e[0m" + CYAN +
	" command and works in much the same way.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mPONG\e[0m" + RESET + " IRC.1");
	return 0;
}

int Bot::awayCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mAWAY\e[0m   Parameters: [message]" + CYAN
	+ "Provides the server with a message to automatically send in reply\
to a " + RESET + "\e[1mPRIVMSG\e[0m" + CYAN + " directed at the user, but \
not to a channel they are on.\
If <message> is omitted, the away status is removed.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mAWAY\e[0m" + RESET +
	" :Gone to lunch:WiZ " + GREEN + "\e[1mAWAY\e[0m");
	return 0;
}

int Bot::rehashCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mREHASH\e[0m   Parameters: None" + CYAN
	+ "Causes the server to re-read and re-process its configuration\
file(s). This command can only be sent by IRC operators.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mREHASH\e[0m" + RESET);
	return 0;
}

int Bot::restartCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mRESTART\e[0m   Parameters: None" + CYAN
	+ "The restart message can only be used by an operator to force a server\
restart itself. This message is optional since it may be viewed as a\
risk to allow arbitrary people to connect to a server as an operator\
and execute this command, causing (at least) a disruption to service.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mRESTART\e[0m" + RESET);
	return 0;
}

int Bot::userhostCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mUSERHOST\e[0m   Parameters: \
<nickname>{<space><nickname>}" + CYAN
	+ "The " + RESET + "\e[1mUSERHOST\e[0m" + CYAN + " command takes a list of up to 5 nicknames, each\
separated by a space character and returns a list of information\
about the nicknames specified.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mUSERHOST\e[0m" 
	+ RESET + " Wiz Michael Marty p");
	return 0;
}

int Bot::isonCmd(){
		std::string str = ":";
	reply.push_back(str + "      Command: \e[1mISON\e[0m   Parameters: \
<nickname>{<space><nickname>}" + CYAN
	+ "Queries the server to see if the clients in the space-separated\
list <nicknames> are currently on the network. The server returns\
only the nicknames that are on the network in a space-separated\
list. If none of the clients are on the network the server returns\
an empty list.");
	reply.push_back(str + PURPLE + "Example:" + GREEN + "\e[1mISON\e[0m" 
	+ RESET + " phone trillian WiZ jarlek Avalon Angel Monstah");
	return 0;
}

void Bot::showInfo() {
		std::string str = ":";
	reply.push_back(str + CYAN + "	Welcome to Chat Bot!\
This bot is made to help you work with IRC: \
the bot shows you what commands are implemented\
and what arguments must be entered in order for\
the server to understand and correctly process\
your request.\
We aren't to code a client and this IRC don't  handle\
server to server communication.\
If you want to see the list of commands supported\
in this IRC enter " + PURPLE + "/HELP." + RESET);
}