#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT = 50;

Engine::Engine() {
    // Create the game window.
    bool startFullScreen = false;
    TCODConsole::initRoot(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Run", startFullScreen);

    m_playerColor = TCODColor::lightestGreen; 
    player = new Actor(40,25,'@', m_playerColor);
    actors.push(player);
    TCODSystem::setFps(20);
    map = new Map(80,45);
}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
}

void Engine::update() {
    TCOD_key_t key;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
    if (key.vk == TCODK_UP || key.c == 'k')
    {
        if ( ! map->isWall(player->x,player->y-1)) {
            player->y--; 
        }
    }
    else if (key.vk == TCODK_DOWN || key.c == 'j')
    {
        if ( ! map->isWall(player->x,player->y+1)) {
            player->y++;
        }
    }
    else if (key.vk == TCODK_LEFT || key.c == 'h')
    {
        if ( ! map->isWall(player->x-1,player->y)) {
            player->x--;
        }
    }
    else if (key.vk == TCODK_RIGHT || key.c == 'l')
    {
        if ( ! map->isWall(player->x+1,player->y)) {
            player->x++;
        }
    }
}

void Engine::render() {
	TCODConsole::root->clear();
	// draw the map
	map->render();
	// draw the actors
	for (Actor **iterator=actors.begin();
	    iterator != actors.end(); iterator++) {
	    (*iterator)->render();
	}
}
