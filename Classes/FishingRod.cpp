#include "FishingRod.h"

using namespace cocos2d;

FishingRod::FishingRod() {}
FishingRod::~FishingRod() {}

/// <summary>
/// Khởi tạo con trỏ FishingRod
/// </summary>
/// <param name="pikachu">Con trỏ kiểu Pikachu</param>
/// <returns>Nếu thành công trả về con trỏ trỏ đến một đối tượng FishingRod.
///Nếu không thành công trả về con trỏ NULL.</returns>
FishingRod* FishingRod::create(Sprite *pikachu)
{
	FishingRod* pFishingRod = new FishingRod();
	if (pFishingRod->initWithSpriteFrameName("CanCau.png"))
	{
		pFishingRod->autorelease();
		pFishingRod->initOptions(pikachu);
		return pFishingRod;
	}
	else
	{
		CC_SAFE_DELETE(pFishingRod);
		return NULL;
	}
}

/// <summary>
/// Khởi tạo các thông số cho FishingRod.
/// Gọi trong hàm khởi tạo cần câu (create()).
/// </summary>
/// <param name="pikachu">Một con trỏ kiểu Pikachu</param>
/// <returns>None</returns>
void FishingRod::initOptions(Sprite *pikachu)
{
	//Lấy kích thước màn hình gán vào a và b.
	a = Director::getInstance()->getVisibleSize().width;
	b = Director::getInstance()->getVisibleSize().height;
	
	//Gán giá trị độ dài của gậy theo đúng kích thước ảnh;
	lengthOfRod = 690;
	//Khởi tạo giá trị độ dài ban đầu cho dây câu bằng 1/5 chiều cao màn hình.
	baseLengthOfLine = 200;

	//Khởi tạo các giá trị khác.
	maxRotation = 40;
	scrollFactor = 30;
	isRotate = true;
	//Tạo action quay đi quay lại của cần câu(FishingRod).
	rotateAction = RepeatForever::create(Sequence::createWithTwoActions(RotateBy::create(3.0f, 2*maxRotation), RotateBy::create(3.0f, -2*maxRotation)));
	
	//Gán thông số góc quay ban đầu và scale cho cần.
	this->setRotation(-maxRotation);
	this->setScale(3.0f);
	//Chạy action rotateAction.
	this->runAction(rotateAction);

	//Khởi tạo đối tượng móc câu (hookSprite).
	hookSprite = Sprite::createWithSpriteFrameName("16639368.png");
	//Gán vị trí của móc câu.
	hookPosition = Vec2(getTopPositionOfRod().x, 200);
	hookSprite->setPosition(hookPosition.x, hookPosition.y);
	//Thêm đối tượng vào đối tượng này pikachu đã truyền vào.
	pikachu->addChild(hookSprite, 15);

	//Body cho móc câu.
	//Tạo body cho cá.
	hookBody = PhysicsBody::createBox(Size(hookSprite->getContentSize().width, hookSprite->getContentSize().height), PhysicsMaterial(0, 0, 0));
	hookBody->setCategoryBitmask(eObjectBitmask::HOOK);
	hookBody->setCollisionBitmask(0);
	hookBody->setContactTestBitmask(eObjectBitmask::FISH);
	hookSprite->setPhysicsBody(hookBody);


	//Khởi tạo đối tượng dây câu(lineSprite).
	lineSprite = Sprite::createWithSpriteFrameName("Day.png");
	lineSprite->setPosition(getTopPositionOfRod().x, (hookSprite->getPositionY() + getTopPositionOfRod().y) / 2);
	//Chỉnh chiều dài dây câu cho phù hợp.
	lineSprite->setScaleY((getTopPositionOfRod().y - hookSprite->getPositionY()) / this->getContentSize().height);
	//Thêm đối tượng vào đối tượng này pikachu đã truyền vào.
	pikachu->addChild(lineSprite, 14);
}


/// <summary>
/// Hàm trả về vị trí đỉnh của cần câu.
/// +Tham số : None.
/// +Trả về: Vec2 vị trí đỉnh của cần câu.
/// </summary>
Vec2 FishingRod::getTopPositionOfRod()
{
	Vec2 ketQua;
	ketQua.x = this->getPosition().x + sinf(this->getRotation()*3.14 / 180)*lengthOfRod;
	ketQua.y = this->getPosition().y + cosf(this->getRotation()*3.14 / 180)*lengthOfRod;
	return ketQua;
}

/// <summary>
/// Hàm update của cần câu. Gọi trong hàm update của Pikachu.
/// +Tham số: None.
/// +Trả về: None.
/// </summary>
void FishingRod::Update()
{
	//Nếu đang thả móc câu mà cần vẫn đang quay. Cho cần dừng lại.
	if ((hookPosition.y < 200) && (isRotate))
	{
		isRotate = false;
		this->pauseSchedulerAndActions();
	}
	//Nếu móc câu đã kéo hết cỡ mà cần vẫn chưa quay. Cho cần quay trở lại.
	else if ((hookPosition.y >= 200) && (!isRotate))
	{
		isRotate = true;
		this->resumeSchedulerAndActions();
	}
	//Update vị trí X cho móc câu.
	hookSprite->setPositionX(getTopPositionOfRod().x);
	//Update vị trí cho dây câu.
	lineSprite->setPosition(getTopPositionOfRod().x, (hookSprite->getPositionY() + getTopPositionOfRod().y + 50) / 2);
	//Update chiều dài của dây câu.
	lineSprite->setScaleY((getTopPositionOfRod().y - hookSprite->getPositionY() - 50) /lineSprite->getContentSize().height);
}

/// <summary>
/// Hàm trả về vị trí của móc câu theo vị trí pikachu.
/// +Tham số: None.
/// +Trả về: Vec2 vị trí của móc câu theo vị trí pikachu.
/// </summary>
Vec2 FishingRod::getHookPosition()
{
	return Vec2(hookSprite->getPositionX(), hookSprite->getPositionY());
}

void FishingRod::setOpacityForHook(int opacity)
{
	hookSprite->setOpacity(opacity);
}

void FishingRod::onMouseScroll(Event *event)
{
	EventMouse* e = (EventMouse*)event;
	hookPosition.y += scrollFactor * e->getScrollY();
	//Nếu vị trí móc câu cao hơn vị trị max, gán nó = max. 
	if (hookPosition.y > 200)
	{
		hookPosition.y = 200;
	}
	//Nếu vị trí móc câu thấp hơn vị trị min, gán nó = min.
	else if (hookPosition.y < -1000)
	{
		hookPosition.y = -1000;
	}
	hookSprite->setPositionY(hookPosition.y);
}
