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

class MainWindow;
class QTimer;

class AnimationPlayer : public QLabel {
public:
    AnimationPlayer(QWidget* parent = nullptr);
    
    void setMainWindow(MainWindow* main) {
        this->win = main;
    }
    
public slots:
    void playButtonAction();
    void stopButtonAction();
    void topLeftAnchorButtonAction();
    void centerAnchorButtonAction();
    
protected:
    void paintEvent(QPaintEvent* event);
    
private:
    void animate();
    QPoint computeCenter(); 
    MainWindow *win = nullptr;
    bool playing = false;
    QTimer *timer = nullptr;
    int currentFrame = 0;
    int currentNumFrame = 0;
    bool centerAnchor = false;
};

#endif /* AnimationPlayer_hpp */
