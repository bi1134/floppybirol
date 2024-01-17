#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"
#include <AudioEngine.h>
#include <cocostudio/SimpleAudioEngine.h>
USING_NS_CC;

Scene* SplashScene::createScene()
{
	//scene is auto release obj
	auto scene = Scene::create();

	//layer is auto release obj
	auto layer = SplashScene::create();

	//add layer as child to scene
	scene->addChild(layer);

	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s \n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SplashScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Hit.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Dissapear.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Point.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Wing.mp3");

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::GoToMainMenuScene), DISPLAY_TIME_SPLASH_SCENE);

	auto backgroundSprite = Sprite::create("SplashScene.png");
	backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	backgroundSprite->setScale(0.95);

	this->addChild(backgroundSprite);

	return true;
}

void SplashScene::GoToMainMenuScene(float dt)
{
	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}
