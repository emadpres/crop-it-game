//
// Created by Mojtaba Eslahi Kelorazi on 2019-04-19.
//

#include "Credit.h"
#include "ui/CocosGUI.h"

#include "GameOptions.h"
#include "MainMenu.h"

USING_NS_CC;

bool Credit::init()
{
    if ( !LayerColor::initWithColor(Color4B::ORANGE))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    auto label = Label::createWithTTF("Game: Credit", GameOptions::getInstance()->getMainFont(), 24);
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    auto teamMemberButton1 = ui::Button::create("CreditTeam.png");
    teamMemberButton1->setPosition(origin+visibleSize/2+Vec2(0,visibleSize.height/3));
//    teamMemberButton1->setTitleText("Emad Aghajani");
    teamMemberButton1->addClickEventListener([&](Ref* sender){
//        auto classic = GameClassic::create();
//        auto scene = Scene::create();
//        scene->addChild(classic);
//        Director::getInstance()->replaceScene(scene);
    });
    addChild(teamMemberButton1);

    auto teamMemberLabel1 = Label::createWithSystemFont("Emad Aghajani", GameOptions::getInstance()->getMainFont(), 24);
    teamMemberLabel1->setPosition(origin+visibleSize/2+Vec2(0,visibleSize.height/3-80));
    addChild(teamMemberLabel1);

    auto teamMemberButton2 = ui::Button::create("CreditTeam.png");
    teamMemberButton2->setPosition(origin+visibleSize/2+Vec2(0,visibleSize.height/3-250));
//    teamMemberButton2->setTitleText("Vahid Heidaripour");
    teamMemberButton2->addClickEventListener([&](Ref* sender){
//        auto classic = GameClassic::create();
//        auto scene = Scene::create();
//        scene->addChild(classic);
//        Director::getInstance()->replaceScene(scene);
    });
    addChild(teamMemberButton2);

    auto teamMemberLabel2 = Label::createWithSystemFont("Vahid Heidaripour", GameOptions::getInstance()->getMainFont(), 24);
    teamMemberLabel2->setPosition(origin+visibleSize/2+Vec2(0,visibleSize.height/3-250-80));
    addChild(teamMemberLabel2);

    auto teamMemberButton3 = ui::Button::create("CreditTeam.png");
    teamMemberButton3->setPosition(origin+visibleSize/2+Vec2(0,visibleSize.height/3-500));
//    teamMemberButton3->setTitleText("Mojtaba Eslahi");
    teamMemberButton3->addClickEventListener([&](Ref* sender){
//        auto classic = GameClassic::create();
//        auto scene = Scene::create();
//        scene->addChild(classic);
//        Director::getInstance()->replaceScene(scene);
    });
    addChild(teamMemberButton3);

    auto teamMemberLabel3 = Label::createWithSystemFont("Mojtaba Eslahi", GameOptions::getInstance()->getMainFont(), 24);
    teamMemberLabel3->setPosition(origin+visibleSize/2+Vec2(0,visibleSize.height/3-500-80));
    addChild(teamMemberLabel3);

    auto backButton = ui::Button::create("BackButton.png");
    backButton->setPosition(Vec2(origin.x+visibleSize.width-100, origin.y+80));
    backButton->addClickEventListener([&](Ref* sender){
        auto main = MainMenu::create();
        auto scene = Scene::create();
        scene->addChild(main);
        Director::getInstance()->replaceScene(scene);
    });
    addChild(backButton);


    return true;
}