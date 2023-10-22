//
//  AnimationPlayer.cpp
//  Sprited
//
//  Created by Vulcain on 18/10/2023.
//

#include "include/AnimationPlayer.hpp"
#include "include/MainWindow.hpp"
#include "include/Utils.hpp"
#include "include/SpriteSelectorTab.hpp"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

AnimationPlayer::AnimationPlayer(QWidget* parent) : QLabel(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QLabel *renderAnim = new QLabel(this);
    setGeometry(0, 0, 400, 800);
    
    QHBoxLayout *hlayout1 = new QHBoxLayout(this);
    QHBoxLayout *hlayout2 = new QHBoxLayout(this);
    
    QPushButton *playButton = new QPushButton("Play", this);
    connect(playButton, &QPushButton::clicked, this, &AnimationPlayer::playButtonAction);
    
    QPushButton *stopButton = new QPushButton("Stop", this);
    connect(stopButton, &QPushButton::clicked, this, &AnimationPlayer::stopButtonAction);
    
    QLabel *anchorLabel = new QLabel("Anchor:", this);
    
    QPushButton *topleftAnchorButton = new QPushButton("Top Left", this);
    connect(topleftAnchorButton, &QPushButton::clicked, this, &AnimationPlayer::topLeftAnchorButtonAction);
    
    QPushButton *centerAnchorButton = new QPushButton("Center", this);
    connect(centerAnchorButton, &QPushButton::clicked, this, &AnimationPlayer::centerAnchorButtonAction);
    
    hlayout1->addWidget(anchorLabel);
    hlayout1->addWidget(topleftAnchorButton);
    hlayout1->addWidget(centerAnchorButton);
    
    hlayout2->addWidget(playButton);
    hlayout2->addWidget(stopButton);
    
    layout->addWidget(renderAnim);
    layout->addLayout(hlayout1);
    layout->addLayout(hlayout2);
    
    setLayout(layout);
}

QPoint AnimationPlayer::computeCenter() {
    auto table = win->getAnimationScene()->getTable();
    auto spr = win->getSpriteScene()->getSprites();
    int maxWidth = 0;
    int maxHeight = 0;
    
    for(int i = 0; i < table.size(); i++) {
        if(spr[table[i].first].cols > maxWidth) {
            maxWidth = spr[table[i].first].cols;
        }
        
        if(spr[table[i].first].rows > maxHeight) {
            maxHeight = spr[table[i].first].rows;
        }
    }
    
    return QPoint(maxWidth, maxHeight);
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
    auto rects = win->getSpriteScene()->getRect();
    auto table = win->getAnimationScene()->getTable();
    auto numbers = win->getSelectorTab()->getNumbers();
    auto max = computeCenter();
    
    if(table.size() > 0 && spr.size() > 0) {
        int index = table[currentFrame].first;
        std::cout << "index : " << index << std::endl;
        
        cv::Mat img_cv = numbers[index];
        
        auto img = toQImage(img_cv);
        double posX = (max.x() - img.width())/2.f;
        double posY = (max.y() - img.height())/2.f;
        
        painter.drawImage(QPoint(posX, posY), img);
    }
}

void AnimationPlayer::animate() {
    auto table = win->getAnimationScene()->getTable();
    
    // render when reach the num frame
    if(table[currentFrame].second == currentNumFrame) {
        update();
        currentNumFrame = 0;
        currentFrame++;
    } else {
        currentNumFrame++;
    }
    
    // if the current frame is too high
    if(currentFrame >= table.size()) {
        currentFrame = 0;
        currentNumFrame = 0;
        update();
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

void AnimationPlayer::topLeftAnchorButtonAction() {
    centerAnchor = false;
}

void AnimationPlayer::centerAnchorButtonAction() {
    centerAnchor = true; 
}
