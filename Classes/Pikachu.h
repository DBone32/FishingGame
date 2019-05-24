#ifndef __PIKACHU_H__
#define __PIKACHU_H__

#include "cocos2d.h"
#include "Definitions.h"
#include "FishingRod.h"

USING_NS_CC;

class Pikachu : public Sprite {
public:
	Pikachu();
	~Pikachu();
	static Pikachu* create();
	void initOptions();
	void onMouseScroll(Event *event);
	void Update();
	Vec2 getHookPosition();

	void setOpacityForHook(int opacity);
	//Biến kiểm tra xem móc câu có đang trống hay không.
	int hookIsEmpty = 1;

private:

	//Vector chứa các sprite frame.
	Vector<SpriteFrame*> taiVector, matVector;

	//Bộ phận trên người Pikachu.
	Sprite *tayPikachu, *taiPikachu, *matPikachu;
	//Cần câu cá.
	FishingRod *fishingRod;

	//Chiều rộng màn hình
	int a;
	//Chiều cao màn hình
	int b;
};

#endif