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

class MainWindow;

class SpriteScene : public QLabel {
public:
    SpriteScene(QWidget* parent = nullptr);
    
    void setMainWindow(MainWindow* main) {
        this->win = main;
    }
    
    std::vector<std::pair<cv::Rect, int>>& getRect() {
        return rect;
    }
    
    std::vector<std::pair<std::string,cv::Mat>>& getSprites() {
        return sprites;
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
    
    cv::Rect mergeRect();
    
protected:
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent* event);
    
private:
    bool isPointInsideAABB(const cv::Point& point, const cv::Rect& box);
    //bool aabb(cv::Rect a, cv::Rect b);
    std::vector<std::pair<cv::Rect, int>> rect;
    std::vector<std::pair<int, cv::Rect>> rectSelected;
    std::vector<std::pair<std::string,cv::Mat>> sprites;
    cv::Mat spritesheet;
    MainWindow *win = nullptr;
};

#endif /* SpriteScene_hpp */
