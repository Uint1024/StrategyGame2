#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

class Config
{
public:
    Dimension getTileSize() { return tile_size; }
    Dimension getScreenSize() { return screen_size_; }
    Dimension getMapSize() { return map_size_; }
private:
    const Dimension tile_size{20,20};
    const Dimension screen_size_{640,480};
    const Dimension map_size_{200,200};

};


#endif // CONFIG_H_INCLUDED
