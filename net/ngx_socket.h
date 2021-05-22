#ifndef __NGX_SOCKET_H__
#define __NGX_SOCKET_H__

struct ngx_listen {
	int		port;
	int		fd;
}ngx_listen_t, *p_ngx_listen_t;

#include <vector>

class NGXSocket
{
public:
	NGXSocket();
	~NGXSocket();

public:
	virtual bool Initialize();

private:
	bool ngx_open_listening_sockets();
	void ngx_close_listening_sockets();
	bool set_nonblocking();

private:
	int							m_ListenPortCount;
	std::vector<p_ngx_listen_t>  m_ListenSocketList;
		
};

#endif //__NGX_SOCKET_H__
