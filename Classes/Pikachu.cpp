#include "Pikachu.h"

using namespace cocos2d;

Pikachu::Pikachu() {}
Pikachu::~Pikachu() {}

/*
	*Khởi tạo con trỏ Pikachu
	*Tham số: None.
	*Nếu thành công trả về con trỏ trỏ đến một đối tượng Pikachu.
	 Nếu không thành công trả về con trỏ NULL.
*/
Pikachu* Pikachu::create()
{
	Pikachu* pPikachu = new Pikachu();
	if (pPikachu->initWithSpriteFrameName("ThanPikachu.png"))
	{
		pPikachu->autorelease();
		pPikachu->initOptions();
		return pPikachu;
	}
	else
	{
		CC_SAFE_DELETE(pPikachu);
		return NULL;
	}
}

void Pikachu::initOptions()
{
	//Lấy kích thước màn hình gán vào a và b.
	a = Director::getInstance()->getVisibleSize().width;
	b = Director::getInstance()->getVisibleSize().height;

	//Đặt vị trí của Pikachu
	this->setPosition(a/2, 3*b/ 4);
	//Đặt kích thước của Pikachu bằng 1/4 chiều cao màn hình.
	this->setScale(b/ (this->getContentSize().height * 4));
	
	//Tạo sprite tay của Pikachu. Gán vị trí của tay.
	tayPikachu = Sprite::createWithSpriteFrameName("Tay.png");
	tayPikachu->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	//Thêm tay vào thân.
	this->addChild(tayPikachu, 10);

	//Vector sẽ chứa 72 hình ảnh cho hành động vẫy tai.
	taiVector.reserve(72);
	//Mảng chứa tên của các Sprite Frame
	char spriteFrameByName[20] = { 0 };
	//Lần lượt thêm các Sprite Frame vào mảng.
	for (int index = 1; index <= 72; index++)
	{
		//Gán tên của Sprite Frame tương ứng vào mảng spriteFrameByName.
		sprintf(spriteFrameByName, "TaiPikachu_%d.png", index);
		//Thêm sprite tương ứng vào mảng.
		taiVector.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameByName));
	}
	//Tạo hành động cho tai của Pikachu.
	Animation* taiAnimation = Animation::createWithSpriteFrames(taiVector, 0.04f);
	Animate* taiAnimate = Animate::create(taiAnimation);
	//Hành động được lặp đi lặp lại.
	Action *taiAction = RepeatForever::create(taiAnimate);

	//Tạo sprite tai cho Pikachu. Gán vị trí cho nó.
	taiPikachu = Sprite::createWithSpriteFrameName("TaiPikachu_1.png");
	taiPikachu->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);	
	//Cho tai bắt đầu hành động.
	taiPikachu->runAction(taiAction);
	//Thêm tai vào thân.
	this->addChild(taiPikachu, -1);

	//Thêm các Sprite Frame nháy mắt vào matVector.
	matVector.reserve(3);
	matVector.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("MatNham.png"));
	matVector.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("MatNham.png"));
	matVector.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("MatMo.png"));
	//Tạo hành động nháy mắt.
	Animation* matAnimation = Animation::createWithSpriteFrames(matVector, 0.05f);
	Animate *matAnimate = Animate::create(matAnimation);
	//Cho mắt nháy sau 5 giây.
	auto matAction = Sequence::createWithTwoActions(DelayTime::create(5.0f), matAnimate);
	//Tạo sprite mắt cho Pikachu. Gán vị trí cho nó.
	matPikachu = Sprite::createWithSpriteFrameName("MatMo.png");
	matPikachu->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	//Mỗi 5 giay nháy mắt một lần.
	matPikachu->runAction(RepeatForever::create(matAction));
	//Thêm mắt vào thân.
	this->addChild(matPikachu, 4);

	//Tạo cần câu cá.
	fishingRod = FishingRod::create(this);
	fishingRod->setPosition(this->getContentSize().width/ 2, this->getContentSize().width / 2 - 75);
	this->addChild(fishingRod, 9);

	// Khởi tạo một mouse event.
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseScroll = CC_CALLBACK_1(Pikachu::onMouseScroll, this);
	// Thêm mouse event vào cơ chế event dispatcher(Bộ lắng nghe sự kiện).
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Pikachu::onMouseScroll(Event * event)
{
	fishingRod->onMouseScroll(event);
}

void Pikachu::Update()
{
	fishingRod->Update();
}
/// <summary>
/// Hàm trả về vị trí của móc câu theo tọa độ màn hình.
/// +Tham số: None.
/// +Trả về: Vec2 vị trí của móc câu theo tọa độ màn hình.
/// </summary>
Vec2 Pikachu::getHookPosition()
{
	Vec2 ketqua;
	ketqua.x = this->getPositionX() + this->getScale()*(fishingRod->getHookPosition().x - this->getContentSize().width /2);
	ketqua.y = this->getPositionY() + this->getScale()*(fishingRod->getHookPosition().y - this->getContentSize().height /2);
	return ketqua;
}

/// <summary>
/// Hàm set độ trong suốt của móc câu.
/// </summary>
///<param name="opacity">Độ trong của móc câu (0-255)</param>
/// <returns>None</returns>
void Pikachu::setOpacityForHook(int opacity)
{
	fishingRod->setOpacityForHook(opacity);
}
