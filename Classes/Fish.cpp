#include "Fish.h"

using namespace cocos2d;

Fish::Fish() {}
Fish::~Fish() {}
/// <summary>
/// Khởi tạo con trỏ FishingRod
/// </summary>
/// <param name="pikachu">Con trỏ kiểu Pikachu</param>
/// <returns>Nếu thành công trả về con trỏ trỏ đến một đối tượng FishingRod.
///Nếu không thành công trả về con trỏ NULL.</returns>
Fish* Fish::create()
{
	Fish* pFish = new Fish();
	if (pFish->initWithSpriteFrameName("Fish4_1.png"))
	{
		pFish->autorelease();
		pFish->initOptions();
		return pFish;
	}
	else
	{
		CC_SAFE_DELETE(pFish);
		return NULL;
	}
}

void Fish::initOptions()
{
	//Lấy kích thước màn hình gán vào a và b.
	a = Director::getInstance()->getVisibleSize().width;
	b = Director::getInstance()->getVisibleSize().height;
	this->setScale(0.6f);
	// Đặt hình ảnh ngẫu nhiên cho cá.
	switch (RandomHelper::random_int(1, 4))
	{
	case 1:
		this->setSpriteFrame("Fish1_1.png");
		break;
	case 2:
		this->setSpriteFrame("Fish2_1.png");
		break;
	case 3:
		this->setSpriteFrame("Fish3_1.png");
		break;
	default:
		break;
	}
	//Phần lệch của body so với tâm.
	Vec2 bodyOffet;

	//Tạo thời gian di chuyển ngẫu nhiên cho cá:
	float moveTime = RandomHelper::random_real(4.0f, 8.0f);
	//Tạo hướng đi ngẫu nhiên cho cá.
	trend = RandomHelper::random_int(0, 1);
	if (trend == 0) // Nếu là cá đi từ trái sang phải:
	{
		bodyOffet = Vec2(this->getContentSize().width / 4, 0);
		this->setPosition(-0.2f * a, RandomHelper::random_int(0, b/2));
		this->runAction(Sequence::createWithTwoActions(MoveBy::create(moveTime, Vec2(1.4f *a, 0)), CallFunc::create(CC_CALLBACK_0(Fish::whenFinished, this))));
	}
	else // Nếu là cá đi từ phải sang trái:
	{
		bodyOffet = Vec2(-this->getContentSize().width / 4, 0);
		this->setFlipX(true);
		this->setPosition(1.2f * a, RandomHelper::random_int(0, b / 2));
		this->runAction(Sequence::createWithTwoActions(MoveBy::create(moveTime, Vec2(- 1.4f *a, 0)), CallFunc::create(CC_CALLBACK_0(Fish::whenFinished, this))));
	}
	//Tạo body cho cá.
	fishBody = PhysicsBody::createBox(Size(this->getContentSize().height / 2, this->getContentSize().height / 2), PhysicsMaterial(0, 0, 0), bodyOffet);
	fishBody->setCategoryBitmask(eObjectBitmask::FISH);
	fishBody->setCollisionBitmask(0);
	fishBody->setContactTestBitmask(eObjectBitmask::LINE | eObjectBitmask::HOOK);
	this->setPhysicsBody(fishBody);
}

/// <summary>
/// Hàm update vị trí của cá theo vị trí móc câu.
/// Có hiệu lực khi con cá bị dính móc câu.
/// </summary>
///<param name="hookPosition">Vec2 vị trí của móc câu</param>
/// <returns>None</returns>
void Fish::Update(Vec2 hookPosition)
{
	if (isCollistion)
	{
		this->setPosition(hookPosition.x, hookPosition.y);
	}
}
/// <summary>
/// Gọi khi con cá va chạm với dòng kẻ bờ (Khi kéo cá lên bờ).
/// +Tham số: None.
/// +Trả về: None.
/// </summary>
void Fish::whenCollideWithLine()
{
	// Để vị trí của cá ko dính vào móc câu nữa.
	isCollistion = false;
	//Cho cá nhảy vào rọ, sau đó gọi hàm whenFinised().
	this->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(a / 8, 9 * b / 10)), CallFunc::create(CC_CALLBACK_0(Fish::whenFinished, this))));
}

/// <summary>
/// Gọi khi con cá va chạm với móc câu.
/// +Tham số: None.
/// +Trả về: None.
/// </summary>
void Fish::whenCollideWithHook()
{
	//Dừng mọi action lại.
	this->stopAllActions();
	//Cho cá ngóc đầu lên trời
	this->scheduleOnce(schedule_selector(Fish::setRotation90), 0.01f);
	isCollistion = true;
}

/// <summary>
/// Hàm gán biến isFinished = true.
/// Gọi khi con cá đi hết quãng đường hoặc được câu xong.
/// +Tham số: None.
/// +Trả về: None.
/// </summary>
void Fish::whenFinished()
{
	isFinished = true;
}

/// <summary>
/// Hàm cho cá ngóc đầu lên trời.
/// </summary>
void Fish::setRotation90(float dt)
{
	if (trend == 0)
		this->setRotation(-90);
	else
		this->setRotation(90);
}
