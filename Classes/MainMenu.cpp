#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include "GameClassic.h"
#include "GameOptions.h"

USING_NS_CC;

bool MainMenu::init()
{
    if ( !LayerColor::initWithColor(Color4B::RED))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto gameNmae = Label::createWithSystemFont("CropIt!", GameOptions::getInstance().getMainFont(), 36);
    gameNmae->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - gameNmae->getContentSize().height));
    addChild(gameNmae);

    auto playButton = ui::Button::create("PlayButton.png");
    playButton->setPosition(origin+visibleSize/2);
    playButton->addClickEventListener([&](Ref* sender){
        auto classic = GameClassic::create();
        auto scene = Scene::create();
        scene->addChild(classic);
        Director::getInstance()->replaceScene(scene);
    });
    addChild(playButton);
    playButton->setScale(2.0f);

    return true;
}
