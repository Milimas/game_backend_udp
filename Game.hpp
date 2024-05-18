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
		std::cerr << id << "::" << __PRETTY_FUNCTION__ << std::endl ;
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
		std::cerr << id << "::" << __PRETTY_FUNCTION__ << std::endl ;
	}

	Player join( const Player& player )
	{
		std::cout << "player: " << player.id << " joined :" << id << std::endl ;
		players[player.addr] = player ;
		return (player) ;
	}
};

#endif
