#ifndef __GAME_OVER_LAYER_H__
#define __GAME_OVER_LAYER_H__

#include "cocos2d.h"

class GameOverLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    // ... other member variables and functions
    CREATE_FUNC(GameOverLayer);
};

#endif // __GAME_OVER_LAYER_H__