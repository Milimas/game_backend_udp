#ifndef MATCHMAKING_HPP
#define MATCHMAKING_HPP

#include "Player.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <netinet/in.h>

class MatchMaking
{
public:
	std::map<struct sockaddr_in, Player> players ;
	std::vector<Game> games ;
	std::map<struct sockaddr_in, Game> playersToGame ;
	std::queue<Player> gameQueue ;

	MatchMaking( void )
	{
	}
	~MatchMaking( void )
	{
	}
    bool isPlayerNew(struct sockaddr_in clientaddr)
    {
        if ( players.find(clientaddr) == players.end() )
            return (true) ;
        return (false) ;
    }
    bool isPlayerInGame(struct sockaddr_in clientaddr)
    {
        if ( !isPlayerNew(clientaddr) && playersToGame.find(clientaddr) != playersToGame.end() )
            return (true) ;
        return (false) ;
    }
    void createNewPlayer(struct sockaddr_in clientaddr)
    {
        if ( isPlayerNew(clientaddr) )
		{
            memcpy(&players[clientaddr].addr, &clientaddr, sizeof(clientaddr)) ;
            std::cerr << __PRETTY_FUNCTION__ << ": NEW PLAYER ID: " << players[clientaddr].id << std::endl ;
            gameQueue.push(players[clientaddr]) ;
            createNewGame() ;
		}
    }
    void    createNewGame( void )
    {
        Player player1 ;
        Player player2 ;
        Game game ;

        if (gameQueue.size() >= 2)
        {
            player1 = game.join(gameQueue.front()) ;
            gameQueue.pop() ;

            player2 = game.join(gameQueue.front()) ;
            gameQueue.pop() ;

            playersToGame[player1.addr] = game ;
            playersToGame[player2.addr] = game ;
            games.push_back(game) ;
            std::cerr << __PRETTY_FUNCTION__ << ": NEW GAME ID: " << game.id << std::endl ;
        }
    }
    struct sockaddr_in getOpponentSockaddr(struct sockaddr_in clientaddr)
    {
        std::map<struct sockaddr_in, Player>::iterator pIt = playersToGame[clientaddr].players.begin() ;
        while ( pIt != playersToGame[clientaddr].players.end() )
        {
            if (pIt->second.id != players[clientaddr].id)
                return (pIt->second.addr) ;
            pIt++ ;
        }
        return ((struct sockaddr_in){0}) ;
    }
};

#endif
