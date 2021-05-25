#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>		//htons, htonl, transform byte-order
#include <unistd.h>
#include <sys/ioctl.h>		//I/O control, set non-blocking I/O

#include "ngx_conf.h"
#include "ngx_socket.h"
extern "C" {
#include "ngx_string.h"
}

NGXSocket::NGXSocket(){
	m_ListenPortCount = 0;
	return;
}

NGXSocket::~NGXSocket()
{ 	//Free Memory
    std::vector<p_ngx_listen_t>::iterator item;	
	for(item = m_ListenSocketList.begin(); item != m_ListenSocketList.end(); ++item)
	{		
		delete (*item);
	}//end for
	m_ListenSocketList.clear(); 
    return;
}

bool NGXSocket::ngx_open_listening_sockets()
{
	NGX_Config *p_config = NGX_Config::GetInstance();
	m_ListenPortCount = p_config->GetInt("ListenPortCount",m_ListenPortCount);

	int		isock;
	struct	sockaddr_in serv_addr;	//structure of server's address
	int 	iport;					//port
	char	confName[128];			//for temporary use

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
			PANIC("NGXSocket::Initialize() socket() failed");
			return false;
		}


		/* Set properties of socket */
		int reuseaddr = 1; //Set SO_REUSEADDR to prevent TIME_WAIT cause bind() failure
		if (setsockopt(isock, SOL_SOCKET, SO_REUSEADDR, (const void*)&reuseaddr, sizeof(reuseaddr)) == -1) {
			PANIC("");
			close(isock);
			return false;
		}

		/* Set socket as Non-blocking */
		if (set_nonblocking(isock) == false) {
			PANIC("");
			close(isock);
			return false;
		}

		/* Set Address & Port that server need to listen */
		confName[0] = 0;
		sprintf(confName, "ListenPort%d",i);
		iport = p_config->GetInt(confName, 64181);
		serv_addr.sin_port = htons((in_port_t)iport);

		/* Bind Server Address */
		if (bind(isock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		{
			PANIC("");
			close(isock);
			return false;
		}

		/* Begin to listen */
		if (listen(isock, NGX_LISTEN_BACKLOG) == -1) 
		{
			PANIC("");
			close(isock);
			return false;
		}

		//Put into 
		p_ngx_listen_t p_listensockitem = new ngx_listen_t; 
		memset(p_listensockitem,0,sizeof(ngx_listen_t)); 
		p_listensockitem->port = iport;
		p_listensockitem->fd   = isock;

printf("Successfully listen to port %d\n",iport);
		m_ListenSocketList.push_back(p_listensockitem); //add to list
	}//for (int i=0; i<m_ListenPortCount; i++)

	return true;
}

bool NGXSocket::set_nonblocking(int sockfd) 
{    
    int non_block =1; //0: clear, 1: set
    if(ioctl(sockfd, FIONBIO, &non_block) == -1) 	return false;//FIONBIO: set non-blocking I/O
    else									return true;
}

void NGXSocket::ngx_close_listening_sockets()
{
    for(int i = 0; i < m_ListenPortCount; i++) 
    {  
        close(m_ListenSocketList[i]->fd);
printf("Successfully close port %d\n",m_ListenSocketList[i]->port);
    }
    return;
}

