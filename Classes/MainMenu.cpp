#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameOptions.h"
#include "UserData.h"
#include "HelperFunctions.h"
#include "Credit.h"
#include "Game_ClassicMode/GameClassic.h"


USING_NS_CC;

cocos2d::Scene* MainMenu::createScene(bool isMainMenu)
{
    auto scene = cocos2d::Scene::create();
    auto layer = MainMenu::create(isMainMenu);

    if (layer != nullptr)
    {
        scene->addChild(layer);
        return scene;
    }

    return nullptr;
}

MainMenu* MainMenu::create(bool isMainMenu)
{
    auto mainScene = new (std::nothrow) MainMenu();
    if (mainScene && mainScene->init(isMainMenu))
    {
        mainScene->autorelease();
        return mainScene;
    }
    else
    {
        delete mainScene;
        mainScene = nullptr;
        return nullptr;
    }
}

bool MainMenu::init(bool isMainMenu)
{
    if ( !LayerColor::initWithColor(Color4B::MAGENTA))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto gameNmaeLabel = Label::createWithSystemFont("CropIt!", GameOptions::getInstance()->getMainFont(), 36);
    gameNmaeLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - gameNmaeLabel->getContentSize().height));
    addChild(gameNmaeLabel);

    auto bestScoreLabel = Label::createWithSystemFont("Best Score",
            GameOptions::getInstance()->getMainFont(), 30);
    bestScoreLabel->setPosition(visibleSize / 2 + Size(0.0f, visibleSize.height / 3));
    addChild(bestScoreLabel);

    auto scoreLabel = Label::createWithSystemFont(ToString(UserData::getInstance()->getHighScore()),
            GameOptions::getInstance()->getMainFont(), 34);
    scoreLabel->setPosition(visibleSize / 2 + Size(0.0f, visibleSize.height / 4));
    addChild(scoreLabel);

    auto playButton = ui::Button::create("PlayButton.png");
    playButton->setPosition(origin+visibleSize/2);
    playButton->addClickEventListener([&](Ref* sender){
        auto classic = GameClassic::create();
        auto scene = Scene::create();
        scene->addChild(classic);
        Director::getInstance()->replaceScene(scene);
    });
    addChild(playButton);
    playButton->setScale(4.0f);

    auto soundButton = ui::Button::create("Sound.png");
    auto soundButtonContentSize = soundButton->getContentSize();
    soundButton->setPosition(Vec2(origin.x + visibleSize.width - 2.0f * soundButtonContentSize.width,
            origin.y + 2.0f * soundButtonContentSize.height));
    addChild(soundButton);
    soundButton->setScale(4.0f);

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
    creditButton->setPosition(Vec2(origin.x + creditButtonContentSize.width,
            origin.y + creditButtonContentSize.height));
    addChild(creditButton);
    creditButton->setScale(2.0f);
    creditButton->addClickEventListener([&](Ref* sender){
        auto creditPage = Credit::create();
        auto scene = Scene::create();
        scene->addChild(creditPage);
        Director::getInstance()->replaceScene(scene);
    });

    return true;
}
