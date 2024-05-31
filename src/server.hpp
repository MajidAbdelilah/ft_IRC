#include <sys/socket.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <iostream>
#include <poll.h> //-> for poll()
#include <vector> //-> for vector
#include <sys/types.h> //-> for socket()
#include <fcntl.h> //-> for fcntl()
#include <csignal> //-> for signal()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <unistd.h> //-> for close()
//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
//-------------------------------------------------------//

class IrcCommand
{
	public:
	std::string command;
	std::string prefix;
	std::vector<std::string> params;

	IrcCommand(std::string cmd, std::string pre, std::vector<std::string> par)
	{
		command = cmd;
		prefix = pre;
		params = par;
	}

	IrcCommand()
	{
		command = "";
		prefix = "";
		params.clear();
	}
	~IrcCommand()
	{
		params.clear();
	}
};


class Client //-> class for client
{
private:
	int Fd; //-> client file descriptor
	std::string IPadd; //-> client ip address
	bool pass_accepted; //-> boolean for password accepted
	std::vector<IrcCommand> commands;
public:
	Client(){pass_accepted = false;}; //-> default constructor
	int GetFd() const {return Fd;}
	void addCommand(IrcCommand cmd) {commands.push_back(cmd);}
	std::vector<IrcCommand> getCommands() {return commands;}
	void clearCommands() {commands.clear();}
	void SetFd(int fd) {Fd = fd;}
	void setIpAdd(std::string ipadd) {IPadd = ipadd;}
	IrcCommand parseIrcResponse(std::string cmd);
	void printCommands();

};

class Server //-> class for server
{
private:
	int Port; //-> server port
	int SerSocketFd; //-> server socket file descriptor
	static bool Signal; //-> static boolean for signal
	std::vector<Client> clients; //-> vector of clients
	std::vector<struct pollfd> fds;
public:
	Server(){SerSocketFd = -1;}
	void handleIrcServerResponse(Client client, std::string command); //-> handle irc server response

	void ServerInit(); //-> server initialization
	void SerSocket(); //-> server socket creation
	void AcceptNewClient(); //-> accept new client
	void ReceiveNewData(int fd); //-> receive new data from a registered client
	Client getClient(int fd); //-> get the client by file descriptor
	static void SignalHandler(int signum); //-> signal handler
 
	void CloseFds(); //-> close file descriptors
	void ClearClients(int fd); //-> clear clients

	
};