#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"
#include "Pikachu.h"
#include "Fish.h"


class PlayScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(PlayScene);

	bool onContactBegin(PhysicsContact &contact);
private:
	void update(float dt);
	void tangDiem();

	Pikachu *pikachu;

	//List để quản lý các đối tượng cá.
	std::list <Fish*> listFish;
	void createFish(float dt);
	//Con trỏ trung gian dùng để xóa fish.
	Fish *tempFish;

	//Điểm của người chơi.
	int score = 0;
	//Label hiển thị điểm.
	LabelTTF *scoreLabel;

	//Chiều rộng màn hình
	int a;
	//Chiều cao màn hình
	int b;
};

#endif // __HELLOWORLD_SCENE_H__
