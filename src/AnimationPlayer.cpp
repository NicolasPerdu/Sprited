//
//  AnimationPlayer.cpp
//  Sprited
//
//  Created by Vulcain on 18/10/2023.
//

#include "include/AnimationPlayer.hpp"
#include "include/MainWindow.hpp"
#include "include/Utils.hpp"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

AnimationPlayer::AnimationPlayer(QWidget* parent) : QLabel(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QLabel *renderAnim = new QLabel(this);
    setGeometry(0, 0, 400, 800);
    
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    
    QPushButton *playButton = new QPushButton("Play", this);
    connect(playButton, &QPushButton::clicked, this, &AnimationPlayer::playButtonAction);
    
    QPushButton *stopButton = new QPushButton("Stop", this);
    connect(stopButton, &QPushButton::clicked, this, &AnimationPlayer::stopButtonAction);
    
    hlayout->addWidget(playButton);
    hlayout->addWidget(stopButton);
    
    layout->addWidget(renderAnim);
    layout->addLayout(hlayout);
    
    setLayout(layout);
}

void AnimationPlayer::paintEvent(QPaintEvent* event) {
    if(!playing)
        return;
    
    QPainter painter(this);
    
    if(win->getSpriteScene() == nullptr)
        return;
    
    if(win->getAnimationScene() == nullptr)
        return;
    
    auto spr = win->getSpriteScene()->getSprites();
    auto table = win->getAnimationScene()->getTable();
    
    if(table.size() > 0 && spr.size() > 0) {
        painter.drawImage(QPoint(0, 0), toQImage(spr[table[currentFrame].first]));
    }
}

void AnimationPlayer::animate() {
    currentFrame++;
    currentNumFrame++;
    
    auto table = win->getAnimationScene()->getTable();
    if(table[currentFrame].second == currentNumFrame) {
        update();
        currentNumFrame = 0;
    }
}

void AnimationPlayer::playButtonAction() {
    playing = true;
    
    currentFrame = 0;
    currentNumFrame = 0;
    
    if(timer == nullptr) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &AnimationPlayer::animate);
    }
    
    timer->start(1000.f/60.f);
    update();
}

void AnimationPlayer::stopButtonAction() {
    playing = false;
    timer->stop();
}
