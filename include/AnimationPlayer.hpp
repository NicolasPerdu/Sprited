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
class QComboBox;
class AnimationScene;

class AnimationPlayer : public QLabel {
public:
    AnimationPlayer(MainWindow* win, QWidget* parent = nullptr);
    
    void regenCombo();
    
public slots:
    void playButtonAction();
    void stopButtonAction();
    //void topLeftAnchorButtonAction();
    //void centerAnchorButtonAction();
    void fpsInputAction(double val);
    void comboBoxUpdated(QString str);
    
protected:
    void paintEvent(QPaintEvent* event);
    
private:
    void animate();
    QPoint computeCenter(); 
    MainWindow *win = nullptr;
    QDoubleSpinBox *fpsInput = nullptr;
    QPushButton *playButton = nullptr;
    QComboBox *combo = nullptr;
    bool playing = false;
    //double fps = 60.f;
    QTimer *timer = nullptr;
    int currentFrame = 0;
    int currentNumFrame = 0;
    AnimationScene* currentScene = nullptr;
    //bool centerAnchor = false;
};

#endif /* AnimationPlayer_hpp */
