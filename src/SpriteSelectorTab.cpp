//
//  SpriteSelectorTab.cpp
//  Sprited
//
//  Created by Vulcain on 15/10/2023.
//

#include "../include/SpriteSelectorTab.hpp"
#include "../include/Utils.hpp"
#include <QImage>
#include <opencv2/opencv.hpp>

void SpriteSelectorTab::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    painter.setPen(QPen(Qt::red, 1));
    //painter.setBrush(QBrush(Qt::blue));
    
    // normal rectangle
    int x = 0;
    int y = 0;
    int maxH = 0;
    
    std::vector<std::pair<int, int>> pairs;
    pairs.reserve(rect.size());
    
    for(int i = 0; i < rect.size(); i++) {
        pairs.push_back(std::make_pair(rect[i].first.y*spritesheet.cols + rect[i].first.x, i));
    }
    
    std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
    
    numbers.clear();
    
    for(int i = 0; i < pairs.size(); i++) {
        //std::cout << "pos: " << x << ", " << y << ", " << maxH << std::endl;
        int index = pairs[i].second;
        cv::Mat img = sprites[index].second.clone();
        
        if(rect[index].second == 1) {
            cv::transpose(img, img);
            cv::flip(img, img, 0);
        }
        
        numbers.insert(std::make_pair(i, img));
        
        painter.drawImage(QPoint(x, y), toQImage(img));
        painter.drawText(x+5, y+10, QString::number(i));
        
        x = x + img.cols;
        maxH = std::max(maxH, img.rows);
        
        if(x > this->size().width()) {
            x = 0;
            y = y + maxH;
            maxH = 0;
        }
        
        //painter.drawRect(rect[i].x, rect[i].y, rect[i].width, rect[i].height);
    }
    
    //painter.setPen(QPen(Qt::red, 1));
    
    //for(int i = 0; i < rectSelected.size(); i++) {
     //   painter.drawRect(rectSelected[i].second.x, rectSelected[i].second.y, rectSelected[i].second.width, rectSelected[i].second.height);
    //}
}
