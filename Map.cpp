#include<iostream>
#include <stdio.h>
#include "libtcod.hpp"
#include "Map.hpp"
#include "Actor.hpp"
#include "Engine.hpp"


static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;

class BspListener : public ITCODBspCallback {
private:
    Map &map; // a map to dig
    int roomNum; // room number
    int lastx,lasty; // center of the last room

public:
    BspListener(Map &map) : map(map), roomNum(0) {}
    bool visitNode(TCODBsp *node, void *userData) {
    	if ( node->isLeaf() ) {
            printf ("\n\n*****  Start node for room %d *****\n", roomNum);
            // This is a leaf, i.e., has no children / splits
    		int x,y,w,h;
			// dig a room
			TCODRandom *rng=TCODRandom::getInstance();
			w=rng->getInt(ROOM_MIN_SIZE, node->w-2);
			h=rng->getInt(ROOM_MIN_SIZE, node->h-2);
			x=rng->getInt(node->x+1, node->x+node->w-w-1);
			y=rng->getInt(node->y+1, node->y+node->h-h-1);
			map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1, roomNum);
            map.render();
            //std::cin.get();
            map.render();
            int i = 0;
            while (i < 100000000)
            {
                i++;
            }
            printf("Middle of room: x=%d, y=%d\n", x+w/2, y+h/2);
			if ( roomNum != 0 ) {
			    // dig a corridor from last room
			    map.dig(lastx,lasty,x+w/2,lasty);
			    map.dig(x+w/2,lasty,x+w/2,y+h/2);
			}
            lastx=x+w/2;
            lasty=y+h/2;
            printf ("Finished node for Room %d\n\n", roomNum);
            roomNum++;
        }
        return true;
    }
};

Map::Map(int width, int height) : width(width),height(height) {
    // We will make the tiles the size of one characater.
    tiles=new Tile[width*height];

    TCODBsp bsp(0,0,width,height);
    bsp.splitRecursive(NULL,8,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.5f,1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener,NULL);
}

Map::~Map() {
    delete [] tiles;
}

void Map::dig(int x1, int y1, int x2, int y2) {
    if ( x2 < x1 ) {
        int tmp=x2;
        x2=x1;
        x1=tmp;
    }
    if ( y2 < y1 ) {
        int tmp=y2;
        y2=y1;
        y1=tmp;
    }
    int sumx = x2 - x1;
    int sumy = y2 - y1;
    printf("x2 - x1 %d\n", sumx);
    printf("y2 - y1 %d\n", sumy);
    for (int tilex=x1; tilex <= x2; tilex++) {
        for (int tiley=y1; tiley <= y2; tiley++) {
            // tiley * width gets number of tiles in complete horizontal rows 
            // tilex adds the number of tiles on the current row
            // e.g x is 2 y is 3
            // ******
            // ******
            // **
            tiles[tilex+(tiley*width)].canWalk=true;
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, int roomNum) {
    dig (x1,y1,x2,y2);
    if ( first ) {
        // put the player in the centre of the first room
        engine.player->x=(x1+x2)/2;
        engine.player->y=(y1+y2)/2;
    } else {
        TCODRandom *rng=TCODRandom::getInstance();
        if ( rng->getInt(0,3)==0 ) {
            engine.actors.push(new Actor((x1+x2)/2,(y1+y2)/2,'T',
                TCODColor::yellow));
        }
        char rn = '1' + roomNum;
        engine.actors.push(new Actor((x1+x2)/2,(y1+y2)/2,
            //Integer.ToString(roomNum),
            rn,
            TCODColor::yellow));
    }
}

bool Map::isWall(int x, int y) const {
    return !tiles[x+y*width].canWalk;
}
 
void Map::render() const {
    //static const TCODColor lightWall = TCODColor::white; 
    static const TCODColor darkWall = TCODColor::lightGrey; 
    static const TCODColor darkGround = TCODColor::black; 
    //static const TCODColor lightGround = TCODColor::darkerGrey; 

	for (int x=0; x < width; x++) {
	    for (int y=0; y < height; y++) {
	        TCODConsole::root->setCharBackground( x,y,
	            isWall(x,y) ? darkWall : darkGround );
	    }
	}
}
