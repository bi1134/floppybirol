#include "Pipe.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"
#include <AudioEngine.h>
USING_NS_CC;

Pipe::Pipe()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

void Pipe::SpawnPipe(cocos2d::Layer* layer)
{
	CCLOG("SPAWN PIPE");

	auto topPipe = Sprite::create("UpperPipe.png");
	auto bottomPipe = Sprite::create("BottomPipe.png");

	auto topPipeBody = PhysicsBody::createBox(Size(topPipe->getContentSize().width * 0.6, topPipe->getContentSize().height));
	auto bottomPipeBody = PhysicsBody::createBox(Size(bottomPipe->getContentSize().width * 0.6, bottomPipe->getContentSize().height));

	auto random = CCRANDOM_0_1();

	if (random < LOWER_SCREEN_PIPE_THRESHOLD)
	{
		random = LOWER_SCREEN_PIPE_THRESHOLD;
	}

	else if (random > UPPER_SCREEN_PIPE_THRESHOLD)
	{
		random = UPPER_SCREEN_PIPE_THRESHOLD;
	}

	auto topPipePosition = (random * visibleSize.height) + (topPipe->getContentSize().height / 2);

	topPipeBody->setDynamic(false);
	bottomPipeBody->setDynamic(false);

	topPipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	bottomPipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	topPipeBody->setContactTestBitmask(true);
	bottomPipeBody->setContactTestBitmask(true);


	topPipe->setPhysicsBody(topPipeBody);
	bottomPipe->setPhysicsBody(bottomPipeBody);

	topPipe->setPosition(Vec2(visibleSize.width + topPipe->getContentSize().width + origin.x, topPipePosition));

	bottomPipe->setPosition(Vec2(topPipe->getPositionX(), topPipePosition - (Sprite::create("birol.png")->getContentSize().height * PIPE_GAP) - topPipe->getContentSize().height));

	layer->addChild(topPipe);
	layer->addChild(bottomPipe);

	auto topPipeAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));
	auto bottomPipeAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));

	topPipe->runAction(topPipeAction);
	bottomPipe->runAction(bottomPipeAction);

	auto pointNode = Node::create();
	auto pointBody = PhysicsBody::createBox(Size(1, Sprite::create("birol.png")->getContentSize().height * PIPE_GAP));

	pointBody->setDynamic(false);
	pointBody->setCollisionBitmask(POINT_COLLISION_BITMASK);
	pointBody->setContactTestBitmask(true);

	pointNode->setPhysicsBody(pointBody);
	pointNode->setPosition(Point(topPipe->getPositionX(), topPipe->getPositionY() - (topPipe->getContentSize().height / 2) - ((Sprite::create("birol.png")->getContentSize().height * PIPE_GAP) / 2)));

	layer->addChild(pointNode);

	auto pointNodeAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));

	pointNode->runAction(pointNodeAction);
}
