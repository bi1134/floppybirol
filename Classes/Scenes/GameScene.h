#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Pipe.h"
#include "Bird.h"


class GameScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);

	void retryButtonCallback(cocos2d::Ref* sender);

private:
	void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };

	void SpawnPipe(float dt);

	bool onContactBegin(cocos2d::PhysicsContact& contact);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void stopFlying(float dt);

	void update(float dt);

	void gameOver();

	bool isGameOver;


	cocos2d::MenuItemImage* retryButton;

	cocos2d::PhysicsWorld* sceneWorld;

	Pipe pipe;

	Bird* bird;

	unsigned int score;

};

#endif //  __GAME_SCENE_H__
