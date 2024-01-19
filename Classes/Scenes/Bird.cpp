	#ifndef __BIRD_H__
	#define __BIRD_H__

	#include "cocos2d.h"
	#include <cocostudio/SimpleAudioEngine.h>

		class Bird : public cocos2d::Sprite {
		public:
			Bird(cocos2d::Layer * layer);

			void Fall();
			void Fly() { CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f); CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Wing.mp3"); isFalling = false; };
			void StopFlying() { isFalling = true; }

			void cutInHalf();

		private:
			cocos2d::Size visibleSize;
			cocos2d::Vec2 origin;

			cocos2d::Sprite* floppyBirol;
			bool isFalling;

		};

	#endif //  __BIRD_H__


