#include "GameClassic.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Ball.h"
#include "HelperFunctions.h"
#include "Geometry/Polygon.h"


USING_NS_CC;
using namespace std;

bool GameClassic::init() {
    if (!LayerColor::initWithColor(Color4B::ORANGE)) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto label = Label::createWithTTF("Game: Classic", "fonts/Marker Felt.ttf", 24);
    label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label);

    _drawingNode = DrawNode::create();
    addChild(_drawingNode);
    InitialGameArea();
    InitialBall();
    InitCropper();


    RenderPolygon();


    auto soundButton = ui::Button::create("Sound.png");
    auto soundButtonContentSize = soundButton->getContentSize();
    soundButton->setPosition(Vec2(origin.x + visibleSize.width - 2.0f * soundButtonContentSize.width,
                                  origin.y + 2.0f * soundButtonContentSize.height));
    addChild(soundButton);
    soundButton->setScale(4.0f);
    soundButton->addClickEventListener([&](Ref*){
//        BreakSegment(0,0.3);
//        RenderPolygon();
    });



    return true;
}

void GameClassic::InitialGameArea() {
    ///////////////// Transparent Square Background (Game Area Shadow)
    const auto gameAreaRect = GetGameAreaRect();
    _drawingNode->drawSolidRect(gameAreaRect.origin, gameAreaRect.origin+gameAreaRect.size, Color4F(50, 50, 50, 0.4));
    /////////////////
    _polygon = new Polygon();
    int w = GetGameAreaSquareWidth();
    _polygon->Initial_Square(GetGameAreaCenter()-Vec2(w/2,w/2), w);
    //_polygon->Initial_Custom1(GetGameAreaCenter()-Vec2(w/2,w/2), w);
}

Vec2 GameClassic::GetGameAreaCenter() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = origin + visibleSize / 2 + Vec2(0, visibleSize.height / 8.0);
    return center;
}

int GameClassic::GetGameAreaSquareWidth() {
    return 270*2;
}

void GameClassic::RenderPolygon() {
    _drawingNode->clear();
    for (const pair<Vec2, Vec2> &seg: _polygon->GetSegments()) {
        _drawingNode->drawSegment(seg.first, seg.second, 3, Color4F::BLACK);
        _drawingNode->drawSolidCircle(seg.first,5,0,5,Color4F::YELLOW);
        _drawingNode->drawSolidCircle(seg.second,5,0,5,Color4F::RED);
    }
}


void GameClassic::InitialBall() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _ball = Ball::create("ball.png");
    _ball->setPosition(GetGameAreaCenter());
    _ball->SetVelocity(Vec2(40,30));
    addChild(_ball);

    schedule([&](float dt)
    {
        Vec2 newPos = _ball->EstimateMove(dt);
        _ball->setPosition(newPos);
        }, 1.0/30, "ball_tick");

    schedule([&](float dt)
             {
                 _ball->SetVelocity(Vec2((GetRand01()-0.5)*120, (GetRand01()-0.5)*120));
             }, 1, "ball_tick_velo");

}

Vec2 GameClassic::GetCropperOriginalPos()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return origin+Vec2(visibleSize.width/2, 100);
}

void GameClassic::InitCropper() {
    EventListenerTouchOneByOne* cropperTouchListener= EventListenerTouchOneByOne::create();
    cropperTouchListener->setSwallowTouches(true);
    cropperTouchListener->onTouchBegan = [=](Touch* touch, Event* event){
        auto target = static_cast<Node*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        //CCLOG("Location in Node: %f\t%f", locationInNode.x, locationInNode.y);
        Size s = target->getContentSize();
        Rect rect = Rect( 0,0 , s.width, s.height);
        if (rect.containsPoint(locationInNode) && target->isVisible())
        {
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

    cropperTouchListener->onTouchMoved = [=](Touch* touch, Event* event){
        auto target = static_cast<Node*>(event->getCurrentTarget());
        target->setPosition(target->getPosition() + touch->getDelta());
    };

    cropperTouchListener->onTouchEnded = cropperTouchListener->onTouchCancelled = [&](Touch* touch, Event* event){
        auto target = static_cast<Node*>(event->getCurrentTarget());
        Vec2 locationGlobal = touch->getLocation(); // remember: this position is based on (0,0), not Origin
        bool inGameArea = GetGameAreaRect().containsPoint(locationGlobal);
        target->stopAllActions();
        target->setScale(1);
        target->setOpacity(255);
        if(inGameArea)
        {
            target->setPosition(GetCropperOriginalPos());
            _polygon->Crop(locationGlobal,1);
            RenderPolygon();
        }
        else
        {
            target->runAction(MoveTo::create(0.2, GetCropperOriginalPos()));
        }
    };


    auto cropper = Sprite::create("cropper.png");
    cropper->setPosition(GetCropperOriginalPos());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(cropperTouchListener, cropper);
    addChild(cropper);

}

cocos2d::Rect GameClassic::GetGameAreaRect() {

    const auto center = GetGameAreaCenter();
    const auto SEG_LEN = GetGameAreaSquareWidth();
    ///////////////// Transparent Square Background (Game Area Shadow)

    return Rect(center + Vec2(-SEG_LEN/2, -SEG_LEN/2),Size(+SEG_LEN,SEG_LEN));
}