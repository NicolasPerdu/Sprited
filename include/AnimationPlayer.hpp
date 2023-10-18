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

class AnimationPlayer : public QLabel {
public:
    AnimationPlayer(QWidget* parent = nullptr);
    
    void setMainWindow(MainWindow* main) {
        this->win = main;
    }
    
public slots:
    void playButtonAction();
    void stopButtonAction();
    
protected:
    void paintEvent(QPaintEvent* event);
    
private:
    MainWindow *win = nullptr;
    bool playing = false;
};

#endif /* AnimationPlayer_hpp */
