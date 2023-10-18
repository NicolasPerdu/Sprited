//
//  SpriteSelectorTab.hpp
//  Sprited
//
//  Created by Vulcain on 15/10/2023.
//

#ifndef SpriteSelectorTab_hpp
#define SpriteSelectorTab_hpp

#include <stdio.h>

#include <stdio.h>
#include <QLabel>
#include <QPainter>
#include <vector>
#include <opencv2/opencv.hpp>

class SpriteSelectorTab : public QLabel {
public:
    SpriteSelectorTab(QWidget* parent = nullptr) : QLabel(parent) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        setGeometry(100, 100, 400, 400);
    }
    
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
    //void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent* event);
    
private:
    std::vector<cv::Rect> rect;
    std::vector<cv::Mat> sprites;
};

#endif /* SpriteSelectorTab_hpp */
