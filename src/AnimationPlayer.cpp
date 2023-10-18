//
//  AnimationPlayer.cpp
//  Sprited
//
//  Created by Vulcain on 18/10/2023.
//

#include "include/AnimationPlayer.hpp"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

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
    QPainter painter(this);
    
    //painter.drawImage(QPoint(0, 0), pixmap().toImage());
}

void AnimationPlayer::playButtonAction() {
    playing = true;
}

void AnimationPlayer::stopButtonAction() {
    playing = false;
}
