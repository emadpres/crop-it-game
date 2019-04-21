#include "RestartScene.h"
#include "UserData.h"
#include "GameOptions.h"
#include "ui/CocosGUI.h"
#include "Game_ClassicMode/GameClassic.h"
#include "HelperFunctions.h"

bool RestartScene::init()
{
    if (!LayerColor::init())
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto bg = cocos2d::Sprite::create("bg.png");
    addChild(bg);
    bg->setPosition(origin + visibleSize / 2);

    auto container = cocos2d::Sprite::create("container2.png");
    addChild(container);
    container->setPosition(origin + cocos2d::Vec2(visibleSize.width / 2, visibleSize.height - container->getContentSize().height));

    auto bar = cocos2d::Sprite::create("bar.png");
    auto progressBar = cocos2d::ProgressTimer::create(bar);
    container->addChild(progressBar);
    progressBar->setPosition(origin + container->getContentSize() / 2 + cocos2d::Size(-20.0f, 10.0f));
    progressBar->setType(cocos2d::ProgressTimer::Type::BAR);
    progressBar->setMidpoint(cocos2d::Vec2(0.0f, 0.5f));
    progressBar->setPercentage(0);
    progressBar->setBarChangeRate(cocos2d::Vec2(1, 0));

    auto star = cocos2d::Sprite::create("star.png");
    container->addChild(star);
    star->setPosition(origin + cocos2d::Vec2(0.0f, container->getContentSize().height / 2+10));
    star->setScale(0.8f);

    int highScore = UserData::getInstance()->getHighScore();
    int r = highScore / 20;
    int re = highScore % 20;
    int firstSeed = 0;

    firstSeed = (r == 0) ? 5 : r * 20;

    for (int i = 1; i <= 3; ++i)
    {
        int number = firstSeed + (i - 1) * 5;
        auto scoreIndicator = cocos2d::Label::createWithSystemFont(ToString(number),
                GameOptions::getInstance()->getMainFont(), 38);
        container->addChild(scoreIndicator);
        scoreIndicator->setPosition(container->getContentSize().width / 4 * i + 20.0f, -scoreIndicator->getContentSize().height);
        scoreIndicator->enableBold();
        scoreIndicator->enableShadow();
    }

    if (highScore != 0) {

        if (re != 0) {
            if (r == 0) {
                progressBar->setPercentage(re * 5);
            } else {
                progressBar->setPercentage(25 + re * 5);
            }
        } else {
            progressBar->setPercentage(25);
        }
    }

    auto playButton = cocos2d::ui::Button::create("PlayButton.png");
    addChild(playButton);
    playButton->setPosition(origin + visibleSize / 2 + cocos2d::Size(0.0f, 30.0f));
    playButton->addClickEventListener([&](Ref* sender){
        auto classic = GameClassic::create();
        auto scene = cocos2d::Scene::create();
        scene->addChild(classic);
        cocos2d::Director::getInstance()->replaceScene(scene);
    });
    playButton->setZoomScale(-0.1f);


    auto bestScoreLabel = cocos2d::Label::createWithSystemFont("Best Score",
                                                      GameOptions::getInstance()->getMainFont(), 40);
    bestScoreLabel->setPosition(origin + visibleSize / 2 + cocos2d::Size(0.0f, visibleSize.height / 4));
    addChild(bestScoreLabel);
    bestScoreLabel->enableOutline(cocos2d::Color4B::BLACK);
    bestScoreLabel->enableShadow();

    auto scoreLabel = cocos2d::Label::createWithSystemFont(ToString(UserData::getInstance()->getHighScore()),
                                                  GameOptions::getInstance()->getMainFont(), 65);
    scoreLabel->setPosition(origin + visibleSize / 2 + cocos2d::Size(0.0f, visibleSize.height / 5));
    addChild(scoreLabel);
    scoreLabel->enableOutline(cocos2d::Color4B::BLACK);
    scoreLabel->enableShadow();

    return true;
}
