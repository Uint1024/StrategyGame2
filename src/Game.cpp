#include "Game.h"

Game::Game() : config_(), graphics_(), world_editor_{Point{10,10}, Dimension{100,100}, true, Color{200,0,0}}
{

    gameLoop();
}

Game::~Game()
{
    //dtor
}


void Game::gameLoop()
{
    playing_ = true;
   SDL_Event event;
    Dimension screen_dimension_in_tiles = config_.getScreenSize() / config_.getTileSize();
    Peasant peasant1{Point{20,20}};
    peasant1.setGoal(Point{10,10});
    std::vector<DIRECTION> direc = {UP, LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT};
    //peasant1.setPathfinding(direc);
    npcs_list_.push_back(peasant1);
    start = {0,0};
    finish = {0,0};
    chrono_for_pathfinding_test_ = 0;
    for(int x = 0; x < config_.getMapSize().x ; x++ )
    {
        for(int y = 0 ; y < config_.getMapSize().y ; y++)
        {
            Plains plains_tile(Point{x,y});
            world_map_.push_back(plains_tile);
        }

    }

    for(int i = 0 ; i <= 200 ; i++)
    {
        for(int j = 0 ; j <= 200 ; j++)
        {
            solid_map_[i][j] = false;
        }
    }

    elapsed_time_ = SDL_GetTicks();
    while(playing_)
    {
        update(event);
        draw(graphics_, config_);
    }

    SDL_Quit();
}

void Game::update(SDL_Event& event)
{
    //inputs_.mouseIsImmobile();
    inputs_.calculateMouseTravel(event);
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            playing_ = false;
            break;

        case SDL_KEYDOWN:
            inputs_.keyDownEvent(event);
            break;

        case SDL_KEYUP:
            inputs_.keyUpEvent(event);
            break;

        case SDL_MOUSEBUTTONDOWN:
            inputs_.mouseButtonDownEvent(event);
            break;

        case SDL_MOUSEBUTTONUP:
            inputs_.mouseButtonUpEvent(event);
            break;

        case SDL_MOUSEMOTION:
            inputs_.mouseMotionEvent(event);
            break;

        default:
            break;
        }

    }

    Point mouse_coords = inputs_.getMouseCoordinatesInPixels() + graphics_.getCamera().getPosition();
    Point mouse_coordinates_in_tiles;
    mouse_coordinates_in_tiles = (mouse_coords - (mouse_coords % config_.getTileSize()))/ config_.getTileSize();

    if(inputs_.getPressedMouseButtons()[SDL_BUTTON_LEFT])
    {
        if(mouse_coords.intersect(world_editor_.getRect()) ||
           inputs_.getLockedWindow() == (&world_editor_))
        {
            world_editor_.receiveInputs(inputs_);
        }
        else if(inputs_.getLockedWindow() == nullptr)
        {
            auto it = std::find_if(world_map_.begin(), world_map_.end(),
                                    [&mouse_coordinates_in_tiles](const Tile& obj) -> bool
                                    {
                                        (mouse_coordinates_in_tiles == obj.getPosition());
                                    });

            if((*it).getType() != WALL)
            {
                world_map_.erase(it);

                Wall wallou(mouse_coordinates_in_tiles);
                world_map_.push_back(wallou);
                std::cout << "creating wall at" << mouse_coordinates_in_tiles.x << ":" << mouse_coordinates_in_tiles.y << "with type" << wallou.getType() << std::endl;
                solid_map_[mouse_coordinates_in_tiles.x][mouse_coordinates_in_tiles.y] = true;
            }

            /*for(auto npc : npcs_list_)
            {
                SDL_Rect rect ={npc.getPosition().x * config_.getTileSize().x + graphics_.getCamera().getPosition().x,
                                npc.getPosition().y * config_.getTileSize().y + graphics_.getCamera().getPosition().y,
                                config_.getTileSize().x, config_.getTileSize().y};
                if(mouse_coords.intersect(rect))
                {
                    //std::cout << npc.getGoal().x << std::endl;
                    std::cout << findPath(npc.getPosition(), npc.getGoal()) << std::endl;
                }
            }*/
        }
    }

    if(inputs_.getPressedMouseButtons()[SDL_BUTTON_RIGHT] && npcs_list_[0].getTimeSinceLastOrder() > 300)
    {
        npcs_list_[0].setGoal(mouse_coordinates_in_tiles);
        findPath(npcs_list_[0].getPosition(), npcs_list_[0].getGoal());
        //std::cout << findPath(npcs_list_[0].getPosition(), npcs_list_[0].getGoal()) << std::endl;
        //for(int i = 0 ; i < npcs_list_[0].get)
        npcs_list_[0].setTimeSinceLastOrder();
    }

    if(inputs_.getPressedKeys()[SDL_SCANCODE_RIGHT])
        graphics_.moveCamera(IVector{5,0});
    if(inputs_.getPressedKeys()[SDL_SCANCODE_LEFT])
        graphics_.moveCamera(IVector{-5,0});
    if(inputs_.getPressedKeys()[SDL_SCANCODE_UP])
        graphics_.moveCamera(IVector{0,-5});
    if(inputs_.getPressedKeys()[SDL_SCANCODE_DOWN])
        graphics_.moveCamera(IVector{0,5});


    elapsed_time_ = SDL_GetTicks() - get_ticks_previous_frame_;

    chrono_for_pathfinding_test_ += elapsed_time_;
    fps_ = 1000/elapsed_time_;
    get_ticks_previous_frame_ = SDL_GetTicks();


    //std::cout << chrono_for_pathfinding_test_ << std::endl;
    if(chrono_for_pathfinding_test_ > 50)
    {
        npcs_list_[0].update();
        chrono_for_pathfinding_test_ = 0;
    }
}


void Game::draw(Graphics& graphics, Config& config)
{
    graphics_.clear();
    graphics_.drawTiles(config, world_map_, pathfinding_nodes_);
    graphics_.drawWindows(world_editor_);
    graphics_.drawNpcs(config_, npcs_list_);
    graphics_.flip();
}



std::string Game::findPath( const Point start,
                 const Point finish)
{
    static std::priority_queue<Node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static Node* n0;
    static Node* m0;
    static int i, j, x, y, xdx, ydy;
    Point new_pos = {0,0};
    static char c;
    int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    pqi=0;

    // reset the node maps
    for(y=0;y<200;y++)
    {
        for(x=0;x<200;x++)
        {
            closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
        }
    }

    // create the start node and push into list of open nodes
    n0=new Node(start, 0, 0);
    n0->calculate_f_score(finish);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]=n0->getFScore(); // mark it on the open nodes map

    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new Node( pq[pqi].top().getPosition(),
                     pq[pqi].top().getGScore(), pq[pqi].top().getFScore());

        x=n0->getPosition().x; y=n0->getPosition().y;

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y]=0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y]=1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==finish.x && y==finish.y)
        {
            // generate the path from finish to start
            // by following the directions
            std::string path="";
            std::vector<DIRECTION> pathfinding_vector;
            while(!(x==start.x && y==start.y))
            {
                j=dir_map[x][y];

                c='0'+(j+8/2)%8;
                path=c+path;
                auto it = pathfinding_vector.begin();
                it = pathfinding_vector.insert(it,static_cast<DIRECTION>((j+4)%8) );

                x+=xMovement[j];
                y+=yMovement[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
            npcs_list_[0].setPathfinding(pathfinding_vector);
            return path;
        }
        // generate moves (child nodes) in all possible directions
        for(i=0;i<8;i++)
        {
            //xdx=x+xMovement[i]; ydy=y+yMovement[i];
            new_pos = Point{x+xMovement[i], y+yMovement[i]};

            /*auto wall_position = find_if(world_map_.begin(), world_map_.end(), [&new_pos](const Tile& tile) -> bool
                                    {
                                   return (tile.getPosition() == new_pos && tile.getType() == WALL);
                                   });
*/
            //if(wall_position == world_map_.end())
                //std::cout << "tile at " << new_pos.x << ":" << new_pos.y <<  "isn't a wall, so we proceed" << std::endl;

            if(!(new_pos.x<0 || new_pos.x>199 || new_pos.y<0 || new_pos.y>199 || closed_nodes_map[new_pos.x][new_pos.y]==1) && !solid_map_[new_pos.x][new_pos.y])
            {
                //std::cout << new_pos.x << ":" << new_pos.y << std::endl;
                // generate a child node
                m0=new Node( new_pos, n0->getGScore(),
                             n0->getFScore());
                m0->calculate_g_score(static_cast<DIRECTION>(i));
                m0->calculate_f_score(finish);

                // if it is not in the open list then add into that
                if(open_nodes_map[new_pos.x][new_pos.y]==0)
                {
                    open_nodes_map[new_pos.x][new_pos.y]=m0->getFScore();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[new_pos.x][new_pos.y]=(i+8/2)%8;
                    //std::cout << "new dir = " << dir_map[new_pos.x][new_pos.y] << std::endl;

                }
                else if(open_nodes_map[new_pos.x][new_pos.y]>m0->getFScore())
                {

                    // update the priority info
                    open_nodes_map[new_pos.x][new_pos.y]=m0->getFScore();
                    // update the parent direction info
                    dir_map[new_pos.x][new_pos.y]=(i+8/2)%8;
                    /*std::cout << "tile at " << new_pos.x << ":" << new_pos.y <<
                    "isn't a wall, and it's in the open list, but it has a higher priority, so we add it and give it direction" <<
                    dir_map[new_pos.x][new_pos.y] << std::endl;*/
                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getPosition() == new_pos))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    return ""; // no route found
}
