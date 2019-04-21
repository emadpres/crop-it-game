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

    auto gameNameLabel = Label::createWithSystemFont("Credit", GameOptions::getInstance()->getMainFont(), 70);
    gameNameLabel->setPosition(nameBg->getContentSize() / 2 + Size(0.0f, gameNameLabel->getContentSize().height / 3));
    nameBg->addChild(gameNameLabel);
    gameNameLabel->enableBold();
    gameNameLabel->enableShadow();
    gameNameLabel->enableOutline(Color4B::BLACK);

    auto teamMemberButton1 = ui::Button::create("1.png");
    teamMemberButton1->setPosition(origin+visibleSize/2+Vec2(-visibleSize.width / 6,visibleSize.height/6));
    teamMemberButton1->addClickEventListener([&](Ref* sender){
        Application::getInstance()->openURL(GameOptions::getInstance()->getUrls()[1]);
    });
    addChild(teamMemberButton1);
    teamMemberButton1->setZoomScale(-0.1f);

    auto teamMemberLabel1 = Label::createWithSystemFont("Emad Aghajani", GameOptions::getInstance()->getMainFont(), 40);
    teamMemberLabel1->setPosition(1.4 * teamMemberButton1->getContentSize().width, teamMemberLabel1->getContentSize().height);
    teamMemberButton1->addChild(teamMemberLabel1);
    teamMemberLabel1->enableBold();
    teamMemberLabel1->setTextColor(Color4B::BLACK);

    auto teamMemberButton2 = ui::Button::create("2.png");
    teamMemberButton2->setPosition(origin+visibleSize/2 + Vec2(-visibleSize.width / 6, 0.0f));
    teamMemberButton2->addClickEventListener([&](Ref* sender){
        Application::getInstance()->openURL(GameOptions::getInstance()->getUrls()[3]);
    });
    addChild(teamMemberButton2);
    teamMemberButton2->setZoomScale(-0.1f);

    auto teamMemberLabel2 = Label::createWithSystemFont("Vahid Heidaripour", GameOptions::getInstance()->getMainFont(), 40);
    teamMemberLabel2->setPosition(1.4 * teamMemberButton2->getContentSize().width, teamMemberLabel2->getContentSize().height);
    teamMemberButton2->addChild(teamMemberLabel2);
    teamMemberLabel2->enableBold();
    teamMemberLabel2->setTextColor(Color4B::BLACK);


    auto teamMemberButton3 = ui::Button::create("3.png");
    teamMemberButton3->setPosition(origin+visibleSize/2+Vec2(-visibleSize.width / 6,-visibleSize.height/6));
    teamMemberButton3->addClickEventListener([&](Ref* sender){
        Application::getInstance()->openURL(GameOptions::getInstance()->getUrls()[2]);
    });
    addChild(teamMemberButton3);
    teamMemberButton3->setZoomScale(-0.1f);

    auto teamMemberLabel3 = Label::createWithSystemFont("Mojtaba Eslahi", GameOptions::getInstance()->getMainFont(), 40);
    teamMemberLabel3->setPosition(1.4 * teamMemberButton3->getContentSize().width, teamMemberLabel3->getContentSize().height);
    teamMemberButton3->addChild(teamMemberLabel3);
    teamMemberLabel3->enableBold();
    teamMemberLabel3->setTextColor(Color4B::BLACK);


    auto gitButton = ui::Button::create("Octocat.png");
    gitButton->setPosition(origin + gitButton->getContentSize() / 2 + Size(0.0f, 20.0f));
    gitButton->addClickEventListener([&](Ref* sender){
        Application::getInstance()->openURL(GameOptions::getInstance()->getUrls()[0]);
    });
    addChild(gitButton);
    gitButton->setScale(0.9);
    gitButton->setZoomScale(-0.1f);


    auto backButton = ui::Button::create("back.png");
    backButton->setPosition(origin + Vec2(visibleSize.width - backButton->getContentSize().width / 2, backButton->getContentSize().height / 2));
    backButton->addClickEventListener([&](Ref* sender){
        auto scene = Scene::create();
        scene->addChild(MainMenu::create());
        Director::getInstance()->replaceScene(scene);

//        auto main = MainMenu::create();
//        auto scene = Scene::create();
//        scene->addChild(main);
//        Director::getInstance()->replaceScene(scene);
    });
    addChild(backButton);
    backButton->setScale(0.6f);


    return true;
}