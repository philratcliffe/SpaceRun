class Engine {
public :
    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
 
    Engine();
    ~Engine();
    void update();
    void render();
private:
    TCODColor m_playerColor;
};
 
extern Engine engine;
