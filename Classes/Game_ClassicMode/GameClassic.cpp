#include "GameClassic.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Ball.h"
#include "HelperFunctions.h"


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

    drawingNode = DrawNode::create();
    addChild(drawingNode);
    InitialGameArea();
    InitialBall();
    InitCropper();


    RenderSegment();


    auto soundButton = ui::Button::create("Sound.png");
    auto soundButtonContentSize = soundButton->getContentSize();
    soundButton->setPosition(Vec2(origin.x + visibleSize.width - 2.0f * soundButtonContentSize.width,
                                  origin.y + 2.0f * soundButtonContentSize.height));
    addChild(soundButton);
    soundButton->setScale(4.0f);
    soundButton->addClickEventListener([&](Ref*){
//        BreakSegment(0,0.3);
//        RenderSegment();
    });



    return true;
}

void GameClassic::InitialGameArea() {
    ///////////////// Transparent Square Background (Game Area Shadow)
    const auto gameAreaRect = GetGameAreaRect();
    drawingNode->drawSolidRect(gameAreaRect.origin, gameAreaRect.origin+gameAreaRect.size, Color4F(50, 50, 50, 0.4));
    /////////////////
    CreateShape_InitialSquare();
    //CreateShape_Custom1();
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

void GameClassic::RenderSegment() {
    drawingNode->clear();
    for (pair<Vec2, Vec2> &seg: _segments) {
        drawingNode->drawSegment(seg.first, seg.second, 3, Color4F::BLACK);
        drawingNode->drawSolidCircle(seg.first,5,0,5,Color4F::YELLOW);
        drawingNode->drawSolidCircle(seg.second,5,0,5,Color4F::RED);
    }
}

void GameClassic::CreateShape_InitialSquare() {

    const auto center = GetGameAreaCenter();
    const auto SEG_LEN = GetGameAreaSquareWidth();

    _segments.clear();
    _segments.push_back(make_pair(center + Vec2(-SEG_LEN/2, +SEG_LEN/2), center + Vec2(+SEG_LEN/2, +SEG_LEN/2)));
    _segments.push_back(make_pair(center + Vec2(+SEG_LEN/2, +SEG_LEN/2), center + Vec2(+SEG_LEN/2, -SEG_LEN/2)));
    _segments.push_back(make_pair(center + Vec2(+SEG_LEN/2, -SEG_LEN/2), center + Vec2(-SEG_LEN/2, -SEG_LEN/2)));
    _segments.push_back(make_pair(center + Vec2(-SEG_LEN/2, -SEG_LEN/2), center + Vec2(-SEG_LEN/2, +SEG_LEN/2)));
}

void GameClassic::CreateShape_Custom1() {
    const auto center = GetGameAreaCenter();
    const auto SEG_LEN = GetGameAreaSquareWidth();

    _segments.clear();
    _segments.push_back(make_pair(center + Vec2(-SEG_LEN, +SEG_LEN), center + Vec2(+SEG_LEN, +SEG_LEN)));
    _segments.push_back(make_pair(center + Vec2(+SEG_LEN, +SEG_LEN), center + Vec2(+SEG_LEN, -SEG_LEN)));
    _segments.push_back(make_pair(center + Vec2(+SEG_LEN, -SEG_LEN), center + Vec2(0, -SEG_LEN)));
    _segments.push_back(make_pair(center + Vec2(0, -SEG_LEN), center + Vec2(0, 0)));
    _segments.push_back(make_pair(center + Vec2(0, 0), center + Vec2(-SEG_LEN, 0)));
    _segments.push_back(make_pair(center + Vec2(-SEG_LEN, 0), center + Vec2(-SEG_LEN, +SEG_LEN)));
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

void GameClassic::BreakSegment(list<pair<Vec2, Vec2>>::iterator it, float breakRatio)
{
//    list<pair<Vec2, Vec2>>::iterator it = _segments.begin();
//    int cur = 0;
//    while(cur!=segIndex && it!= _segments.end()) {
//        it++;
//        cur++;
//    }
    Vec2 start = (*it).first;
    Vec2 end = (*it).second;
    (*it).second.x = start.x+(end.x-start.x)*breakRatio;
    (*it).second.y = start.y+(end.y-start.y)*breakRatio;
    auto updatedEnd = (*it).second;
    it++;
    _segments.insert(it,make_pair(updatedEnd, end));
    CCLOG("New number of segments: %d", _segments.size());
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
            Crop(locationGlobal,1);
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


void GameClassic::Crop(cocos2d::Vec2 pos, int dir)
{
    int first = Crop(pos, pos+Vec2(1000,0));
    int second = Crop(pos, pos+Vec2(0,-1000));
    if(second<first) {
        int t = first;
        first = second;
        second = t+1;
    }

    std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> _newSegments;
    bool keepInnerPart = true;
    int i =0;
    for (const auto &seg: _segments)
    {
        if(keepInnerPart) {
            if(i>first && i<=second)
                _newSegments.push_back(seg);

        }
        i++;
    }

    _segments.clear();
    for (const auto &seg: _newSegments)
        _segments.push_back(seg);

    RenderSegment();

}

int GameClassic::Crop(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
    float s,t;
    int breakIndex = 0;
    auto it = _segments.begin();
    while (it!=_segments.end()) {
        if(Vec2::isLineIntersect(start, end, (*it).first, (*it).second, &s, &t))
        {
            if(s>=0 && t>=0 && s<=1 && t<=1) {
                BreakSegment(it, t);
                break;
            }

        }
        it++;
        breakIndex++;
    }
    return breakIndex;
}