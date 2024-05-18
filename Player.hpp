#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
class Player
{
public:
	uint64_t	id ;
	struct 		sockaddr_in addr ;
	
	Player( void )
	{
		id = reinterpret_cast<uint64_t>(this) ;
	}
	Player( const Player& rhs )
	{
		id = rhs.id ;
		memcpy(&addr, &rhs.addr, sizeof(addr)) ;
	}
	Player& operator=( const Player& rhs )
	{
		id = rhs.id ;
		memcpy(&addr, &rhs.addr, sizeof(addr)) ;
		return (*this) ;
	}
	~Player ( void )
	{
	}
};

#endif
