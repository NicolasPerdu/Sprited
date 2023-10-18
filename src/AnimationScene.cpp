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

std::vector<std::pair<int, int>> AnimationScene::getTable() {
    std::vector<std::pair<int, int>> res;
    res.reserve(table->size().height());
    
    for(int i = 0; i < table->size().height(); i++) {
        std::string str1 = table->item(i, 0)->text().toStdString();
        int val1 = 0;
        
        try {
            val1 = std::stoi(str1.c_str());
        } catch(std::exception const & e)
        {
             //cout<<"error : " << e.what() <<endl;
        }
        
        std::string str2 = table->item(i, 1)->text().toStdString();
        int val2 = 0;
        
        try {
            val2 = std::stoi(str2.c_str());
        } catch(std::exception const & e)
        {
             //cout<<"error : " << e.what() <<endl;
        }
        
        res.push_back(std::pair<int, int>{val1, val2});
    }
    
    return res;
}


