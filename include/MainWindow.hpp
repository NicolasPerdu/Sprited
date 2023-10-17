#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSpinBox;
class QPushButton;
class SpriteSelectorTab;
class SpriteScene; 

#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //void connectedComponentLabeling(QImage &binary, QImage &labels);
    //void floodFill(QImage &image, QImage &binary, int x, int y, int label);
    MainWindow(QWidget *parent = nullptr);
    bool isSelectEnabled() { return selectEnabled; }
    ~MainWindow();

public slots:
    void save();
    void openImage();
    void openEditor();
    void saveIndividualImages();
    void splitSpriteSheet(int area);
    void sliceButtonAction();
    void enableSelection();
    void exportJson();
    void merge();

private:
    QSpinBox *areaSlice = nullptr;
    QString filename;
    
    bool selectEnabled = false;
    QPushButton *selectButton = nullptr;
    Ui::MainWindow *ui;
    SpriteScene *scene = nullptr;
    SpriteSelectorTab *selectorTab = nullptr;
    cv::Mat spritesheet;
};
#endif // MAINWINDOW_H
