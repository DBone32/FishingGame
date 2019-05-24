#include "PlayScene.h"

USING_NS_CC;

Scene* PlayScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, 0.0f));

	// 'layer' is an autorelease object
	auto layer = PlayScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//Lấy kích thước màn hình gán vào a và b.
    a = Director::getInstance()->getVisibleSize().width;
	b = Director::getInstance()->getVisibleSize().height;


	//MenuBackground
	auto background = Sprite::create("images/background.png");
	background->setScaleX(a / background->getContentSize().width);
	background->setScaleY(b / background->getContentSize().height);
	background->setPosition(a/2, b/2);
	this->addChild(background);

	//ScoreLabel hiển thị điểm
	scoreLabel = LabelTTF::create("0", "fonts/Marker Felt.ttf", 100);
	scoreLabel->setColor(ccc3(0, 0, 0 ));
	scoreLabel->setScale(b/ (scoreLabel->getContentSize().height * 10));
	scoreLabel->setPosition(a/8, 9*b/10);
	this->addChild(scoreLabel);

	//Load file chứa hình ảnh và thông tin của hình ảnh.
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/Pikachu.plist", "images/Pikachu.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/CanCau.plist", "images/CanCau.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/Fish.plist", "images/Fish.png");

	pikachu = Pikachu::create();
	this->addChild(pikachu);
	
	//Tạo bộ lắng nghe sự kiện va chạm vật lý.
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//Tạo line body
	auto lineBody = PhysicsBody::createBox(Size(a, 2), PhysicsMaterial(0, 0, 0), Vec2(0 , 0.3f * b));
	lineBody->setCategoryBitmask(eObjectBitmask::LINE);
	lineBody->setCollisionBitmask(0);
	lineBody->setContactTestBitmask(eObjectBitmask::FISH);
	this->setPhysicsBody(lineBody);

	//Lập lịch gọi hàm tạo cá sau 3s.
	this->schedule(schedule_selector(PlayScene::createFish), 3);

	//Goi phương thức update của scene.
	this->scheduleUpdate();

    return true;
}
/// <summary>
/// Update scene qua từng frame
/// </summary>
void PlayScene::update(float dt)
{
	//Duyệt tất cả cá, cá nào hết nhiệm vụ thì xóa bỏ.
	for (std::list<Fish*>::iterator fish = listFish.begin(); fish != listFish.end(); fish++)
	{
		if ((*fish)->isFinished)
		{
			tempFish = (*fish);
			listFish.remove(*fish);
			tempFish->removeFromParent();
			break;
		}
	}
	//Gọi hàm update của tất cả cá.
	for (std::list<Fish*>::iterator fish = listFish.begin(); fish != listFish.end(); fish++)
	{
		(*fish)->Update(pikachu->getHookPosition());
	}
	pikachu->Update();
}

/// <summary>
/// Hàm tăng điểm. Gọi khi câu được cá.
/// Tăng điểm một số ngẫu nhiên từ 69 đến 96.
/// </summary>
void PlayScene::tangDiem()
{
	score += RandomHelper::random_int(69, 96);
	scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
}

/// <summary>
/// Hàm tạo cá.
/// </summary>
void PlayScene::createFish(float dt)
{
	Fish* pFish = Fish::create();
	this->addChild(pFish, 1);
	listFish.push_back(pFish);
}

/// <summary>
/// Hàm xử lý các va chạm vật lý.
/// Được gọi khi có một va chạm xảy ra.
/// </summary>
bool PlayScene::onContactBegin(PhysicsContact & contact)
{
	auto shapeA = contact.getShapeA();
	auto shapeB = contact.getShapeB();
	//Nếu va chạm là cá và móc câu.
	if ((shapeA->getCategoryBitmask() == eObjectBitmask::HOOK && shapeB->getCategoryBitmask() == eObjectBitmask::FISH))
	{
		if (pikachu->hookIsEmpty)
		{
			//Lấy ra đối tượng có từ body va chạm.
			auto sprite = shapeB->getBody()->getNode();
			auto fish = static_cast<Fish*>(sprite);
			//Gọi phương thức lúc va chạm.
			fish->whenCollideWithHook();
			//Cho móc câu biến mất.
			pikachu->setOpacityForHook(0);
			//Móc câu không còn trống nữa.
			pikachu->hookIsEmpty = false;
		}
	}
	else if ((shapeB->getCategoryBitmask() == eObjectBitmask::HOOK && shapeA->getCategoryBitmask() == eObjectBitmask::FISH))
	{
		if (pikachu->hookIsEmpty)
		{
			//Lấy ra đối tượng có từ body va chạm.
			auto sprite = shapeA->getBody()->getNode();
			auto fish = static_cast<Fish*>(sprite);
			//Gọi phương thức lúc va chạm.
			fish->whenCollideWithHook();
			//Cho móc câu biến mất.
			pikachu->setOpacityForHook(0);
			//Móc câu không còn trống nữa.
			pikachu->hookIsEmpty = false;
		}
	}
	//Nếu là va chạm giữ LINE và cá.
	else if ((shapeA->getCategoryBitmask() == eObjectBitmask::LINE && shapeB->getCategoryBitmask() == eObjectBitmask::FISH))
	{
		//Lấy ra đối tượng có từ body va chạm.
		auto sprite = shapeB->getBody()->getNode();
		auto fish = static_cast<Fish*>(sprite);

		tangDiem();
		//Gọi phương thức lúc va chạm.
		fish->whenCollideWithLine();
		//Cho móc câu xuất hiện trở lại.
		pikachu->setOpacityForHook(255);
		//Cho móc câu trống trở lại
		pikachu->hookIsEmpty = true;
	}
	else if ((shapeB->getCategoryBitmask() == eObjectBitmask::LINE && shapeA->getCategoryBitmask() == eObjectBitmask::FISH))
	{
		//Lấy ra đối tượng có từ body va chạm.
		auto sprite = shapeA->getBody()->getNode();
		auto fish = static_cast<Fish*>(sprite);

		tangDiem();
		//Gọi phương thức lúc va chạm.
		fish->whenCollideWithLine();
		//Cho móc câu xuất hiện trở lại.
		pikachu->setOpacityForHook(255);
		//Cho móc câu trống trở lại
		pikachu->hookIsEmpty = true;
	}

	return true;
}