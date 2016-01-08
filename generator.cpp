#include <random>
#include <vector>
#include <iostream>

//
// Draw a central corridor
// Move to random locartion along corridor (say between 5 and 10% of last
// location) 
// Select a random room size
// Select a random number of rooms
// Put same number of rooms either side of the corridor at location
// join these rooms by a corridor
//
namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
		return dist(mt);
	}

	int randomInt(int min, int max) // inclusive min/max
	{
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	bool randomBool(double probability = 0.5)
	{
		std::bernoulli_distribution dist(probability);
		return dist(mt);
	}
}

struct Rect
{
	int x, y;
	int width, height;
};

struct Point
{
    int x, y;
};

class Dungeon
{
public:
	enum Tile
	{
		Unused		= ' ',
		Floor		= '.',
		Corridor	= ',',
		Wall		= '#',
	};
 
	enum Direction
	{
		North,
		South,
		West,
		East,
	};
 

public:
	Dungeon(int width, int height)
		: _width(width)
		, _height(height)
		, _tiles(width * height, Unused)
		, _rooms()
		, _exits()
	{
	}

    ~Dungeon()
    {
        if (!_rooms.empty())
        {
			std::cout << "Clearing rooms vector.\n";
            _rooms.clear();
        }
    }

    Point getRandomRoomPoint()
    {
        //
        // Get a random room
        //
		int r = randomInt(_rooms.size());
        Rect room = _rooms[r];
        Point p;

        //
        // Choose a random location in the room
        //
        p.x = randomInt(room.x, room.x + room.width - 1);
        p.y = randomInt(room.y, room.y + room.height - 1);
        return p;

    }

    void makeVerticalCorridor(int x, int y, int length)
    {


    }

    void makeCentralCorridor(int x, int y, Direction dir, int length)
    {
        for (int i = 0; i < length; i++)
        {
            setTile(x, y, Floor);
            setTile(x, y+1, Wall);
            setTile(x, y-1, Wall);
            if (dir == West)
                x++;
            else
                x--;
        }
    }

	void generate()
	{
        //
        //
        makeCorridor(2, _height / 2,  West, _width - 4);

		// place the first room in the center
        /*
		if (!makeRoom(_width / 2, _height / 2, true))
		{
			std::cout << "Unable to place the first room.\n";
			return;
		}

        for (int i = 0; i < 1; i++)
        {
            Point p = getRandomRoomPoint();
            makeCorridor(p.x, p.y, West);
		    makeRoom(p.x, p.y - 1);
            setTile(p.x - 1, p.y, Floor);

        }
        */

		for (char& tile : _tiles)
		{
			if (tile == Unused)
				tile = '.';
			else if (tile == Floor || tile == Corridor)
				tile = ' ';
		}
	}

	void print()
	{
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
				std::cout << getTile(x, y);

			std::cout << std::endl;
		}
	}

private:
	char getTile(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= _width || y >= _height)
			return Unused;

		return _tiles[x + y * _width];
	}

	void setTile(int x, int y, char tile)
	{
		_tiles[x + y * _width] = tile;
	}

	bool makeRoom(int x, int y, bool firstRoom = false)
	{
		static const int minRoomSize = 4;
		static const int maxRoomSize = 7;

		Rect room;
        room.x = x;
        room.y = y;
		room.width = randomInt(minRoomSize, maxRoomSize);
		room.height = randomInt(minRoomSize, maxRoomSize);

        //room.x = x - room.width;
	    //room.y = y - room.height;

		if (placeRect(room, Floor))
        {
            _rooms.push_back(room);
            return true;
        }
	    return false;
	}


	bool placeRect(const Rect& rect, char tile)
	{
        printf("placeRect: entry");
        // Check not too near edge of map
		if (rect.x < 2 || rect.y < 2 ||
                rect.x + rect.width >= _width - 1 || rect.y + rect.height >= _height - 1)
        {
            printf("Too near edge");
			return false;
        }

        // Now check no other room near where we want to build
		for (int y = rect.y; y < rect.y + rect.height; ++y)
			for (int x = rect.x; x < rect.x + rect.width; ++x)
			{
                char t = getTile(x, y);
                if (t != Unused && t != Floor)
                {

					setTile(x, y, 'P');
                    printf("NOT UNUSED");
					return false; // the area already used
                }
			}

        setTile(rect.x, rect.y, 'P');
		for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
			for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x)
			{
				if (x == rect.x - 1 || y == rect.y - 1
                        || x == rect.x + rect.width
                        || y == rect.y + rect.height)
					setTile(x, y, Wall);
				else
					setTile(x, y, tile);
			}
            setTile(rect.x, rect.y, 'O');

		return true;
	}


private:
	int _width, _height;
	std::vector<char> _tiles;
	std::vector<Rect> _rooms; // rooms for place stairs or monsters
	std::vector<Rect> _exits; // 4 sides of rooms or corridors
};

int main()
{
	Dungeon d(79, 24);
	d.generate();
	d.print();

	std::cout << "Press Enter to quit... ";
	std::cin.get();

	return 0;
}
