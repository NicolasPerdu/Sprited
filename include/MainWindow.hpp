#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSpinBox;
class QPushButton;
class SpriteSelectorTab;

#include "include/SpriteScene.hpp"
#include "include/AnimationScene.hpp"
#include "include/AnimationPlayer.hpp"
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
    
    SpriteScene* getSpriteScene() {
        return scene;
    }
    
    AnimationScene* getAnimationScene() {
        return animationScene;
    }

public slots:
    void saveProject();
    void open();
    void openEditor();
    void importSprites();
    void saveIndividualImages();
    void splitSpriteSheet(int area);
    void sliceButtonAction();
    void enableSelection();
    void exportJson();
    void merge();

private:
    void loadJsonProject();
    void saveJson(QJsonObject root, QString filenameJson);
    void loadImage(); 
    
    QSpinBox *areaSlice = nullptr;
    QString filename, filenameProject;
    
    bool selectEnabled = false;
    QPushButton *selectButton = nullptr;
    Ui::MainWindow *ui;
    SpriteScene *scene = nullptr;
    SpriteSelectorTab *selectorTab = nullptr;
    AnimationScene *animationScene = nullptr;
    AnimationPlayer *playerScene = nullptr;
    cv::Mat spritesheet;
};
#endif // MAINWINDOW_H
