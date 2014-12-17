/// @addtogroup Engine_Net
// @{
/** 

	@file net.h  Network class for Server and Multiple Client Connections

	V 2.1 Beta 150

	LGPL (c) F. Richter

*/

#ifndef __EP_NET_H__
#define __EP_NET_H__

#include <iostream>

#include <vector>
#include <string>
#include <list>

using std::string;
using std::vector;
using std::list;

#ifdef _WIN32
#include <SDL.h>
#include <SDL_net.h>
#else
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#endif



/// The Network states
#define STATE_IDLE 0
#define STATE_HOSTING_EMPTY 1
#define STATE_HOSTING_CONNECTED 2
#define STATE_CLIENT_CONNECTED 3

/// The Maximum Buffer size
#define MAX_BUFFER_SIZE 100000

/// The function for incoming messages
extern DLLINTERFACE void NetMessageHandler( string message ); // untested 


/// The TCP Client class V.3.4
class cClient
{
public:
	DLLINTERFACE cClient( TCPsocket initSock );
	
	DLLINTERFACE cClient( const cClient &Client );

	DLLINTERFACE ~cClient( void );

	DLLINTERFACE TCPsocket Getsocket( void );

	DLLINTERFACE Uint32 GetIP( void );

	DLLINTERFACE Uint16 GetPort( void );

	TCPsocket sock;
	IPaddress *ip;
};

/**	The TCP SDL_Net Class
 */
class cTCPNet
{
public:
	DLLINTERFACE cTCPNet( unsigned int nport = 9877 );

	DLLINTERFACE ~cTCPNet( void );

	/**	Starts an Server
	 *	returns 0 if already connected or an error
	 */
	DLLINTERFACE int Server_Start( void ); // 2.3

	/**	Checks for an connecting request from an remote client
	 *	used internally
	 */
	DLLINTERFACE int Server_ConnectionCheck( void ); // 2.1

	/**	Connects to an server
	 *	returns 0 if there were no success and 1 on success
	 */
	DLLINTERFACE int Client_Connect( string &address ); // 2.2
	
	/**	Reads the Message
	 *	Returns 0 if an error occured or not connected to anyone
	 */
	DLLINTERFACE int Read_Message( TCPsocket sock, string &message ); // 2.8
	
	/**	Sends the Message to all Connections you have
	 *	uses Send_message if you're an Client
	 *	else Server_SenGlobalMessage if you're an Server
	 */
	DLLINTERFACE void Send_Message_Auto( const string message ); // 1.0

	/**	Sends an Message to ervery Client
	 *	
	 */
	DLLINTERFACE void Server_SendGlobalMessage( const string message ); // 1.1

	/**	Sends an Message
	 *	Returns 0 if an error occured or not connected to anyone
	 */
	DLLINTERFACE int Send_Message( TCPsocket sock, const string message ); // 2.6

	/**	Closes every Connection
	 *	returns 0 if there were no connections or opened sockets
	 */
	DLLINTERFACE int Shutdown( void ); // 2.3

	/**	Checks if an message arrived
	 *	returns the number of messages
	 */
	DLLINTERFACE int Check_Sockets( void ); // 1.5
	
	/**	Allocates an Socketset for buffer message checking
	 *	used internally
	 */
	DLLINTERFACE int Alloc_Socketset( void ); // 2.6

	/// Frees the Socketset
	DLLINTERFACE void Free_Socketset( void ); // 1.1

	/**	Returns the Server IP in an integer without points
	 *	returns 0 if not succesfully Connected ir not an client
	 */
	DLLINTERFACE Uint32 Client_GetIP( void ); // 2.1
	
	/**	Returns the currently used port
	 *	returns 0 if not succesfully hosted or not an connected client
	 */
	DLLINTERFACE Uint16 GetPort( void ); // 2.5

	/**	Sets the used port for Hosting and Connecting
	 *	Should only be used if not Connected (Idle)
	 */
	DLLINTERFACE void SetPort( Uint16 nport ); // 2.5

	/**	Disables new Connections
	 *	Connected Server Only
	 */
	DLLINTERFACE void Server_NoNewConnections( void ); // 1.1

	/**	Disconnects an specific Client
	 *	Connected Server Only
	 */
	DLLINTERFACE void Server_DisconnectClient( TCPsocket sock ); // 2.1

	/**	Checks for messages and closes the connection if the remote user disconnected
	 *	The messages are handled by the handlemessage(string message) function
	 */
	DLLINTERFACE void Update( void ); /// 1.8

	/// Adds the terminating NULL in an char *
	DLLINTERFACE char *c_strNC( const string& theString );

	/**	The Clients
	 *	if the current state is Server theres no maximum
	 *	else if client the first Client is the Remote Server you're Connected to
	 */
	vector<cClient> clients;

	/// undefined - is it only temporary ?!?
	IPaddress ip;
	/// Your Connection Socket
	TCPsocket csocket;
	/// The Standard port used
	Uint16 port;

	/**	The Current Network state :
	 *	0 = Idle
	 *	1 = Hosting Not Connected to anyone
	 *	2 = Hosting Connected
	 *	3 = Client Connected
	 */
	int state;

	SDLNet_SocketSet socketset; /// Socketset

};

#endif
// @}