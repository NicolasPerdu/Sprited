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
#include <unordered_map>
#include <opencv2/opencv.hpp>

class SpriteSelectorTab : public QLabel {
public:
    SpriteSelectorTab(QWidget* parent = nullptr) : QLabel(parent) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        setGeometry(100, 100, 400, 400);
        
        paintEvent(nullptr);
    }
    
    std::vector<std::pair<cv::Rect, int>>& getRect() {
        return rect;
    }
    
    std::vector<std::pair<std::string,cv::Mat>>& getSprites() {
        return sprites;
    }
    
    std::unordered_map<int, cv::Mat>& getNumbers() {
        return numbers;
    }
    
    void setRect(std::vector<std::pair<cv::Rect, int>> &rect) {
        this->rect = rect;
    }
    
    void setSprites(std::vector<std::pair<std::string,cv::Mat>> &sprites) {
        this->sprites = sprites;
    }
    
    void setSpritesheet(cv::Mat &spritesheet) {
        this->spritesheet = spritesheet;
    }
protected:
    //void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent* event);
    
private:
    std::vector<std::pair<cv::Rect, int>> rect;
    std::vector<std::pair<std::string,cv::Mat>> sprites;
    std::unordered_map<int, cv::Mat> numbers;
    cv::Mat spritesheet;
};

#endif /* SpriteSelectorTab_hpp */
