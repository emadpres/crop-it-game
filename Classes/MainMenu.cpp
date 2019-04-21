#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameOptions.h"
#include "UserData.h"
#include "HelperFunctions.h"
#include "Credit.h"
#include "Game_ClassicMode/GameClassic.h"


USING_NS_CC;

bool MainMenu::init()
{
    if ( !LayerColor::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("bg.png");
    addChild(bg);
    bg->setPosition(origin + visibleSize / 2);

    auto nameBg = Sprite::create("nameBg.png");
    addChild(nameBg);
    nameBg->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height - nameBg->getContentSize().height));

    auto gameNameLabel = Label::createWithSystemFont("Crop It", GameOptions::getInstance()->getMainFont(), 70);
    gameNameLabel->setPosition(nameBg->getContentSize() / 2 + Size(0.0f, gameNameLabel->getContentSize().height / 3));
    nameBg->addChild(gameNameLabel);
    gameNameLabel->enableBold();
    gameNameLabel->enableShadow();
    gameNameLabel->enableOutline(Color4B::BLACK);

    auto bestScoreLabel = Label::createWithSystemFont("Best Score",
            GameOptions::getInstance()->getMainFont(), 40);
    bestScoreLabel->setPosition(origin + visibleSize / 2 + Size(0.0f, visibleSize.height / 4));
    addChild(bestScoreLabel);
    bestScoreLabel->enableOutline(Color4B::BLACK);

    auto scoreLabel = Label::createWithSystemFont(ToString(UserData::getInstance()->getHighScore()),
            GameOptions::getInstance()->getMainFont(), 65);
    scoreLabel->setPosition(origin + visibleSize / 2 + Size(0.0f, visibleSize.height / 5));
    addChild(scoreLabel);
    scoreLabel->enableOutline(Color4B::BLACK);
    scoreLabel->enableShadow();

    auto playButton = ui::Button::create("PlayButton.png");
    playButton->setPosition(origin+visibleSize/2);
    playButton->addClickEventListener([&](Ref* sender){
        auto classic = GameClassic::create();
        auto scene = Scene::create();
        scene->addChild(classic);
        Director::getInstance()->replaceScene(scene);
    });
    addChild(playButton);
    playButton->setZoomScale(-0.1f);

    auto soundButton = ui::Button::create("Sound.png");
    auto soundButtonContentSize = soundButton->getContentSize();
    soundButton->setPosition(Vec2(origin.x + visibleSize.width - 2.0f * soundButtonContentSize.width,
            origin.y + 2.0f * soundButtonContentSize.height));
    addChild(soundButton);
    soundButton->setScale(4.0f);
    soundButton->setVisible(false);

    if (!GameOptions::getInstance()->getSoundStatus())
        soundButton->setColor(Color3B::RED);

    soundButton->addClickEventListener([&](Ref* sender){
        auto button = static_cast<ui::Button*>(sender);
        if (GameOptions::getInstance()->getSoundStatus())
        {
            button->setColor(Color3B::RED);
            CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
            GameOptions::getInstance()->setSoundStatus(false);
        }
        else
        {
            button->setColor(Color3B::WHITE);
            CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
            GameOptions::getInstance()->setSoundStatus(true);
        }
    });

    auto creditButton = ui::Button::create("Credit.png");
    auto creditButtonContentSize = creditButton->getContentSize();
    creditButton->setPosition(origin + Vec2(visibleSize.width / 2, creditButton->getContentSize().height));
    addChild(creditButton);
    creditButton->addClickEventListener([&](Ref* sender){
        auto creditPage = Credit::create();
        auto scene = Scene::create();
        scene->addChild(creditPage);
        Director::getInstance()->replaceScene(scene);
    });
    creditButton->setZoomScale(-0.1f);

    auto creditLabel = Label::createWithSystemFont("credit",
            GameOptions::getInstance()->getMainFont(),
            45);
    creditButton->addChild(creditLabel);
    creditLabel->setPosition(creditButton->getContentSize() / 2 + Size(0.0f, 10.0f));
    creditLabel->setTextColor(Color4B::BLACK);

    return true;
}
