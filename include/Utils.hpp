//
//  Utils.hpp
//  Sprited
//
//  Created by Vulcain on 18/10/2023.
//

#ifndef Utils_h
#define Utils_h

#include <QImage>
#include <opencv2/opencv.hpp>

inline QImage toQImage(const cv::Mat& mat)
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

#endif /* Utils_h */
