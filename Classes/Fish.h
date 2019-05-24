#ifndef __FISH_H__
#define __FISH_H__

#include "cocos2d.h"
#include "Definitions.h"

USING_NS_CC;

class Fish : public Sprite {
public:
	Fish();
	~Fish();
	static Fish* create();
	void initOptions();
	void Update(Vec2 hookPosition);
	void whenCollideWithLine();
	void whenCollideWithHook();
	void whenFinished();

	//Biến kiểm tra xem cá đã va chạm với móc câu hay chưa.
	bool isCollistion = false;

	//Biến kiểm tra xem con cá đã hết nhiệm vụ chưa :v.
	bool isFinished = false;

	//Hướng đi của cá:
	//Bằng 0 ứng với hướng từ trái sang phải.
	//Khác 0 ứng với hướng từ phải sang trái.
	int trend;
private:
	void setRotation90(float dt);
	//Body của cá, để bắt sự kiện va chạm với móc câu.
	PhysicsBody* fishBody;

	//Vector vận tốc của con cá.
	Vec2 velocity;
	//Chiều rộng màn hình
	int a;
	//Chiều cao màn hình
	int b;
};

#endif //__PICTURE_H__