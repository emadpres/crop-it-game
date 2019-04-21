#include "GameClassic.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Ball.h"
#include "HelperFunctions.h"
#include "GameOptions.h"
#include "Geometry/Polygon.h"
#include "Geometry/TransformInfo.h"
#include "UserData.h"
#include "RestartScene.h"
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

    UserData::getInstance()->SetCurrentLevel(1);

    _levelLabel = Label::createWithSystemFont(ToString(UserData::getInstance()->GetCurrentLevel()),
                                              GameOptions::getInstance()->getMainFont(), 30);
    _levelLabel->setPosition(
            Vec2(origin.x + visibleSize.width / 2,
                 origin.y + visibleSize.height - _levelLabel->getContentSize().height));
    this->addChild(_levelLabel);


    auto bar = Sprite::create("bar.png");
    _progressBar = ProgressTimer::create(bar);
    addChild(_progressBar);
    _progressBar->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height - _levelLabel->getContentSize().height -
                                   _progressBar->getContentSize().height));
    _progressBar->setType(ProgressTimer::Type::BAR);
    _progressBar->setMidpoint(Vec2(0.0f, 0.5f));
    _progressBar->setPercentage(0);
    _progressBar->setBarChangeRate(Vec2(1, 0));

    auto container = Sprite::create("container.png");
    container->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height - _levelLabel->getContentSize().height -
                                container->getContentSize().height));

    addChild(container);


    _drawing_bg = DrawNode::create();
    _drawing_poly = DrawNode::create();
    addChild(_drawing_bg);
    addChild(_drawing_poly);
    InitialGameArea();
    InitialBall();
    SetCropper();
    InitCropper();
    RenderPolygon();
    return true;
}

void GameClassic::InitialGameArea() {
    ///////////////// Transparent Square Background (Game Area Shadow)
    const auto gameAreaRect = GetGameAreaRect();
    _drawing_bg->drawSolidRect(gameAreaRect.origin, gameAreaRect.origin + gameAreaRect.size, Color4F(50, 50, 50, 0.4));
    /////////////////
    int w = GetGameAreaSquareWidth();
    _polygon = new Polygon(GetGameAreaCenter() - Vec2(w / 2, w / 2), Size(w, w));
    _polygon->Initial_Square(GetGameAreaCenter() - Vec2(w / 2, w / 2), w);
    //_polygon->Initial_Custom1(GetGameAreaCenter()-Vec2(w/2,w/2), w);
    ////////////////
    _currentArea = _startingAreaAfterLevelUp = _polygon->CalcArea();
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
    _drawing_poly->clear();
    for (const pair<Vec2, Vec2> &seg: _polygon->GetSegments()) {
        _drawing_poly->drawSegment(seg.first, seg.second, 3, Color4F::BLACK);
        _drawing_poly->drawSolidCircle(seg.first, 5, 0, 5, Color4F::YELLOW);
        _drawing_poly->drawSolidCircle(seg.second, 5, 0, 5, Color4F::RED);
    }

    if (isCropperLinesValid) {
        _drawing_poly->drawSegment(_cropperLine1.first, _cropperLine1.second, 3, Color4F::ORANGE);
        _drawing_poly->drawSegment(_cropperLine2.first, _cropperLine2.second, 3, Color4F::ORANGE);
    }

}

void GameClassic::InitialBall() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _ball = Ball::create("ball.png", &_polygon->GetSegments(), &_cropperLine1, &_cropperLine2);
    _ball->setPosition(GetGameAreaCenter() + Vec2(100, 100));
    _ball->SetVelocity(Vec2(500, 110));
    addChild(_ball);
    IntialBallMovement();
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

            cropperDir = 0;
            auto directionName = _cropper->getName();
            auto rotationNo = (int) (_cropper->getRotation() / 90.0f);
            if (directionName == "line") {
                if (rotationNo % 2 == 0)
                    cropperDir = 5;
                else
                    cropperDir = 6;
            } else {
                if (rotationNo % 4 == 0)
                    cropperDir = 1;
                else if (rotationNo % 4 == 1)
                    cropperDir = 2;
                else if (rotationNo % 4 == 2)
                    cropperDir = 3;
                else if (rotationNo % 4 == 3)
                    cropperDir = 4;
            }

            Vec2 dir1, dir2;
            TranslateDir(cropperDir, dir1, dir2);
            _cropperLine1.first = _cropperLine1.second = _cropperLine2.first = _cropperLine2.second = locationGlobal;
            _cropperLineIntersectionPointWithPoly1 = _polygon->RayPos(locationGlobal, dir1);
            _cropperLineIntersectionPointWithPoly2 = _polygon->RayPos(locationGlobal, dir2);
            isCropperLinesValid = true;
            _ball->SetCropperLinesValidity(true);
            schedule(CC_CALLBACK_1(GameClassic::CropperLineAnimationRunner, this), 1 / 60.0, "cropper_line");
            RenderPolygon();
            SetCropper();
        } else {
            target->runAction(MoveTo::create(0.2, GetCropperOriginalPos()));
            if (_isRotatable) {
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


void GameClassic::CropperLineAnimationRunner(float dt) {
    bool firstDone = false, secondDone = false;
    Vec2 r1 = _cropperLineIntersectionPointWithPoly1 - _cropperLine1.second;
    Vec2 r2 = _cropperLineIntersectionPointWithPoly2 - _cropperLine2.second;

    float DRAW_SPEED = 6;

    if (r1.length() > 10) {
        r1.normalize();
        _cropperLine1.second += DRAW_SPEED * r1;
    } else {
        _cropperLine1.second = _cropperLineIntersectionPointWithPoly1;
        firstDone = true;
    }

    if (r2.length() > 10) {
        r2.normalize();
        _cropperLine2.second += DRAW_SPEED * r2;
    } else {
        _cropperLine2.second = _cropperLineIntersectionPointWithPoly2;
        secondDone = true;
    }

    if (firstDone && secondDone) {
        _ball->SetCropperLinesValidity(false);
        unschedule("cropper_line");
        Vec2 removedSideCenter;
        _polygon->Crop(_cropperLine1.first, cropperDir, _ball->getPosition(), removedSideCenter);

        _currentArea = _polygon->CalcArea();
        float removedSoFar = _startingAreaAfterLevelUp - _currentArea;
        float mustRemove = 0.7 * _startingAreaAfterLevelUp;
        float progress = min(100.0, 100.0 * removedSoFar / mustRemove);
        UpdateHud(progress);
        if (progress >= 100) {
            _polyTransformInfo = _polygon->EstimateScaleUp();
            unschedule("ball_tick");
            _targetPolyAfterAnimation = new Polygon(*_polygon);
            _targetPolyAfterAnimation->ScaleUp(_polyTransformInfo);

            _targetBallPos = _ball->getPosition();
            _polyTransformInfo->TranformVec2(_targetBallPos);
            schedule(CC_CALLBACK_1(GameClassic::ScaleUpAnimationRunner, this), 1 / 60.0, "scale_up");
        }
        isCropperLinesValid = false;
    }
    RenderPolygon();
}

void GameClassic::ScaleUpAnimationRunner(float dt) {
    const float ANIMATION_DUR = 2.0;
    _polyTransformInfo->_animationProgress01 += dt / ANIMATION_DUR;
    if (_polyTransformInfo->_animationProgress01 >= 0.25) {
        _polyTransformInfo->_animationProgress01 = 1;
        unschedule("scale_up");
        _currentArea = _startingAreaAfterLevelUp = _polygon->CalcArea();
        delete _targetPolyAfterAnimation;
        _targetPolyAfterAnimation = nullptr;
        IntialBallMovement();
    }

    auto it_target = _targetPolyAfterAnimation->GetSegments().begin();
    auto it_live = _polygon->GetSegments().begin();

    while (it_target != _targetPolyAfterAnimation->GetSegments().end()) {
        it_live->first =
                it_live->first + _polyTransformInfo->_animationProgress01 * (it_target->first - it_live->first);
        it_live->second =
                it_live->second + _polyTransformInfo->_animationProgress01 * (it_target->second - it_live->second);
        it_target++;
        it_live++;
    }

    _ball->setPosition(
            _ball->getPosition() + _polyTransformInfo->_animationProgress01 * (_targetBallPos - _ball->getPosition()));


    RenderPolygon();
}

cocos2d::Rect GameClassic::GetGameAreaRect() {

    const auto center = GetGameAreaCenter();
    const auto SEG_LEN = GetGameAreaSquareWidth();
    ///////////////// Transparent Square Background (Game Area Shadow)

    return Rect(center + Vec2(-SEG_LEN / 2, -SEG_LEN / 2), Size(+SEG_LEN, SEG_LEN));
}

CropperImage GameClassic::GetInitialDirection() const {
    if (GetRand1N(2) == 1)
        return CropperImage::LINE;

    return CropperImage::ANGLE;
}

void GameClassic::SetCropper() {
    _isRotatable = (GetRand01() > .5) ? true : false;

    if (_cropper == nullptr) {
        _cropper = Sprite::create();
    }

    if (_arrows == nullptr) {
        _arrows = Node::create();

        for (int i = 0; i < 2; ++i) {
            auto arrowSprite = Sprite::create("arrow.png");
            _arrows->addChild(arrowSprite);
            arrowSprite->setPosition(_arrows->getPosition() + Vec2(80 * i - 40, 40));
            arrowSprite->setRotation(90 * i + 45);
        }

        for (int i = 0; i < 2; ++i) {
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

void GameClassic::IntialBallMovement() {
    schedule([&](float dt) {
        bool touchCropprLine = _ball->MoveBall(dt);
        if(touchCropprLine) {
            unschedule("ball_tick");

            schedule([&](float){
                auto s = Scene::create();
                auto l = RestartScene::create();
                s->addChild(l);
                Director::getInstance()->replaceScene(s);
            },1,"go_reset_menu");

        }
    }, 1.0 / 60, "ball_tick");
}

void GameClassic::UpdateHud(int percentage) {
    // if percentage is 100 -> go to end then go back
    // if percentage is less than 100 -> go to it

    FiniteTimeAction *setPercentage = CallFunc::create([&, percentage]() {
        _progressBar->setPercentage(percentage);
    });

    auto progressToPercent = ProgressFromTo::create(1.0f, _progressBar->getPercentage(), percentage);

    if (percentage == 100) {
        UserData::getInstance()->SetCurrentLevel(UserData::getInstance()->GetCurrentLevel() + 1);
        int bestScore = UserData::getInstance()->getHighScore();
        if (UserData::getInstance()->GetCurrentLevel() > bestScore) {
            UserData::getInstance()->setHighScore(UserData::getInstance()->GetCurrentLevel());
        }
        auto progressToStart = ProgressFromTo::create(1.0f, _progressBar->getPercentage(), 0);
        _progressBar->runAction(Sequence::create(progressToPercent, setPercentage, progressToStart, nullptr));
        _levelLabel->setString(ToString(UserData::getInstance()->GetCurrentLevel()));
    } else {
        _progressBar->runAction(progressToPercent);
    }
}


GameClassic::~GameClassic() {
    delete _polygon;
    _polygon = nullptr;
}
