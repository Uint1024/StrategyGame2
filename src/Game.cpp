#include "Game.h"
#include "Tree.h"
#include "Stockpile.h"
#include "Blueprint.h"

Game::Game() : config_(), graphics_(), world_editor_{new Window{Point{10,10}, Dimension{100,100}, true, Color{200,200,200}}},
                selected_npc_(nullptr)
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
    selected_npc_ = &npcs_list_[0];
    //std::cout << "in main, " << selected_npc_ << " " << &npcs_list_[0] << std::endl;
    start = {0,0};
    finish = {0,0};
    chrono_for_pathfinding_ = 0;
    chrono_movement_ = 0;
    current_selected_world_editor_object_ = EMPTY;
    /*for(x=0 ; x<config_.getMapSize().x ; x++)
    {
        for(y = 0 ; y < config_.getMapSize().y ; y++)
        {
            closed_nodes_map_.push_back(0);

        }
    }*/
    for(int y = 0 ; y < config_.getMapSize().y ; y++)
    {

        for(int x = 0; x < config_.getMapSize().x ; x++ )
        {
            auto plains_tile = std::make_shared<Plains>(Point{x,y});
            world_map_[0].push_back(plains_tile);
            world_map_[1].push_back(nullptr);
        }

    }

    for(int i = 0 ; i <= config_.getMapSize().x ; i++)
    {
        for(int j = 0 ; j <= config_.getMapSize().y ; j++)
        {
            solid_map_.push_back(false);
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
    //inputs_.calculateMouseTravel(event);


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
    inputs_.update(event);


    //mouse coordinates on the world map (NOT on the screen)
    Point mouse_coords = inputs_.getMouseCoordinatesInPixels() + graphics_.getCamera().getPosition();

    //tile coordinates that corresponds to the mouse position on the map
    Point mouse_coordinates_in_tiles;
    mouse_coordinates_in_tiles = (mouse_coords - (mouse_coords % config_.getTileSize()))/ config_.getTileSize();


    int mouse_position_in_vector = mouse_coordinates_in_tiles.y * config_.getMapSize().y + mouse_coordinates_in_tiles.x;
    if(inputs_.getPressedMouseButtons()[SDL_BUTTON_LEFT])
    {
        //level editor window
        if(inputs_.getMouseCoordinatesInPixels().intersect(world_editor_->getRect()) ||
           inputs_.getLockedWindow() == (world_editor_))
        {
            world_editor_->receiveInputs(inputs_);
            current_selected_world_editor_object_ = world_editor_->clickedOnIcon(inputs_.getMouseCoordinatesInPixels());
        }
        else if(inputs_.getLockedWindow() == nullptr)
        {

            auto it = std::find_if(npcs_list_.begin(), npcs_list_.end(),
                                    [mouse_coordinates_in_tiles](const Peasant& npc) -> bool {
                                        return mouse_coordinates_in_tiles == npc.getPosition();
                                   });

            if(it != npcs_list_.end())
            {
                for(auto &npc : npcs_list_)
                {
                    if(mouse_coords.intersect(npc.getRect(graphics_, config_)))
                    {
                        selected_npc_ = &npc;

                    }
                }
            }
            else
            {
                if(current_selected_world_editor_object_ != EMPTY)
                {
                    if(current_selected_world_editor_object_ == WOOD_WALL_BLUEPRINT)
                    {
                        world_map_[1][mouse_position_in_vector] = std::make_shared<Blueprint>(mouse_coordinates_in_tiles,
                                                                                              current_selected_world_editor_object_,
                                                                                              "wood_wall_blueprint.png");
                    }
                    else if(current_selected_world_editor_object_ == PLAINS)
                    {
                        world_map_[0][mouse_position_in_vector] = std::make_shared<Plains>(mouse_coordinates_in_tiles);
                    }
                    else if(current_selected_world_editor_object_ == TREE)
                    {
                        world_map_[1][mouse_position_in_vector] = std::make_shared<Tree>(mouse_coordinates_in_tiles);
                    }
                    else if(current_selected_world_editor_object_ == STOCKPILE)
                    {
                        world_map_[1][mouse_position_in_vector] = std::make_shared<Stockpile>(mouse_coordinates_in_tiles);
                    }
                }
            }

        }
    }


    if(inputs_.getHeldMouseButtons()[SDL_BUTTON_LEFT])
    {
        Point mouse_selection_start_coordinates_in_tiles = (inputs_.getMouseSelectionStart() -
                                                        (inputs_.getMouseSelectionStart() % config_.getTileSize()))/ config_.getTileSize();
        Point mouse_selection_end_coordinates_in_tiles = (inputs_.getMouseSelectionEnd() -
                                                        (inputs_.getMouseSelectionEnd() % config_.getTileSize()))/ config_.getTileSize();

        //difference in tiles
        Point diff = mouse_selection_end_coordinates_in_tiles - mouse_selection_start_coordinates_in_tiles;

        //position of the start and end of the selection from the up left corner to the bottom right corner
        Point map_selection_start;
        Point map_selection_end;

       if(diff.x > 0 && diff.y > 0)
        {
            map_selection_start = mouse_selection_start_coordinates_in_tiles;
            map_selection_end = mouse_selection_end_coordinates_in_tiles;
        }

        else if(diff.x > 0 && diff.y < 0)
        {
            map_selection_start = mouse_selection_start_coordinates_in_tiles + Point{0,diff.y};
            map_selection_end = mouse_selection_start_coordinates_in_tiles + Point{diff.x, 0};
        }
        else if(diff.x < 0 && diff.y < 0)
        {
            map_selection_start = mouse_selection_start_coordinates_in_tiles + Point{diff.x,diff.y};
            map_selection_end = mouse_selection_start_coordinates_in_tiles;
        }
        else if(diff.x < 0 && diff.y > 0)
        {
            map_selection_start = mouse_selection_start_coordinates_in_tiles + Point{diff.x,0};
            map_selection_end = mouse_selection_start_coordinates_in_tiles + Point{0, diff.y};
        }

        if(diff.x != 0 && diff.y != 0)
        {
            for(int x = map_selection_start.x ; x <= map_selection_end.x ; x++)
            {
                for(int y = map_selection_start.y ; y <= map_selection_end.y ; y++)
                {
                    switch(current_selected_world_editor_object_)
                    {
                        case TREE:
                            world_map_[1][y * config_.getMapSize().y + x] = std::make_shared<Tree>(Point{x,y});
                            break;
                        case STOCKPILE:
                            world_map_[1][y * config_.getMapSize().y + x] = std::make_shared<Stockpile>(Point{x,y});
                            break;
                        case PLAINS:
                            world_map_[0][y * config_.getMapSize().y + x] = std::make_shared<Plains>(Point{x,y});
                            break;
                    }

                }
            }
        }

    }

    if(inputs_.getPressedKeys()[SDL_SCANCODE_X] || inputs_.getPressedMouseButtons()[SDL_BUTTON_MIDDLE])
    {

        world_map_[1][mouse_position_in_vector] = nullptr;

    }

    if(inputs_.getPressedKeys()[SDL_SCANCODE_F] && SDL_GetTicks() - last_time_npc_creation_ > 150)
    {
        npcs_list_.push_back(Peasant{mouse_coordinates_in_tiles});
        last_time_npc_creation_ = SDL_GetTicks();
    }


    if(inputs_.getPressedMouseButtons()[SDL_BUTTON_RIGHT])
    {
        if(selected_npc_ != nullptr)
        {
            if(SDL_GetTicks() - selected_npc_->getTimeOfLastOrder() > 100)
            {
                if(world_map_[1][mouse_position_in_vector] != nullptr)
                {
                    if(world_map_[1][mouse_position_in_vector]->getType() == TREE)
                    {
                        selected_npc_->setObjectGoal(world_map_[1][mouse_position_in_vector]);
                    }
                }

                selected_npc_->setGoal(mouse_coordinates_in_tiles);
                findPath(*selected_npc_);
                selected_npc_->setTimeOfLastOrder(SDL_GetTicks());
            }
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


    elapsed_time_ = SDL_GetTicks() - get_ticks_previous_frame_;

    chrono_for_pathfinding_ += elapsed_time_;
    chrono_movement_ += elapsed_time_;
    fps_ = 1000/elapsed_time_;
    //std::cout << fps_ << std::endl;
    get_ticks_previous_frame_ = SDL_GetTicks();

    if( chrono_for_pathfinding_> 100)
    {
        for(auto &peasant : npcs_list_)
        {
            if(peasant.getGoal() != peasant.getPosition())
                findPath(peasant);
        }
         chrono_for_pathfinding_ = 0;
    }

    if(chrono_movement_ > 70)
    {

        for(auto &peasant : npcs_list_)
        {
            peasant.update(npcs_list_, world_map_);
        }

        chrono_movement_ = 0;

    }


    for(auto it = world_map_[1].begin() ; it != world_map_[1].end() ; ++it)
    {
        if((*it) != nullptr)
        {
            if((*it)->getType() == TREE)
            {
                std::shared_ptr<Tree> tree = std::dynamic_pointer_cast<Tree>((*it));
                if(!tree->update()) //if update() return false, the tree is dead
                {
                    (*it) = nullptr;
                }
            }

            else if((*it)->getType() == WOOD_WALL_BLUEPRINT &&
               std::dynamic_pointer_cast<Blueprint>((*it))->isBuilt())
            {
               (*it) = std::make_shared<Wall>((*it)->getPosition());
            }
        }
    }
}


void Game::draw(Graphics& graphics, Config& config)
{
    graphics_.clear();
    graphics_.drawTiles(config, world_map_, pathfinding_nodes_);

    graphics_.drawNpcs(config_, npcs_list_, selected_npc_);
    //graphics_.drawNodes(closed_nodes_map, open_nodes_map);
    graphics_.drawInputs(inputs_);
    world_editor_->render(graphics_);
    graphics_.flip();
}



bool Game::findPath(Peasant& peasant)
{
    Point start = peasant.getPosition();
    Point finish = peasant.getGoal();
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


    int closed_nodes_map[config_.getMapSize().x][config_.getMapSize().y];
    int open_nodes_map[config_.getMapSize().x][config_.getMapSize().y];
    int dir_map[config_.getMapSize().x][config_.getMapSize().y];
    // reset the node maps
    for(y=0;y<config_.getMapSize().y;y++)
    {
        for(x=0;x<config_.getMapSize().x;x++)
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

    searched_tiles = 0;
    delete n0;
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

        if(x==finish.x && y==finish.y)
        {
            // generate the path from finish to start
            // by following the directions

            std::vector<DIRECTION> pathfinding_vector;
            while(!(x==start.x && y==start.y))
            {
                j=dir_map[x][y];

                //c='0'+(j+8/2)%8;
                //path=c+path;
                auto it = pathfinding_vector.begin();
                it = pathfinding_vector.insert(it,static_cast<DIRECTION>((j+4)%8) );

                x+=xMovement[j];
                y+=yMovement[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
            peasant.setPathfinding(pathfinding_vector);
            return true;
        }

        if (searched_tiles > 10000)
        {
            /*if(peasant.getRandomGoalsTried() > 8)
            {

                peasant.noGoal();
                std::cout << "can't find path" << std::endl;
                return false;
            }
            else
            {
                std::cout << "can't find path, searching close one" << std::endl;
                peasant.newRandomGoal();
                return false;
            }*/

                peasant.noGoal();
                std::cout << "can't find path" << std::endl;
                return false;

        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<8;i++)
        {
            new_pos = Point{x+xMovement[i], y+yMovement[i]};
            bool tile_is_solid = false;
            if(!(new_pos.x<0 || new_pos.x>99 || new_pos.y<0 || new_pos.y>99))
            {
                std::shared_ptr<GameObject> tile_at_new_pos = world_map_[1][new_pos.y * config_.getMapSize().y + new_pos.x];


                if(tile_at_new_pos!= nullptr)
                {

                    tile_is_solid = tile_at_new_pos->isSolid();


                    if(tile_at_new_pos == peasant.getObjectGoal())
                    {
                        std::vector<DIRECTION> pathfinding_vector;

                        while(!(x==start.x && y==start.y))
                        {
                            j=dir_map[x][y];

                            auto it = pathfinding_vector.begin();
                            it = pathfinding_vector.insert(it,static_cast<DIRECTION>((j+4)%8) );

                            x+=xMovement[j];
                            y+=yMovement[j];
                        }

                        // garbage collection
                        delete n0;
                        // empty the leftover nodes
                        while(!pq[pqi].empty()) pq[pqi].pop();
                        peasant.setPathfinding(pathfinding_vector);
                        return true;
                    }
                }

            }
            //auto it = find_if(npcs_list_.begin(), npcs_list_.end(), [&new_pos](const Peasant& peas){ return peas.getPosition() == new_pos;});
            if(!(new_pos.x<0 || new_pos.x>99 || new_pos.y<0 || new_pos.y>99 || closed_nodes_map[new_pos.x][new_pos.y]==1)
               && !tile_is_solid)
            {

                /*!solid_map_[new_pos.x + new_pos.y * config_.getMapSize().y]*/
                searched_tiles++;
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
                    delete m0;

                }
                else if(open_nodes_map[new_pos.x][new_pos.y]>m0->getFScore())
                {

                    // update the priority info
                    open_nodes_map[new_pos.x][new_pos.y]=m0->getFScore();
                    // update the parent direction info
                    dir_map[new_pos.x][new_pos.y]=(i+8/2)%8;

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
                    delete m0;
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }

}
