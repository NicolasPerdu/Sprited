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
    
    //painter.setPen(QPen(Qt::black, 1));
    //painter.setBrush(QBrush(Qt::blue));
    
    // normal rectangle
    int x = 0;
    int y = 0;
    int maxH = 0;
    
    for(int i = 0; i < rect.size(); i++) {
        std::cout << "pos: " << x << ", " << y << ", " << maxH << std::endl; 
        painter.drawImage(QPoint(x, y), toQImage(sprites[i]));
        x = x + rect[i].width;
        maxH = std::max(maxH, rect[i].height);
        
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
