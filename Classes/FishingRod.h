#ifndef __FISHINGROD_H__
#define __FISHINGROD_H__

#include "cocos2d.h"
#include "Definitions.h"

USING_NS_CC;

class FishingRod : public Sprite {
public:
	FishingRod();
	~FishingRod();
	static FishingRod* create(Sprite *pikachu);
	void initOptions(Sprite *pikachu);
	void onMouseScroll(Event *event);
	void Update();
	Vec2 getHookPosition();
	void setOpacityForHook(int opacity);
private:
	//Body của móc câu, để bắt sự kiện va chạm với cá.
	PhysicsBody* hookBody;
	Vec2 getTopPositionOfRod();
	//Bộ phận của cần câu.
	Sprite *hookSprite, *lineSprite;
	//Action quay đi quay lại của cần.
	Action *rotateAction;
	bool isRotate;
	//Vị trí của móc câu.
	Vec2 hookPosition;
	// Độ dài của cây gậy.
	int lengthOfRod;
	
	//Độ dài ban đầu của dây câu.
	int baseLengthOfLine;

	//Độ nhạy của lăn chuột.
	int scrollFactor;

	// Góc quay tối đa của gậy.
	int maxRotation;
	//Chiều rộng màn hình
	int a;
	//Chiều cao màn hình
	int b;
};

#endif