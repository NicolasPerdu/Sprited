//
//  SpriteSelectorTab.cpp
//  Sprited
//
//  Created by Vulcain on 15/10/2023.
//

#include "../include/SpriteSelectorTab.hpp"
#include <QImage>
#include <opencv2/opencv.hpp>

QImage SpriteSelectorTab::MatToQImage(const cv::Mat& mat)
{
    // Check if the input Mat is empty
    if (mat.empty())
    {
        return QImage();
    }

    // Determine the format of the QImage based on the Mat's number of channels
    QImage::Format format;
    if (mat.channels() == 1)
    {
        format = QImage::Format_Grayscale8;
    }
    else if (mat.channels() == 3)
    {
        format = QImage::Format_RGB888;
    }
    else if (mat.channels() == 4)
    {
        format = QImage::Format_ARGB32;
    }
    else
    {
        return QImage(); // Unsupported format
    }

    // Create a QImage from the Mat data
    QImage image((uchar *) mat.data, mat.cols, mat.rows, static_cast<qsizetype >(mat.step), format);

    // If the Mat is stored as BGR, convert it to RGB format
    if (mat.channels() == 3 && mat.type() == CV_8UC3)
    {
        image = image.rgbSwapped();
    }

    return image;
}

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
        painter.drawImage(QPoint(x, y), MatToQImage(sprites[i]));
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
