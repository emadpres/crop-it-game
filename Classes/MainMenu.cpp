#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include "Game_ClassicMode/GameClassic.h"

USING_NS_CC;

bool MainMenu::init()
{
    if ( !LayerColor::initWithColor(Color4B::RED))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    auto label = Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);


    auto button = ui::Button::create("CloseNormal.png", "play.png");
    button->setPosition(origin+visibleSize/2);
    button->setTitleText("Play !");
    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED: {
                auto classic = GameClassic::create();
                auto scene = Scene::create();
                scene->addChild(classic);
                Director::getInstance()->replaceScene(scene);
                break;
            }
            default:
                break;
        }
    });
    this->addChild(button,2);

    return true;
}
