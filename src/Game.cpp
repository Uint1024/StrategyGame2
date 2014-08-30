#include "Game.h"

Game::Game() : config_(), graphics_()
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

    start = {0,0};
    finish = {0,0};
    for(int x = 0; x < config_.getMapSize().x ; x++ )
    {
        for(int y = 0 ; y < config_.getMapSize().y ; y++)
        {
            Plains plains_tile(Point{x,y});
            world_map_.push_back(plains_tile);
        }

    }

    while(playing_)
    {
        update(event);
        draw(graphics_, config_);
    }

    SDL_Quit();
}

void Game::update(SDL_Event& event)
{
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

    if(inputs_.getPressedMouseButtons()[SDL_BUTTON_LEFT])
    {

        Point mouse_coords = inputs_.getMouseCoordinatesInPixels() + graphics_.getCamera().getPosition();
        Point mouse_coordinates_in_tiles;
        mouse_coordinates_in_tiles = (mouse_coords - (mouse_coords % config_.getTileSize()))/ config_.getTileSize();

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
            //std::cout << world_map_.size() << std::endl;
        }


    }

    if(inputs_.getPressedMouseButtons()[SDL_BUTTON_RIGHT])
    {
        Point mouse_coords = inputs_.getMouseCoordinatesInPixels();
        Point mouse_coordinates_in_tiles;
        mouse_coordinates_in_tiles = (mouse_coords - (mouse_coords % config_.getTileSize()))/ config_.getTileSize();
        if(start == Point{0,0})
            start = Point{mouse_coordinates_in_tiles.x, mouse_coordinates_in_tiles.y};
        else if (start != Point{mouse_coordinates_in_tiles.x, mouse_coordinates_in_tiles.y})
        {
            finish = Point{mouse_coordinates_in_tiles.x, mouse_coordinates_in_tiles.y};
            std::string pathfound = findPath(start, finish);
            std::cout << pathfound << std::endl;
        }
    }

    if(inputs_.getPressedKeys()[SDL_SCANCODE_RIGHT])
        graphics_.moveCamera(IVector{5,0});
    if(inputs_.getPressedKeys()[SDL_SCANCODE_LEFT])
        graphics_.moveCamera(IVector{-5,0});
    if(inputs_.getPressedKeys()[SDL_SCANCODE_UP])
        graphics_.moveCamera(IVector{0,-5});
    if(inputs_.getPressedKeys()[SDL_SCANCODE_DOWN])
        graphics_.moveCamera(IVector{0,5});

}


void Game::draw(Graphics& graphics, Config& config)
{
    graphics_.clear();
    graphics_.drawTiles(config, world_map_, pathfinding_nodes_);
    graphics_.flip();
}



std::string Game::findPath( const Point start,
                 const Point finish)
{
    std::string path = "";
    std::vector<Node> nodes_map;

    //enum DIRECTION {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
    int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dir_map[config_.getTileSize().x][config_.getTileSize().y];

    static std::priority_queue<Node> priotity_q[2]; // list of open (not-yet-tried) nodes
    static int priority_q_index; // pq index
    static Node* node0;
    static Node* m0;
    static char c;
    priority_q_index=0;

    Dimension screen_dimension_in_tiles = config_.getScreenSize() / config_.getTileSize();

    int closed_nodes_map[screen_dimension_in_tiles.x][screen_dimension_in_tiles.y];
    int open_nodes_map[screen_dimension_in_tiles.x][screen_dimension_in_tiles.y];

    // reset the node maps
    for(int y=0;y<screen_dimension_in_tiles.y;y++)
    {
        for(int x=0;x<screen_dimension_in_tiles.x;x++)
        {
            closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
        }
    }



    // create the start node and push into list of open nodes
    node0=new Node(start, 0, 0);
    node0->calculate_f_score(finish);
    priotity_q[priority_q_index].push(*node0);
    //open_nodes_map[x][y]=n0->getPriority(); // mark it on the open nodes map

    // A* search
    while(!priotity_q[priority_q_index].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        node0=new Node( priotity_q[priority_q_index].top().getPosition(),
                        priotity_q[priority_q_index].top().getGScore(),
                        priotity_q[priority_q_index].top().getFScore());

        //x=n0->getxPos(); y=n0->getyPos();

        priotity_q[priority_q_index].pop(); // remove the node from the open list
        Point node0_position = node0->getPosition();
        //open_nodes_map[node0_position.x][node0_position.y]=0;

        // mark it on the closed nodes map
        closed_nodes_map[node0_position.x][node0_position.y]=1;

        auto it = find_if(pathfinding_nodes_.begin(), pathfinding_nodes_.end(), [&node0_position](const Node* node) -> bool{ node0_position == node->getPosition();});

        if(it != pathfinding_nodes_.end())
        {
            (*it)->setStatus(2);
        }
        else
        {
            Node* node_on_path = new Node(node0_position, node0->getGScore(), node0->getFScore());
            node_on_path->setStatus(2);
            pathfinding_nodes_.push_back(node_on_path);

        }
//        pathfinding_nodes_.push_back(n0);
        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(node0->getPosition() == finish)
        {
            std::cout << "path found, ending at " << node0->getPosition().x << ":" << node0->getPosition().y << std::endl;
            // generate the path from finish to start
            // by following the directions
            //string path="";
            int x = node0->getPosition().x;
            int y = node0->getPosition().y;
            int j;
            char c;
            while(!(x==start.x && y==start.y))
            {
                j=dir_map[x][y];
                c='0'+(j+8/2)%8;
                path=c+path;
                x+=xMovement[j];
                y+=xMovement[j];

                std::cout << dir_map[x][y] << " at " << x << ":" << y << std::endl;
                std::cout << path << std::endl;
            }

            // garbage collection
            //delete n0;
            // empty the leftover nodes
            while(!priotity_q[priority_q_index].empty()) priotity_q[priority_q_index].pop();

            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(int i=0;i<8;i++)
        {

            Point next_position = {node0_position.x + xMovement[i], node0_position.y + yMovement[i]};

            if(!(next_position.x<0 || next_position.x>screen_dimension_in_tiles.x-1 ||
                 next_position.y<0 || next_position.y>screen_dimension_in_tiles.y-1 ||
                 /*map[next_position.x][ydy]==1  ||*/ closed_nodes_map[next_position.x][next_position.y]==1))
            {
                // generate a child node
                m0=new Node( next_position, node0->getGScore(),
                             node0->getFScore());
                m0->calculate_g_score(static_cast<DIRECTION>(i));
                m0->calculate_f_score(finish);

                // if it is not in the open list then add into that
                if(open_nodes_map[next_position.x][next_position.y]==0)
                {
                    open_nodes_map[next_position.x][next_position.y]=m0->getFScore();
                    priotity_q[priority_q_index].push(*m0);

                    //m0->setStatus(1);
                    //pathfinding_nodes_.push_back(m0);
                    // mark its parent node direction
                    dir_map[next_position.x][next_position.y]=(i+8/2)%8;
                    //pathfinding_nodes_.push_back(m0);
                }
                //if it's already in the open list but the priority was lower
                //(ie is was longer to go to this tile through the previous path)
                else if(open_nodes_map[next_position.x][next_position.y]>m0->getFScore())
                {
                    // update the priority info
                    open_nodes_map[next_position.x][next_position.y]=m0->getFScore();
                    // update the parent direction info
                    dir_map[next_position.x][next_position.y]=(i+8/2)%8;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead

                    //while the top node of the current priority queue isn't this node,
                    //we push the top node from the current queue to the other priority list
                    //and then remove it from the current list
                    while(!(priotity_q[priority_q_index].top().getPosition()==next_position))
                    {
                        priotity_q[1-priority_q_index].push(priotity_q[priority_q_index].top());
                        priotity_q[priority_q_index].pop();
                    }

                    //and then we remove the node from the current queue (what)
                    priotity_q[priority_q_index].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(priotity_q[priority_q_index].size()>priotity_q[1-priority_q_index].size()) priority_q_index=1-priority_q_index;
                    while(!priotity_q[priority_q_index].empty())
                    {
                        priotity_q[1-priority_q_index].push(priotity_q[priority_q_index].top());
                        priotity_q[priority_q_index].pop();
                    }
                    priority_q_index=1-priority_q_index;
                    priotity_q[priority_q_index].push(*m0); // add the better node instead
                    //pathfinding_nodes_.push_back(m0);
                }
                else delete m0; // garbage collection
            }
        }
        delete node0; // garbage collection
    }
    return ""; // no route found*/
}
