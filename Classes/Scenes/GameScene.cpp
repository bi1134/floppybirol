#include "GameScene.h"
#include "Definitions.h"
#include "GameOverScene.h"
#include "ui/CocosGUI.h"
#include <AudioEngine.h>
USING_NS_CC;

Scene* GameScene::createScene()
{
	//scene is auto release obj
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0,0));


	//layer is auto release obj
	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	//add layer as child to scene
	scene->addChild(layer);

	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s \n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("Background.png");
	backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(backgroundSprite);

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(BIRD_COLLISION_BITMASK);

	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);

	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::SpawnPipe), PIPE_SPAWN_FREQUENCY * visibleSize.width);

	bird = new Bird(this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	isGameOver = false;

	retryButton = MenuItemImage::create("retryButton.png", "retryButtonClicked.png", CC_CALLBACK_1(GameScene::retryButtonCallback, this));
	retryButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	retryButton->setVisible(false);

	auto menu = Menu::create(retryButton, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	gameOverLayer = GameOverLayer::create();
	gameOverLayer->setVisible(false);
	this->addChild(gameOverLayer, 1);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	score = 0;

	this->scheduleUpdate();

	return true;
}

void GameScene::SpawnPipe(float dt)
{
	pipe.SpawnPipe(this);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
	if (isGameOver)
	{
		return false; // Don't process collision if the game is already over
	}

	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		// Stop any ongoing actions or schedules related to gameplay
		unscheduleAllCallbacks();

		this->stopAllActionsByTag();

		// Your game over logic goes here

		// Set gameOverLayer to be visible
		retryButton->setVisible(true);

		// Pause the physics world
		sceneWorld->setSpeed(0.0f);

		isGameOver = true;

		return false; // Prevent further processing of collisions
	}
	else if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && POINT_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && POINT_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		score++;
		CCLOG("point scored: %i", score);
	}

	return true;
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	bird->Fly();

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::stopFlying), BIRD_FLY_DURATION);

	return true;

}

void GameScene::stopFlying(float dt)
{
	bird->StopFlying();
}

void GameScene::update(float dt)
{
	bird->Fall();
}

void GameScene::gameOver()
{
	if (!gameOverLayer->isVisible()) // Ensure the game over layer is not already visible
	{
		// Stop any ongoing actions or schedules related to gameplay

		// Your game over logic goes here

		// Show the game over layer
		gameOverLayer->setVisible(true);
	}
}

void GameScene::retryButtonCallback(Ref* sender)
{
	// Reset the game state (e.g., reset score, position, etc.)
	// ...

	// Hide the retry button
	retryButton->setVisible(false);

	// Set the flag to indicate that the game is not over
	isGameOver = false;

	// Restart the game scene
	auto newScene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));

	// Optionally, you can also perform other actions related to restarting the game here
}