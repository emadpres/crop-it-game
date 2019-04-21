#include "RestartScene.h"
#include "UserData.h"
#include "GameOptions.h"
#include "ui/CocosGUI.h"
#include "Game_ClassicMode/GameClassic.h"
#include "HelperFunctions.h"

bool RestartScene::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B::YELLOW))
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    auto bar = cocos2d::Sprite::create("bar.png");
    auto progressBar = cocos2d::ProgressTimer::create(bar);
    addChild(progressBar);
    progressBar->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height - progressBar->getContentSize().height));
    progressBar->setType(cocos2d::ProgressTimer::Type::BAR);
    progressBar->setMidpoint(cocos2d::Vec2(0.0f, 0.5f));
    progressBar->setPercentage(0);
    progressBar->setBarChangeRate(cocos2d::Vec2(1, 0));

    auto container = cocos2d::Sprite::create("container.png");
    addChild(container);
    container->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height - container->getContentSize().height));

    int highScore = UserData::getInstance()->getHighScore();
    int r = highScore / 20;
    int re = highScore % 20;
    int firstSeed = 0;

    firstSeed = (r == 0) ? 5 : r * 20;

    for (int i = 1; i <= 3; ++i)
    {
        int number = firstSeed + (i - 1) * 5;
        auto scoreIndicator = cocos2d::Label::createWithSystemFont(ToString(number),
                GameOptions::getInstance()->getMainFont(), 32);
        container->addChild(scoreIndicator);
        scoreIndicator->setPosition(container->getContentSize().width / 4 * i, -scoreIndicator->getContentSize().height);
        scoreIndicator->setTextColor(cocos2d::Color4B::BLACK);
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
    playButton->setPosition(visibleSize / 2 + cocos2d::Size(0.0f, 30.0f));
    playButton->addClickEventListener([&](Ref* sender){
        auto classic = GameClassic::create();
        auto scene = cocos2d::Scene::create();
        scene->addChild(classic);
        cocos2d::Director::getInstance()->replaceScene(scene);
    });
    playButton->setScale(4.0f);

    auto bestScoreText = cocos2d::Label::createWithSystemFont(ToString(highScore),
            GameOptions::getInstance()->getMainFont(), 40);
    container->addChild(bestScoreText);
    bestScoreText->setPosition(container->getContentSize() / 2 + cocos2d::Size(0.0f, -150.0f));
    bestScoreText->setTextColor(cocos2d::Color4B::BLACK);

    auto bestScoreLabel = cocos2d::Label::createWithSystemFont("Best Score",
            GameOptions::getInstance()->getMainFont(), 24);
    container->addChild(bestScoreLabel);
    bestScoreLabel->setPosition(container->getContentSize() / 2 + cocos2d::Size(0.0f, -100.0f));
    bestScoreLabel->setTextColor(cocos2d::Color4B::BLACK);

    return true;
}
