
#include "SDL_ep.h"

void NetMessageHandler( string message )
{
	
}

////////////////////////# cClient #////////////////////////////////

cClient :: cClient( TCPsocket initSock )
{
	printf( "Server : Client Added\n" );
	
	sock = initSock;
	// get the clients IP and port number
	ip = SDLNet_TCP_GetPeerAddress( sock );
	
	if( !ip )
	{
		printf( "Client Creation SDLNet_TCP_GetPeerAddress Error : %s\n", SDLNet_GetError() );
	}
}

cClient :: cClient( const cClient &Client )
{
	this->sock = Client.sock;
	this->ip = Client.ip;
}

cClient :: ~cClient( void )
{
	// Should be empty here !
}

TCPsocket cClient :: Getsocket( void )
{
	return sock;
}

Uint32 cClient :: GetIP( void )
{
	return ip->host;
	// maybe use : char *SDLNet_ResolveIP(IPaddress *address)
}

Uint16 cClient :: GetPort( void )
{
	return ip->port;
}

////////////////////////# cTCPNet #////////////////////////////////

cTCPNet :: cTCPNet( unsigned int nport /* = 9877  */ )
{
	csocket = NULL;

	ip.host = 0;
	ip.port = 0;

	port = nport;

	state = STATE_IDLE;

	socketset = NULL;
}

cTCPNet :: ~cTCPNet( void )
{
	Shutdown();
}

/**	Starts an Server
 *	returns 0 if already connected or an error
 */
int cTCPNet :: Server_Start( void ) // 2.3
{
	if( state != STATE_IDLE )
	{
		return 0;
	}

	printf( "Server_Start at port %d\n", port );
	
	// check our port
	if(port <= 0 || port > 66000)
	{
		printf( "Server_Start Error : inavlid Port (%d)\n", port );
		return 0;
	}
	
	// Resolve the argument into an IPaddress type
	if( SDLNet_ResolveHost( &ip, NULL, port ) == -1 )
	{
		printf( "Server_Start Error : SDLNet_ResolveHost : %s\n", SDLNet_GetError() );
		return 0;
	}

	// open the server socket
	csocket = SDLNet_TCP_Open( &ip );

	if( !csocket )
	{
		printf( "Server_Start Error : SDLNet_TCP_Open : %s\n", SDLNet_GetError() );
		return 0;
	}

	state = STATE_HOSTING_EMPTY;

	Alloc_Socketset();
	
	return 1;
}

/**	Checks for an connecting request from an remote client
 *	used internally
 */
int cTCPNet :: Server_ConnectionCheck( void ) // 2.1
{
	if( state != STATE_HOSTING_EMPTY && state != STATE_HOSTING_CONNECTED )
	{
		return 0;
	}
	
	if( !csocket || !SDLNet_SocketReady( csocket ) )	// Check the socket
	{
		return 0;
	}

	// try to accept a connection
	TCPsocket nclient = SDLNet_TCP_Accept( csocket );

	if( !nclient ) // no connection accepted
	{
		printf( "Server_ConnectionCheck SDLNet_TCP_Accept Error : %s\n", SDLNet_GetError() );
		return 0;
	}
	
	clients.push_back( nclient );
	
	// print out the clients IP and port number
	Uint32 ipaddr = SDL_SwapBE32( clients[ clients.size() - 1 ].ip->host );
	
	printf("Accepted a connection from %d.%d.%d.%d port %d\n",
		ipaddr>>24, (ipaddr>>16)&0xff, (ipaddr>>8)&0xff, ipaddr&0xff, (int)port );

	state = STATE_HOSTING_CONNECTED;

	if( !Alloc_Socketset() )
	{
		printf( "Server_ConnectionCheck Alloc_Socketset() Error : %s\n", SDLNet_GetError() );
		return 0;
	}

	return 1;
}

/**	Connects to an server
 *	returns 0 if there were no success and 1 on success
 */
int cTCPNet :: Client_Connect( string &address ) // 2.2
{
	if( state != STATE_IDLE )
	{
		return 0;
	}

	printf( "Client_Connect at ip : %s, Port %d\n", address.c_str(), port );

	// check our port
	if( port <= 0 || port > 66000 )
	{
		printf( "Client_Connect inavlid Port %d Error\n", port );
		return 0;
	}
	
	// Resolve the argument into an IPaddress type
	if( SDLNet_ResolveHost( &ip, address.c_str(), port ) == -1 )
	{
		printf( "Client_Connect SDLNet_ResolveHost Error : %s\n", SDLNet_GetError() );
		return 0;
	}

	// open the server socket
	csocket = SDLNet_TCP_Open( &ip );

	if( !csocket )
	{
		printf( "Client_Connect SDLNet_TCP_Open Error : %s\n", SDLNet_GetError() );
		ip.host = 0;
		ip.port = 0;
		return 0;
	}

	state = STATE_CLIENT_CONNECTED;

	if( !Alloc_Socketset() )
	{
		printf( "Client_Connect Alloc_Socketset() Error : %s\n", SDLNet_GetError() );
		return 0;
	}

	Uint32 ipaddr = SDL_SwapBE32( ip.host );
	
	printf( "Succesfully Connected to %d.%d.%d.%d port %d\n",
		ipaddr>>24, (ipaddr>>16)&0xff, (ipaddr>>8)&0xff, ipaddr&0xff, port );
	
	return 1;
}

/**	Reads the Message
 *	Returns 0 if an error occured or not connected to anyone
 */
int cTCPNet :: Read_Message( TCPsocket sock, string &message ) // 2.8
{
	// If state is valid and an message is on the queue
	if( state == STATE_IDLE || state == STATE_HOSTING_EMPTY )
	{
		return 0;
	}
	
	if( !sock || !SDLNet_SocketReady( sock ) )	// Check the socket
	{
		return 0;
	}

	Uint32 len = 0;

	// Receive the length of the string message
	Uint32 result = SDLNet_TCP_Recv( sock, &len, sizeof( len ) );

	if( !result ) // Remote Connection got Closed or Socket Error
	{
		printf("Read_Message Length Reading Error : Got nothing to read\n");

		if( state == STATE_CLIENT_CONNECTED )
		{
			Shutdown();
		}
		else
		{
			Server_DisconnectClient( sock ); // Disconect this Client
		}

		return 0;
	}

	if( result < sizeof( len ) )
	{
		printf( "Read_Message Length Reading Error : %s\n", SDLNet_GetError() );

		if( state == STATE_CLIENT_CONNECTED )
		{
			Shutdown();
		}
		else
		{
			Server_DisconnectClient( sock ); // Disconect this Client
		}

		return 0;
	}

	// swap byte order to our local order
	len = SDL_SwapBE32( len );
	
	// check if anything is strange, like a zero length buffer
	if( !len )
	{
		printf( "Read_Message Error : Zero Length\n" );

		if( state == STATE_CLIENT_CONNECTED )
		{
			Shutdown();
		}
		else
		{
			Server_DisconnectClient( sock ); // Disconect this Client
		}

		return 0;
	}
		
	if( len > MAX_BUFFER_SIZE )
	{
		printf( "Read_Message Error : Too large Buffer\n" );
		return 0;
	}

	char* buf = NULL;

	// allocate the buffer memory
	buf = static_cast<char*>( malloc( len ) );

	// get the string buffer over the socket
	result = SDLNet_TCP_Recv( sock, buf, len );

	if( result < len )
	{
		free( buf );
		buf = NULL;
		
		printf( "Read_Message Buffer Reading Error : %s\n", SDLNet_GetError() );

		if( state == STATE_CLIENT_CONNECTED )
		{
			Shutdown();
		}
		else
		{
			Server_DisconnectClient( sock ); // Disconnect this Client
		}
		
		return 0;
	}

	message = buf;
	
	free( buf );
	buf = NULL;

	printf( "Received : %s\n", message.c_str() );
	return 1; // Succesfull
}

/**	Sends the Message to all Connections you have
 *	uses Send_message if you're an Client
 *	else Server_SenGlobalMessage if you're an Server
 */
void cTCPNet :: Send_Message_Auto( const string message ) // 1.0
{
	if( state == STATE_CLIENT_CONNECTED ) 
	{
		Send_Message( csocket, message );
	}
	else if( state == STATE_HOSTING_CONNECTED ) 
	{
		Server_SendGlobalMessage( message );
	}
}

/**	Sends an Message to ervery Client
 *	
 */
void cTCPNet :: Server_SendGlobalMessage( const string message ) // 1.1
{
	if( state != STATE_HOSTING_CONNECTED )
	{
		return;
	}

	for( std::vector<cClient>::iterator iter = clients.begin(); iter < clients.end(); iter++ )
	{
		if( iter->sock )
		{
			Send_Message( iter->sock, message );
		}
		else
		{
			printf( "Server_SendGlobalMessage Socket Error : Socket is Closed\n" );
			clients.erase( iter );
		}
	}
}

/**	Sends an Message
 *	Returns 0 if an error occured or not connected to anyone
 */
int cTCPNet :: Send_Message( TCPsocket sock, const string message ) // 2.6
{
	if( !state || state == STATE_HOSTING_EMPTY )
	{
		return 0;
	}
	
	Uint32 len, result;

	len = message.size() + 1; // + 1 for the terminating NULL
	
	// change endianness to network order
	len = SDL_SwapBE32( len );

	if( !len )
	{
		printf( "Send_Message Warning : Zero Length Message\n" );
		return 0;
	}
	
	// send the length of the string
	result = SDLNet_TCP_Send( sock, &len, sizeof( len ) );

	if( result < sizeof( len ) ) 
	{
		printf( "Send_Message Length Sending Error : %s\n", SDLNet_GetError() );
		
		if( state == STATE_CLIENT_CONNECTED )
		{
			Shutdown();
		}
		else
		{
			Server_DisconnectClient( sock ); // Disconnect this Client
		}
		
		return 0; // Error
	}
		
	// back to our local byte order
	len = SDL_SwapBE32( len );
	
	// send the Message
	result = SDLNet_TCP_Send( sock, c_strNC( message ), len );

	if( result < len ) 
	{
		printf( "Send_Message Message Sending Error : %s\n", SDLNet_GetError() );

		if( state == STATE_CLIENT_CONNECTED )
		{
			Shutdown();
		}
		else
		{
			Server_DisconnectClient( sock ); // Disconect this CLient
		}
		
		return 0; // Error
	}

	printf( "Sended : %s\n", message.c_str() );
	
	return 1; // Succesfull
}

/**	Closes every Connection
 *	returns 0 if there were no connections or opened sockets
 */
int cTCPNet :: Shutdown( void ) // 2.3
{
	if( state == STATE_IDLE )
	{
		return 0;
	}
	else if( state == STATE_HOSTING_EMPTY )
	{
		printf( "Starting Server Shutdown\n" );

		if( csocket ) 
		{
			SDLNet_TCP_Close( csocket );
		}
		csocket = NULL;
	}
	else if( state == STATE_HOSTING_CONNECTED )
	{
		printf( "Starting Server Shutdown and Disconnecting all Clients\n" );

		if( csocket ) 
		{
			SDLNet_TCP_Close( csocket );
		}
		csocket = NULL;
		clients.clear();
	}
	else if( state == STATE_CLIENT_CONNECTED )
	{
		printf( "Disconnecting from Server\n" );

		if( csocket ) 
		{
			SDLNet_TCP_Close( csocket );
		}
		csocket = NULL;
	}

	Free_Socketset();
	
	state = STATE_IDLE;
	
	ip.host = 0;
	ip.port = 0;
	
	return 1;
}

/**	Checks if an message arrived
 *	returns the number of messages
 */
int cTCPNet :: Check_Sockets( void ) // 1.5
{
	if( !socketset ) 
	{
		return 0;
	}

	int buffers = SDLNet_CheckSockets( socketset, 0 );
	
	if( buffers == -1 )
	{
		printf( "Check_Sockets Error : %s\n", SDLNet_GetError() );
		Shutdown();
		return 0;
	}
	
	return buffers;
}


/**	Allocates an Socketset for buffer message checking
 *	used internally
 */
int cTCPNet :: Alloc_Socketset( void ) // 2.6
{
	Free_Socketset();
	
	if( state == STATE_HOSTING_CONNECTED ) 
	{
		socketset = SDLNet_AllocSocketSet( clients.size() + 1 ); // + 1 for csocket
	}
	else if( state == STATE_CLIENT_CONNECTED || state == STATE_HOSTING_EMPTY ) 
	{
		socketset = SDLNet_AllocSocketSet( 1 );
	}
	else
	{
		printf( "Alloc_Socketset Error : Not Connected - no sockets to Allocate\n" );
		return 0;
	}

	if( !socketset )
	{
		printf( "SDLNet_AllocSocketSet : Socketset Allocation Error : %s\n", SDLNet_GetError() );
		Shutdown();
		return 0;
	}

	if( csocket && SDLNet_TCP_AddSocket( socketset, csocket ) == -1 )
	{
		printf( "Socketset Connection Socket Allocation Error : SDLNet_TCP_AddSocket : %s\n", SDLNet_GetError() );
		Shutdown();
		return 0;
	}

	if( state == STATE_HOSTING_CONNECTED ) 
	{
		for( unsigned int i = 0; i < clients.size(); i++ )
		{
			printf( "Socketset : Adding Client %d\n",i );
			
			if( SDLNet_TCP_AddSocket( socketset, clients[i].sock ) == -1 )
			{
				printf( "Socketset : Client Socket %d Allocation Error : SDLNet_TCP_AddSocket : %s\n", i, SDLNet_GetError() );
			}
		}
	}

	return 1;
}

// Frees the Socketset
void cTCPNet :: Free_Socketset( void ) // 1.1
{
	if( socketset )
	{
		SDLNet_FreeSocketSet( socketset );
		socketset = NULL;
	}
}

/**	Returns the Server IP in an integer without points
 *	returns 0 if not succesfully Connected ir not an client
 */
Uint32 cTCPNet :: Client_GetIP( void ) // 2.1
{
	if ( state == STATE_CLIENT_CONNECTED ) 
	{
		return SDL_SwapBE32( ip.host );
	}

	return 0;
}

/**	Returns the currently used port
 *	returns 0 if not succesfully hosted or not an connected client
 */
Uint16 cTCPNet :: GetPort( void ) // 2.5
{
	return port;
}

/**	Sets the used port for Hosting and Connecting
 *	Should only be used if not Connected (Idle)
 */
void cTCPNet :: SetPort( Uint16 nport ) // 2.5
{
	if( state == STATE_IDLE ) 
	{
		port = nport;
	}
	else
	{
		printf( "Net SetPort Warning : Already Connected \n" );
	}
}

/**	Disables new Connections
 *	Connected Server Only
 */
void cTCPNet :: Server_NoNewConnections( void ) // 1.1
{
	if( state != STATE_HOSTING_CONNECTED ) 
	{
		return;
	}
	
	SDLNet_TCP_Close( csocket );
	csocket = NULL;
}

/**	Disconnects an specific Client
 *	Connected Server Only
 */
void cTCPNet :: Server_DisconnectClient( TCPsocket sock ) // 2.1
{
	if( state != STATE_HOSTING_CONNECTED ) 
	{
		return;
	}

	for( std::vector<cClient>::iterator iter = clients.begin(); iter < clients.end(); iter++ ) // untested
	{
		if( iter->sock == sock )
		{
			printf( "Server : Client Removed\n" );
			
			if( iter->sock ) 
			{
				SDLNet_TCP_Close( iter->sock );
			}
			
			clients.erase( iter );
			break;
		}
	}

	if( !clients.size() ) // No Clients
	{
		Shutdown();
	}
	else
	{
		Alloc_Socketset();
	}
	

	return;
}
/**	Checks for messages and closes the connection if the remote user disconnected
 *	The messages are handled by the handlemessage(string message) function
 */
void cTCPNet :: Update( void ) // 1.8
{
	if( state == STATE_IDLE ) 
	{
		return;
	}


	int socketsReady = Check_Sockets();

	while( socketsReady > 0 ) // get number of messages,sockets to parse,read
	{
		//printf("Checking Sockets left to handle : %d\n", Check_Sockets() );
		
		// ## Connection Checking
		if( state == STATE_HOSTING_EMPTY || state == STATE_HOSTING_CONNECTED )
		{
			if( Server_ConnectionCheck() )	// Handle incoming Connections 
			{
				socketsReady--;
			}
		}
		
		// ## Message Reading
		if( state == STATE_HOSTING_CONNECTED || state == STATE_CLIENT_CONNECTED )
		{
			string Message;

			if( state == STATE_HOSTING_CONNECTED ) 
			{
				for( std::vector<cClient>::iterator iter = clients.begin(); iter < clients.end(); iter++ )
				{
					if( Read_Message( iter->sock, Message ) ) 
					{
						NetMessageHandler( Message );

						socketsReady--;
					}
				}
			}
			else if( state == STATE_CLIENT_CONNECTED ) 
			{
				if( Read_Message( csocket, Message ) )
				{
					NetMessageHandler( Message );

					socketsReady--;
				}
			}
		}

		//printf( "Messages Left %d\n", socketsReady );

		if( socketsReady <= 0 ) 
		{
			socketsReady = Check_Sockets();
		}
		else
		{
			printf("Warning : Could not read all Net Messages\n" );
			
			socketsReady = 0;
		}
	}
}

// Adds the terminating NULL in an char *
char *cTCPNet :: c_strNC( const string& theString )
{
	char* ret = new char[ theString.length() + 1 ];
	ret[ theString.copy( ret, string::npos) ] = 0;
	
	return ret;
}