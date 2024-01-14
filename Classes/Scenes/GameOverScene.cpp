#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"
#include <AudioEngine.h>
USING_NS_CC;

Scene* GameOverScene::createScene()
{
	//scene is auto release obj
	auto scene = Scene::create();

	//layer is auto release obj
	auto layer = GameOverScene::create();

	//add layer as child to scene
	scene->addChild(layer);

	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s \n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameOverScene::init()
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

	auto retryItem = MenuItemImage::create("RetryButton.png", "RetryButtonClicked.png", CC_CALLBACK_1(GameOverScene::GoToGameScene, this));
	retryItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto menu = Menu::create(retryItem, NULL);
	menu->setPosition(Point::ZERO);

	this->addChild(menu);


	return true;
}

void GameOverScene::GoToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0, scene));
}

void GameOverScene::GoToGameScene(cocos2d::Ref* sender)
{
	auto scene = GameScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0, scene));
}