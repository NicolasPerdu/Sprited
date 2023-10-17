//
//  AnimationScene.cpp
//  Sprited
//
//  Created by Vulcain on 17/10/2023.
//

#include "include/AnimationScene.hpp"
#include <QTableWidget>
#include <QHBoxLayout>

AnimationScene::AnimationScene(QWidget* parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    
    table = new QTableWidget(100, 2, this);
    setGeometry(0, 0, 400, 800);
    
    layout->addWidget(table);
    
    table->setHorizontalHeaderLabels(QStringList {"Sprite Id", "Frames"});
    
    setLayout(layout);
}


