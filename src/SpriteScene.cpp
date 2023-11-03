//
//  SpriteScene.cpp
//  Sprited
//
//  Created by Vulcain on 13/10/2023.
//

#include "../include/SpriteScene.hpp"
#include <QMouseEvent>
#include <opencv2/opencv.hpp>
#include "../include/MainWindow.hpp"

void SpriteScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    painter.drawImage(QPoint(0, 0), pixmap().toImage());

    painter.setPen(QPen(Qt::black, 1));
    //painter.setBrush(QBrush(Qt::blue));
    
    // normal rectangle
    for(int i = 0; i < rect.size(); i++) {
        auto r = rect[i].first;
        painter.drawRect(r.x, r.y, r.width, r.height);
    }
    
    painter.setPen(QPen(Qt::red, 1));
    
    for(int i = 0; i < rectSelected.size(); i++) {
        painter.drawRect(rectSelected[i].second.x, rectSelected[i].second.y, rectSelected[i].second.width, rectSelected[i].second.height);
    }
}

bool SpriteScene::isPointInsideAABB(const cv::Point& point, const cv::Rect& box) {
    return (point.x >= box.x && point.x <= (box.x + box.width) && point.y >= box.y && point.y <= (box.y + box.height));
}

SpriteScene::SpriteScene(QWidget* parent) : QLabel(parent) {
}

cv::Rect SpriteScene::mergeRect() {
    std::pair<int, int> minX = {0,9999};
    std::pair<int, int> minY = {0,9999};
    std::pair<int, int> maxX = {0,0};
    std::pair<int, int> maxY = {0,0};
    
    for(int i = 0; i < rectSelected.size(); i++) {
        if(rectSelected[i].second.x < minX.second)
            minX = {rectSelected[i].first, rectSelected[i].second.x};
        if(rectSelected[i].second.y < minY.second)
            minY = {rectSelected[i].first, rectSelected[i].second.y};
        if((rectSelected[i].second.x + rectSelected[i].second.width) > maxX.second)
            maxX = {rectSelected[i].first, rectSelected[i].second.x + rectSelected[i].second.width};
        if((rectSelected[i].second.y + rectSelected[i].second.height) > maxY.second)
            maxY = {rectSelected[i].first, rectSelected[i].second.y + rectSelected[i].second.height};
    }
    
    cv::Rect res{minX.second, minY.second, maxX.second - minX.second, maxY.second - minY.second};
    
    rect.push_back(std::make_pair(res, false));
    
    cv::Mat sprite = spritesheet(res).clone();
    
    std::set st{minX.first, minY.first, maxX.first, maxY.first};
    
    std::set<int>::iterator it = st.begin();
    sprites.push_back(std::make_pair(sprites[*it].first, sprite));
    
    for(auto& el : st) {
        rect[el] = rect.back();
        rect.pop_back();
        
        sprites[el] = sprites.back();
        sprites.pop_back();
    }
    
    rectSelected.clear();
    this->update();
    
    return res;
}

/*bool SpriteScene::aabb(cv::Rect a, cv::Rect b) {
    if ((a.x + a.width) < b.x || a.x > (b.x + b.width) || (a.y + a.height) < b.y || a.y > (b.y + b.height)) {
        return false;
    }

    return true;
}*/

void SpriteScene::mousePressEvent(QMouseEvent *ev) {
    if(!win->isSelectEnabled())
        return;
    
    int x = ev->pos().x();
    int y = ev->pos().y();
    
    cv::Point mouse{x, y};
    
    for(int i = 0; i < rect.size(); i++) {
        if(isPointInsideAABB(mouse, rect[i].first)) {
            //std::cout << "selected rect num " << i << " : " << rect[i].x << ", " << rect[i].y << ", " << rect[i].width << "," << rect[i].height << std::endl;
            rectSelected.push_back({i, rect[i].first});
        }
    }
    
    this->update();
}
