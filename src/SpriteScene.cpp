//
//  SpriteScene.cpp
//  Sprited
//
//  Created by Vulcain on 13/10/2023.
//

#include "../include/SpriteScene.hpp"
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

void SpriteScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    painter.drawImage(QPoint(0, 0), pixmap().toImage());

    painter.setPen(QPen(Qt::black, 1));
    //painter.setBrush(QBrush(Qt::blue));
    
    for(int i = 0; i < rect.size(); i++) {
        painter.drawRect(rect[i].x, rect[i].y, rect[i].width, rect[i].height);
    }
}

bool SpriteScene::isPointInsideAABB(const cv::Point& point, const cv::Rect& box) {
    return (point.x >= box.x && point.x <= (box.x + box.width) && point.y >= box.y && point.y <= (box.y + box.height));
}

bool SpriteScene::aabb(cv::Rect a, cv::Rect b) {
    if ((a.x + a.width) < b.x || a.x > (b.x + b.width) || (a.y + a.height) < b.y || a.y > (b.y + b.height)) {
        return false;
    }

    return true;
}

void SpriteScene::mousePressEvent(QMouseEvent *ev) {
    int x = ev->pos().x();
    int y = ev->pos().y();
    
    cv::Point mouse{x, y};
    
    for(int i = 0; i < rect.size(); i++) {
        if(isPointInsideAABB(mouse, rect[i])) {
            std::cout << "selected rect num " << i << " : " << rect[i].x << ", " << rect[i].y << ", " << rect[i].width << "," << rect[i].height << std::endl;
        }
    }
}
