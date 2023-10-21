//
//  AnimationScene.hpp
//  Sprited
//
//  Created by Vulcain on 17/10/2023.
//

#ifndef AnimationScene_hpp
#define AnimationScene_hpp

#include <stdio.h>
#include <QWidget>
#include <opencv2/opencv.hpp>

class QTableWidget;

class AnimationScene : QWidget {
public:
    AnimationScene(QWidget* parent = nullptr);
    
    std::vector<std::pair<cv::Rect, int>>& getRect() {
        return rect;
    }
    
    std::vector<cv::Mat>& getSprites() {
        return sprites;
    }
    
    void setRect(std::vector<std::pair<cv::Rect, int>> &rect) {
        this->rect = rect;
    }
    
    void setSprites(std::vector<cv::Mat> &sprites) {
        this->sprites = sprites;
    }
    
    std::vector<std::pair<int, int>> getTable();
    void setTable(std::vector<std::pair<int, int>>& table);
    
    
protected:
    //void mousePressEvent(QMouseEvent *ev);
    //void paintEvent(QPaintEvent* event);
    
private:
    std::vector<std::pair<cv::Rect, int>> rect;
    std::vector<cv::Mat> sprites;
    QTableWidget *table = nullptr;
};

#endif /* AnimationScene_hpp */
