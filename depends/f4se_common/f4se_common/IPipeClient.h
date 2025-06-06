#pragma once

#include "ITypes.h"
#include "IPipeServer.h"

class IPipeClient
{
public:
	IPipeClient();
	virtual ~IPipeClient();

	bool	Open(const char * name);
	void	Close(void);
	
	bool	ReadMessage(UInt8 * buf, UInt32 length);
	bool	WriteMessage(IPipeServer::MessageHeader * msg);

private:
	HANDLE		m_pipe;
	std::string	m_name;
};
