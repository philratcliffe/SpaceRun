#include "libtcod.hpp"  // All the libtcod classes declaration
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

Engine engine;

int main() {
    // The main loop. Loop until the game window is closed by the player.
    while ( !TCODConsole::isWindowClosed() ) {
    	engine.update();
    	engine.render();
		TCODConsole::flush();
    }
    return 0;
}
