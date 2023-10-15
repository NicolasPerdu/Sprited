#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSpinBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //void connectedComponentLabeling(QImage &binary, QImage &labels);
    //void floodFill(QImage &image, QImage &binary, int x, int y, int label);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void open();
    void saveIndividualImages();
    void splitSpriteSheet(int area);
    void sliceButtonAction();
    void enableMerge();
    void exportJson();

private:
    QSpinBox *areaSlice = nullptr;
    QString filename;
    Ui::MainWindow *ui;
    bool mergeEnabled = false;
};
#endif // MAINWINDOW_H
