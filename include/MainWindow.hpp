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
    void writeTGA(std::string filepath, cv::Mat& spritesheet);
    
    SpriteScene* getSpriteScene() {
        return scene;
    }
    
    AnimationScene* getAnimationScene(int index) {
        return animationScenes[index];
    }
    
    AnimationScene* getAnimationSceneByName(std::string name) {
        auto it = std::find_if(animationScenes.begin(), animationScenes.end(), [name] (AnimationScene* a) { return a->getName() == name; } );
        
        if(it != animationScenes.end())
            return *(it);
        else
            return nullptr; 
    }
    
    int getAnimationsSize() {
        return animationScenes.size();
    }
    
    SpriteSelectorTab* getSelectorTab() {
        return selectorTab;
    }

public slots:
    void saveSpriteSheet();
    void saveProject();
    void open();
    void openEditor();
    void importSprites();
    void saveIndividualImages();
    void splitSpriteSheet(int area);
    void sliceButtonAction();
    void enableSelection();
    //void exportJson();
    void sliceGrid();
    void createAnimationScene(QString name);
    void updateBaseFilename(QString str); 
    void merge();
    void openNewAnim(int tabIndex); 

private:
    //void writeData(FILE* outputFile, const int w, const int h, const int channels, const int mipCount, const unsigned char* inRow);
    void loadJsonProject();
    void saveJson(QJsonObject root, QString filenameJson);
    int loadImage(); 
    
    QSpinBox *areaSlice = nullptr;
    QString filename, filenameProject;
    
    QSpinBox* xGrid = nullptr;
    QSpinBox* yGrid = nullptr; 
    
    bool selectEnabled = false;
    QPushButton *selectButton = nullptr;
    Ui::MainWindow *ui;
    SpriteScene *scene = nullptr;
    SpriteSelectorTab *selectorTab = nullptr;
    std::vector<AnimationScene*> animationScenes;
    QTabWidget *animationTab = nullptr;
    AnimationPlayer *playerScene = nullptr;
    cv::Mat spritesheet;
    std::string baseFilename;
    QWidget *animationPlusTab = nullptr;
};
#endif // MAINWINDOW_H
