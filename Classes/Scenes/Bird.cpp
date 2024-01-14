#include "Bird.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"
#include <AudioEngine.h>
USING_NS_CC;

Bird::Bird(cocos2d::Layer *layer)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	floppyBirol = Sprite::create("birol.png");

	floppyBirol->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto floppyBody = PhysicsBody::createCircle(floppyBirol->getContentSize().width / 2);
	floppyBody->setCollisionBitmask(BIRD_COLLISION_BITMASK);
	floppyBody->setContactTestBitmask(true);


	floppyBirol->setPhysicsBody(floppyBody);

	layer->addChild(floppyBirol, 100);

	isFalling = true;

}

void Bird::Fall()
{
	if (isFalling == true)
	{
		floppyBirol->setPositionX(visibleSize.width / 2 + origin.x);
		floppyBirol->setPositionY(floppyBirol->getPositionY() - (BIRD_FALLING_SPEED * visibleSize.height));
	}
	else
	{
		floppyBirol->setPositionX(visibleSize.width / 2 + origin.x);
		floppyBirol->setPositionY(floppyBirol->getPositionY() + (BIRD_FLYING_SPEED * visibleSize.height));
	}

}


