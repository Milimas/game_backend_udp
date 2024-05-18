#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <map>

class Game
{
public:
	std::map<struct sockaddr_in, Player> players ;
	uint64_t	id ;

	Game( void )
	{
		id = reinterpret_cast<uint64_t>(this);
	}
	Game( const Game& rhs )
	{
		players = rhs.players ;
		id = rhs.id ;
	}
	Game& operator=( const Game& rhs )
	{
		players = rhs.players ;
		id = rhs.id ;
		return (*this) ;
	}
	~Game( void )
	{
	}

	Player join( const Player& player )
	{
		players[player.addr] = player ;
		std::cerr << __PRETTY_FUNCTION__ << ": PLAYER ID: " << player.id << " JOINED GAME ID: " << id << std::endl ;
		return (player) ;
	}
};

#endif
