//
//  SpriteScene.hpp
//  Sprited
//
//  Created by Vulcain on 13/10/2023.
//

#ifndef SpriteScene_hpp
#define SpriteScene_hpp

#include <stdio.h>
#include <QLabel>
#include <QPainter>
#include <vector>
#include <opencv2/opencv.hpp>

class SpriteScene : public QLabel {
public:
    SpriteScene(QWidget* parent = nullptr) : QLabel(parent) {}
    
    std::vector<cv::Rect>& getRect() {
        return rect;
    }
    
    std::vector<cv::Mat>& getSprites() {
        return sprites;
    }
    
    void setRect(std::vector<cv::Rect> &rect) {
        this->rect = rect;
    }
    
    void setSprites(std::vector<cv::Mat> &sprites) {
        this->sprites = sprites;
    }
    
protected:
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent* event);
    
private:
    bool isPointInsideAABB(const cv::Point& point, const cv::Rect& box);
    bool aabb(cv::Rect a, cv::Rect b);
    std::vector<cv::Rect> rect;
    std::vector<cv::Mat> sprites;
};

#endif /* SpriteScene_hpp */