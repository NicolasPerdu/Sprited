//
//  AnimationPlayer.hpp
//  Sprited
//
//  Created by Vulcain on 18/10/2023.
//

#ifndef AnimationPlayer_hpp
#define AnimationPlayer_hpp

#include <stdio.h>
#include <QLabel>
#include <QDoubleSpinBox>

class MainWindow;
class QTimer;
class QPushButton;

class AnimationPlayer : public QLabel {
public:
    AnimationPlayer(QWidget* parent = nullptr);
    
    void setMainWindow(MainWindow* main) {
        this->win = main;
    }
    
    double getFps() {
        return fps;
    }
    
    void setFps(double fps) {
        this->fps = fps;
        fpsInput->setValue(fps);
    }
    
public slots:
    void playButtonAction();
    void stopButtonAction();
    //void topLeftAnchorButtonAction();
    //void centerAnchorButtonAction();
    void fpsInputAction(double val);
    
protected:
    void paintEvent(QPaintEvent* event);
    
private:
    void animate();
    QPoint computeCenter(); 
    MainWindow *win = nullptr;
    QDoubleSpinBox *fpsInput = nullptr;
    QPushButton *playButton = nullptr; 
    bool playing = false;
    double fps = 60.f;
    QTimer *timer = nullptr;
    int currentFrame = 0;
    int currentNumFrame = 0;
    //bool centerAnchor = false;
};

#endif /* AnimationPlayer_hpp */
