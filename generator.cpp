#include <random>
#include <vector>
#include <iostream>

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

class Dungeon
{
public:
	enum Tile
	{
		Unused		= ' ',
		Floor		= '.',
		Corridor	= ',',
		Wall		= '#',
		ClosedDoor	= '+',
		OpenDoor	= '-',
		UpStairs	= '<',
		DownStairs	= '>'
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

	void generate()
	{
		// place the first room in the center
		if (!makeRoom(_width / 2, _height / 2, true))
		{
			std::cout << "Unable to place the first room.\n";
			return;
		}

        for (int i = 0; i < 1; i++)
        {
            // Select a random location on the map.
            //int x = randomInt(_width);
            //int y = randomInt(_height);
            Rect room = _rooms[_rooms.size() - 1];
			int x = randomInt(room.x, room.x + room.width - 1);
			int y = randomInt(room.y, room.y + room.height - 1);
            printf("x %d", x);
            printf("y %d", y);
            for (i = 0; i < 10; i++)
            {
                setTile(x, y, 'z');
                x = x + 1;
            }
            //makeCorridor();
        }

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
		static const int minRoomSize = 5;
		static const int maxRoomSize = 8;

		Rect room;
		room.width = randomInt(minRoomSize, maxRoomSize);
		room.height = randomInt(minRoomSize, maxRoomSize);

        room.x = x - room.width;
	    room.y = y - room.height;

		if (placeRect(room, Floor))
        {
            _rooms.push_back(room);
            return true;
        }
	    return false;
	}


	bool placeRect(const Rect& rect, char tile)
	{
        // Check not too near edge of map
		if (rect.x < 2 || rect.y < 2 ||
                rect.x + rect.width >= _width - 1 || rect.y + rect.height >= _height - 1)
			return false;

        // Now check no other room near where we want to build
		for (int y = rect.y - 3; y < rect.y + rect.height + 3; ++y)
			for (int x = rect.x - 4; x < rect.x + rect.width + 4; ++x)
			{
				if (getTile(x, y) != Unused)
					return false; // the area already used
			}

		for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
			for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x)
			{
				if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width || y == rect.y + rect.height)
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
