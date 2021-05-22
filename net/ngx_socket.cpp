#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#include "ngx_conf.h"
#include "ngx_socket.h"




bool NGXSocket::ngx_open_listening_sockets()
{
	NGXConfig *p_config = NGXConfig::GetInstance();
	m_ListenPortCount = 2;//p_config->GetIntDefault("ListenPortCount",m_ListenPortCount);

	int		isock;
	struct	sockaddr_in serv_addr;	//structure of server's address
	int 	iport;					//port
	char	strbuf[128];			//for temporary use

	/************** 1 ************** 
	 * Initialization
	 * *****************************/ 
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;					//IPv4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//Listen to all local IP address

	/* Need listen to multiple ports */
	for (int i = 0; i < m_ListenPortCount; i++)
	{
		isock = socket(AF_INET, SOCK_STREAM, 0);
		if (isock == -1) {
			PANIC("NGXSocket::Initialize() socket() failed,port number=%d.",i);
			return false;
		}
	}

	/* Set properties of socket */
	int reuseaddr = 1; //Set SO_REUSEADDR to prevent TIME_WAIT cause bind() failure
	if (setsockopt(isock, SOL_SOCKET, SO_REUSEADDR, (const void*)&reuseaddr, sizeof(reuseaddr)) == -1) {
		PANIC("port number = %d",i);
		close(isock);
		return false;
	}

	/* Set socket as Non-blocking */
	if (set_nonblocking(isock) == false) {
		PANIC("port number = %d",i);
		close(isock);
		return false;
	}

	/* Set Address & Port that server need to listen */
	strbuf[0] = 0;
	sprintf(strbuf, "Listening Port: %d",i);
	iport = p_config
}
