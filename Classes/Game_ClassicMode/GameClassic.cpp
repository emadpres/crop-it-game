#include "GameClassic.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Ball.h"
#include "HelperFunctions.h"
#include "Geometry/Polygon.h"
#include "GameOptions.h"

USING_NS_CC;
using namespace std;

bool GameClassic::init() {
    if (!LayerColor::initWithColor(Color4B::ORANGE)) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _cropper = nullptr;
    _arrows = nullptr;


    auto label = Label::createWithTTF("Game: Classic", "fonts/Marker Felt.ttf", 24);
    label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label);

    _drawingNode = DrawNode::create();
    addChild(_drawingNode);
    InitialGameArea();
    InitialBall();
    SetCropper();
    InitCropper();
    RenderPolygon();

    CCLOG("%d", (int)(_cropper->getRotation()));

    return true;
}

void GameClassic::InitialGameArea() {
    ///////////////// Transparent Square Background (Game Area Shadow)
    const auto gameAreaRect = GetGameAreaRect();
    _drawingNode->drawSolidRect(gameAreaRect.origin, gameAreaRect.origin + gameAreaRect.size, Color4F(50, 50, 50, 0.4));
    /////////////////
    _polygon = new Polygon();
    int w = GetGameAreaSquareWidth();
    _polygon->Initial_Square(GetGameAreaCenter() - Vec2(w / 2, w / 2), w);
    //_polygon->Initial_Custom1(GetGameAreaCenter()-Vec2(w/2,w/2), w);
}

Vec2 GameClassic::GetGameAreaCenter() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = origin + visibleSize / 2 + Vec2(0, visibleSize.height / 8.0);
    return center;
}

int GameClassic::GetGameAreaSquareWidth() {
    return 270 * 2;
}

void GameClassic::RenderPolygon() {
    _drawingNode->clear();
    for (const pair<Vec2, Vec2> &seg: _polygon->GetSegments()) {
        _drawingNode->drawSegment(seg.first, seg.second, 3, Color4F::BLACK);
        _drawingNode->drawSolidCircle(seg.first, 5, 0, 5, Color4F::YELLOW);
        _drawingNode->drawSolidCircle(seg.second, 5, 0, 5, Color4F::RED);
    }
}

void GameClassic::InitialBall() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _ball = Ball::create("ball.png", &_polygon->GetSegments());
    _ball->setPosition(GetGameAreaCenter() + Vec2(100, 100));
    _ball->SetVelocity(Vec2(500, 110));
    addChild(_ball);

    schedule([&](float dt) {
        _ball->MoveBall(dt);
    }, 1.0 / 60, "ball_tick");

//    schedule([&](float dt)
//             {
//                 _ball->SetVelocity(Vec2((GetRand01()-0.5)*120, (GetRand01()-0.5)*120));
//             }, 1, "ball_tick_velo");

}

Vec2 GameClassic::GetCropperOriginalPos() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return origin + Vec2(visibleSize.width / 2, 100);
}

void GameClassic::InitCropper() {
    EventListenerTouchOneByOne *cropperTouchListener = EventListenerTouchOneByOne::create();
    cropperTouchListener->setSwallowTouches(true);

    cropperTouchListener->onTouchBegan = [&](Touch *touch, Event *event) {
        auto target = static_cast<Node *>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        //CCLOG("Location in Node: %f\t%f", locationInNode.x, locationInNode.y);
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        if (rect.containsPoint(locationInNode) && target->isVisible()) {
            // Assume this scenario:
            // 1.Take card A    2.Free card A => we for example may call sth by `afterCardMoveFinished_Action`
            // 3. and Now we again get card A ! ===> we should destroy all suspending action at first and clear card.
            target->stopAllActions();  // Reason in above scenario
            target->setScale(1.7);
            target->setOpacity(100);
            return true;
        }
        return false;
    };

    cropperTouchListener->onTouchMoved = [&](Touch *touch, Event *event) {
        auto target = static_cast<Node *>(event->getCurrentTarget());
        target->setPosition(target->getPosition() + touch->getDelta());
        _tapLabel->setVisible(false);
        _arrows->setVisible(false);
    };

    cropperTouchListener->onTouchEnded = cropperTouchListener->onTouchCancelled = [&](Touch *touch, Event *event) {

        auto target = static_cast<Sprite *>(event->getCurrentTarget());
        Vec2 locationGlobal = touch->getLocation(); // remember: this position is based on (0,0), not Origin
        target->stopAllActions();
        target->setScale(1);
        target->setOpacity(255);
        if (_polygon->IsPointInsidePolygon(locationGlobal)) {
            target->setPosition(GetCropperOriginalPos());
            // target set new direction

            int dir = 0;
            auto directionName = _cropper->getName();
            auto rotationNo = (int)(_cropper->getRotation() / 90.0f);

            CCLOG("%d", (int)(_cropper->getRotation()));

            if (directionName == "line")
            {
                if (rotationNo % 2 == 0)
                    dir = 5;
                else
                    dir = 6;
            } else
            {
                if (rotationNo % 4 == 0)
                    dir = 1;
                else if (rotationNo % 4 == 1)
                    dir = 2;
                else if (rotationNo % 4 == 2)
                    dir = 3;
                else if (rotationNo % 4 == 3)
                    dir = 4;
            }

            CCLOG("Dir: %d", dir);
            _polygon->Crop(locationGlobal, dir, _ball->getPosition());
            RenderPolygon();
            SetCropper();
        } else {
            target->runAction(MoveTo::create(0.2, GetCropperOriginalPos()));
            if (_isRotatable)
            {
                _tapLabel->setVisible(true);
                _arrows->setVisible(true);
            }
        }

        if (_isRotatable) {
            if (locationGlobal.distance(GetCropperOriginalPos()) < 30)
                _cropper->setRotation(_cropper->getRotation() + 90.0f);
        }
    };

    _arrows->setPosition(GetCropperOriginalPos());
    addChild(_arrows);

    _cropper->setPosition(GetCropperOriginalPos());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(cropperTouchListener, _cropper);
    addChild(_cropper);

}

cocos2d::Rect GameClassic::GetGameAreaRect() {

    const auto center = GetGameAreaCenter();
    const auto SEG_LEN = GetGameAreaSquareWidth();
    ///////////////// Transparent Square Background (Game Area Shadow)

    return Rect(center + Vec2(-SEG_LEN / 2, -SEG_LEN / 2), Size(+SEG_LEN, SEG_LEN));
}

CropperImage GameClassic::GetInitialDirection() const
{
    if (GetRand1N(2) == 1)
        return CropperImage::LINE;

    return CropperImage::ANGLE;
}

void GameClassic::SetCropper() {
    _isRotatable = (GetRand01() > .5) ? true : false;

    if (_cropper == nullptr) {
        _cropper = Sprite::create();
    }

    if (_arrows == nullptr)
    {
        _arrows = Node::create();

        for (int i = 0; i < 2; ++i)
        {
            auto arrowSprite = Sprite::create("arrow.png");
            _arrows->addChild(arrowSprite);
            arrowSprite->setPosition(_arrows->getPosition() + Vec2(80 * i - 40, 40));
            arrowSprite->setRotation(90 * i + 45);
        }

        for (int i = 0; i < 2; ++i)
        {
            auto arrowSprite = Sprite::create("arrow.png");
            _arrows->addChild(arrowSprite);
            arrowSprite->setPosition(_arrows->getPosition() + Vec2(80 * i - 40, -40));
            arrowSprite->setRotation(-90 * i - 45);
        }

        _arrows->runAction(RepeatForever::create(RotateBy::create(1.0f, 20.0f)));

        _tapLabel = Label::createWithSystemFont("Tap to rotate",
                GameOptions::getInstance()->getMainFont(),
                28);
        addChild(_tapLabel);
        _tapLabel->setPosition(GetCropperOriginalPos() + Vec2(0.0f, 100.0f));
    }

    _arrows->setVisible(_isRotatable);
    _tapLabel->setVisible(_isRotatable);


    auto imageDireciton = GetInitialDirection();

    _cropper->setRotation(0.0f);
    switch (imageDireciton) {
        case CropperImage::LINE: {
            _cropper->setTexture("cropper_2.png");
            if (GetRand1N(2) == 1) {
                _cropper->setRotation(90.0f);
            }
            _cropper->setName("line");
            break;
        }
        case CropperImage::ANGLE: {
            _cropper->setTexture("cropper.png");
            int randDirection = GetRand1N(4);
            if (randDirection == 1)
                _cropper->setRotation(0.0f);
            else if (randDirection == 2) {
                _cropper->setRotation(90.0f);
            } else if (randDirection == 3) {
                _cropper->setRotation(180.0f);
            } else if (randDirection == 4) {
                _cropper->setRotation(270.0f);
            }
            _cropper->setName("angle");
            break;
        }
    }
}
