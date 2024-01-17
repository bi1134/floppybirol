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
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));

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

	score = 0;

	scoreLabel = Label::createWithTTF("0", "fonts/ARCADECLASSIC.TTF", 24);
	scoreLabel->setScale(2);
	scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - scoreLabel->getContentSize().height - 30));
	this->addChild(scoreLabel, 10000);

	outlineLabel = Label::createWithTTF("0", "fonts/ARCADECLASSIC.TTF", 24);
	outlineLabel->setScale(scoreLabel->getScale());
	outlineLabel->setPosition(Vec2(scoreLabel->getPositionX() + 2, scoreLabel->getPositionY() - 2)); // slightly offset (for shadow effect)
	outlineLabel->setColor(Color3B::BLACK); // set the color to black for the outline
	this->addChild(outlineLabel, 9999);

	UserDefault* def = UserDefault::getInstance();

	highScore = def->getIntegerForKey("HIGHSCORE", 0);

	highScoreLabel = Label::createWithTTF("0", "fonts/ARCADECLASSIC.TTF", 24);
	highScoreLabel->setScale(2);
	highScoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - highScoreLabel->getContentSize().height));
	highScoreLabel->setVisible(false);
	this->addChild(highScoreLabel, 20000);

	highScoreOutlineLabel = Label::createWithTTF("0", "fonts/ARCADECLASSIC.TTF", 24);
	highScoreOutlineLabel->setScale(highScoreLabel->getScale());
	highScoreOutlineLabel->setPosition(Vec2(highScoreLabel->getPositionX() + 2, highScoreLabel->getPositionY() - 2)); // slightly offset (for shadow effect)
	highScoreOutlineLabel->setColor(Color3B::BLACK); // set the color to black for the outline
	highScoreOutlineLabel->setVisible(false);
	this->addChild(highScoreOutlineLabel, 9998);

	crown = Sprite::create("crown.png");
	crown->setPosition(Vec2(visibleSize.width + origin.x, visibleSize.height + origin.y));
	crown->setScale(0.05);
	crown->setVisible(false);
	this->addChild(crown);

	def->flush();

	auto menu = Menu::create(retryButton, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->scheduleUpdate();

	return true;
}

void GameScene::SpawnPipe(float dt)
{
	pipe.SpawnPipe(this);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	if (isGameOver)
	{
		return false; // Don't process collision if the game is already over
	}

	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		unscheduleAllCallbacks();

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Hit.mp3");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Dissapear.mp3");

		retryButton->setVisible(true);

		// Pause the physics world
		sceneWorld->setSpeed(0.0f);

		// Schedule the cutInHalf method after a short delay

		scheduleOnce([this, visibleSize](float delay) 
		{
				scoreLabel->setAnchorPoint(Vec2(0, 0.5));
				scoreLabel->setPosition(Vec2(100, visibleSize.height - scoreLabel->getContentSize().height - 30));

				outlineLabel->setAnchorPoint(Vec2(0, 0.5));
				outlineLabel->setPosition(Vec2(scoreLabel->getPositionX() + 2, scoreLabel->getPositionY() - 2));

				highScoreLabel->setAnchorPoint(Vec2(1, 0.5));
				highScoreLabel->setPosition(Vec2(visibleSize.width - 100, visibleSize.height - highScoreLabel->getContentSize().height - 30));

				highScoreOutlineLabel->setAnchorPoint(Vec2(1, 0.5));
				highScoreOutlineLabel->setPosition(Vec2(highScoreLabel->getPositionX() + 2, highScoreLabel->getPositionY() - 2));
				
				crown->setPosition(Vec2(highScoreLabel->getPositionX() - 40, highScoreLabel->getPositionY() + 25));

				highScoreLabel->setVisible(true);
				highScoreOutlineLabel->setVisible(true);
				crown->setVisible(true);
		}, 0.05f, "label_transition");

		scheduleOnce([this](float delay) {
			bird->cutInHalf();
			isGameOver = true;


			}, 0.1f, "cut_in_half");

		return false; // Prevent further processing of collisions
	}
	else if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && POINT_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && POINT_COLLISION_BITMASK == a->getCollisionBitmask()))
	{

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Point.mp3");
		score++;
		updateScoreLabel();
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

	updateScoreLabel();
}

void GameScene::updateScoreLabel()
{
	scoreLabel->setString(StringUtils::format("%d", score));

	if (outlineLabel)
	{
		outlineLabel->setString(scoreLabel->getString());
	}
	else
	{
		CCLOG("Outline label not found");
	}

	if (score > highScore)
	{
		highScore = score;

		UserDefault* def = UserDefault::getInstance();
		def->setIntegerForKey("HIGHSCORE", highScore);
		def->flush();
	}

	highScoreLabel->setString(StringUtils::format("%d", highScore));

	if (highScoreOutlineLabel)
	{
		highScoreOutlineLabel->setString(highScoreLabel->getString());
	}
	else
	{
		CCLOG("High score outline label not found");
	}

}

void GameScene::retryButtonCallback(Ref* sender)
{
	// hide the retry button
	retryButton->setVisible(false);

	// set the flag to indicate that the game is not over
	isGameOver = false;

	// restart the game scene
	auto newScene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));

}

void Bird::cutInHalf()
{
	auto fadeOut = FadeOut::create(0.5f);

	// Run the action on the bird sprite
	floppyBirol->runAction(fadeOut);

	scheduleOnce([this](float delay) {
		floppyBirol->removeFromParent();
		}, 0.5f, "remove_bird");
}
