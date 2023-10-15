#include "include/MainWindow.hpp"
#include "include/SpriteSelectorTab.hpp"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QColorSpace>
#include <QPainter>
#include <QStack>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QPushButton>

#include <vector>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    //auto submenu = fileMenu->addMenu("Submenu");
    //submenu->addAction(new QAction("action1"));
    //submenu->addAction(new QAction("action2"));
    
    auto exportMenu = menuBar()->addMenu(tr("&Export"));
    QAction *exportJsonAct = new QAction(tr("&Export JSON..."), this);
    exportJsonAct->setStatusTip(tr("Export an json file"));
    connect(exportJsonAct, &QAction::triggered, this, &MainWindow::exportJson);
    exportMenu->addAction(exportJsonAct);
    
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setGeometry(500, 20, 200, 500);
    //tabWidget->setFixedSize(200, 200);

    QWidget *tab1 = new QWidget();
    tab1->setFixedSize(200, 200);
    
    QWidget *tab2 = new QWidget();
    tab2->setFixedSize(200, 200);
    
    // Create layouts for the tabs
    QVBoxLayout *sliceLayout = new QVBoxLayout(tab1);
    QVBoxLayout *editorLayout = new QVBoxLayout(tab2);
    
    QHBoxLayout *hLayoutSplit = new QHBoxLayout(tab1);
    QHBoxLayout *hLayoutExport = new QHBoxLayout(tab1);

    if(areaSlice == nullptr)
        areaSlice = new QSpinBox(tab1);
    
    QPushButton *sliceButton = new QPushButton("Slice", tab1);
    connect(sliceButton, &QPushButton::clicked, this, &MainWindow::sliceButtonAction);
    
    QPushButton *exportSpriteButton = new QPushButton("Sprites", tab1);
    connect(exportSpriteButton, &QPushButton::clicked, this, &MainWindow::saveIndividualImages);
    
    QPushButton *exportJsonButton = new QPushButton("Json", tab1);
    connect(exportJsonButton, &QPushButton::clicked, this, &MainWindow::exportJson);
    
    QPushButton *mergeButton = new QPushButton("Merge Box", tab1);
    connect(mergeButton, &QPushButton::clicked, this, &MainWindow::enableMerge);
    
    SpriteSelectorTab *selectorTab = new SpriteSelectorTab(tab2);
    //QPushButton *button2 = new QPushButton("Button in Tab 2", tab2);

    hLayoutSplit->addWidget(areaSlice);
    hLayoutSplit->addWidget(sliceButton);
    
    hLayoutExport->addWidget(exportSpriteButton);
    hLayoutExport->addWidget(exportJsonButton);
    
    sliceLayout->addLayout(hLayoutSplit);
    sliceLayout->addLayout(hLayoutExport);
    sliceLayout->addWidget(mergeButton);
    
    editorLayout->addWidget(selectorTab);
    
    tabWidget->addTab(tab1, "Slicing");
    tabWidget->addTab(tab2, "Editor");
}

void MainWindow::enableMerge() {
    mergeEnabled = !mergeEnabled;
}

void MainWindow::open()
{
    filename = QFileDialog::getOpenFileName(this,
                                                 tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    QImageReader reader(filename);
    reader.setAutoTransform(true);

    QImage spritesheet = reader.read();

    if (spritesheet.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return;
    }

    if (spritesheet.colorSpace().isValid())
        spritesheet.convertToColorSpace(QColorSpace::SRgb);

    auto minHeight = std::min(spritesheet.height(), 1080);
    auto smallImage = spritesheet.scaledToHeight(minHeight);
    
    ui->image->setPixmap(QPixmap::fromImage(smallImage));
    ui->image->adjustSize();
}

void MainWindow::sliceButtonAction() {
    ui->image->getRect().clear();
    ui->image->getSprites().clear();
    
    splitSpriteSheet(areaSlice->value());
    ui->image->update();
}

void MainWindow::saveIndividualImages() {
    if (ui->image->getSprites().size() == 0) {
        std::cerr << "Error: there are no sprites." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: there are no sprites."));
        return -1;
    }
    
    QString dir = QFileDialog::getExistingDirectory(this, "Save directory", "/");
    
    int spriteIndex = 0;
    
    for (const cv::Mat& spr : ui->image->getSprites()) {
        std::string filename2 = dir.toStdString() + "sprite" + std::to_string(spriteIndex++) + ".png";
        cv::imwrite(filename2, spr);
    }
}

void MainWindow::splitSpriteSheet(int area) {
    if (filename.size() == 0) {
        std::cerr << "Error: The filename is empty." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: The filename is empty."));
        return -1;
    }
    
    cv::Mat spritesheet = cv::imread(filename.toStdString());

    if (spritesheet.empty()) {
        std::cerr << "Error: Could not load spritesheet image." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: Could not load spritesheet image."));
        return -1;
    }
    
    //QString dir = QFileDialog::getExistingDirectory(this, "Save directory", "/");
    
    cv::Mat gray;
    cv::cvtColor(spritesheet, gray, cv::COLOR_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(gray, binary, 254, 255, cv::THRESH_BINARY);
    
    //cv::imwrite("/Users/vulcain/Sprited/res/spriteBinary.png", binary);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<cv::Mat> individualSprites;
    individualSprites.reserve(contours.size());
    
    int spriteIndex = 0;
    
    std::vector<cv::Rect> rect;
    rect.reserve(contours.size());
    
    for (const std::vector<cv::Point>& contour : contours) {
        cv::Rect boundingBox = cv::boundingRect(contour);
        
        cv::Mat sprite = spritesheet(boundingBox).clone();
            
        if (boundingBox.area() > area && (boundingBox.width < binary.cols && boundingBox.height < binary.rows)) {
            rect.push_back(boundingBox);
            individualSprites.push_back(sprite);
            //std::string filename = dir.toStdString() + "sprite" + std::to_string(spriteIndex++) + ".png";
            //cv::imwrite(filename, sprite);
        }
    }
    
    ui->image->setRect(rect);
    ui->image->setSprites(individualSprites);
}

void MainWindow::exportJson() {
    if (ui->image->getRect().empty()) {
        std::cerr << "Error: Could not find the bounding boxes." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: Could not find the bounding boxes."));
        return -1;
    }
    
    QString dir = QFileDialog::getExistingDirectory(this, "Save directory", "/");
    
    QJsonObject root;
    
    for(int i = 0; i < ui->image->getRect().size(); i++) {
        QJsonArray rect;
        rect.push_back(ui->image->getRect()[i].x);
        rect.push_back(ui->image->getRect()[i].y);
        rect.push_back(ui->image->getRect()[i].width);
        rect.push_back(ui->image->getRect()[i].height);
        std::string id = "spr" + std::to_string(i);
    
        root[id.c_str()] = rect;
    }
    
    QByteArray ba = QJsonDocument(root).toJson();

    QFile fout(dir + "/spr.json");
    fout.open(QIODevice::WriteOnly);
    fout.write(ba);
}

/*void MainWindow::floodFill(QImage &image, QImage &binary, int x, int y, int label) {
    int width = image.width();
    int height = image.height();
    QStack<QPoint> stack;
    stack.push(QPoint(x, y));

    while (!stack.isEmpty()) {
        QPoint point = stack.pop();
        int px = point.x();
        int py = point.y();

        if (px < 0 || px >= width || py < 0 || py >= height) {
            continue;
        }

        if (binary.pixel(px, py) == qRgb(0, 0, 0)) {
            continue;
        }

        image.setPixel(px, py, qRgb(label, label, label));
        binary.setPixel(px, py, qRgb(0, 0, 0));

        stack.push(QPoint(px + 1, py));
        stack.push(QPoint(px - 1, py));
        stack.push(QPoint(px, py + 1));
        stack.push(QPoint(px, py - 1));
    }
}*/

/*void MainWindow::connectedComponentLabeling(QImage &binary, QImage &labels) {
    int width = binary.width();
    int height = binary.height();
    int currentLabel = 1;
    QVector<int> labelsTable(1, 0); // Initialize label equivalence table

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (binary.pixel(x, y) == qRgb(255, 255, 255)) {
                QVector<int> neighbors;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;

                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int neighborLabel = labels.pixel(nx, ny);
                            if (neighborLabel > 0) {
                                neighbors.append(neighborLabel);
                            }
                        }
                    }
                }

                if (neighbors.isEmpty()) {
                    labels.setPixel(x, y, currentLabel);
                    labelsTable.append(currentLabel);
                    currentLabel++;
                } else {
                    // Assign the minimum label from neighbors
                    int minNeighborLabel = *std::min_element(neighbors.begin(), neighbors.end());
                    labels.setPixel(x, y, minNeighborLabel);

                    // Update equivalence table
                    for (int neighbor : neighbors) {
                        if (neighbor != minNeighborLabel) {
                            for (int i = 1; i < labelsTable.size(); ++i) {
                                if (labelsTable[i] == neighbor) {
                                    labelsTable[i] = minNeighborLabel;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Update labels using the equivalence table
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int currentLabel = labels.pixel(x, y);
            if (currentLabel > 0) {
                labels.setPixel(x, y, labelsTable[currentLabel]);
            }
        }
    }
}*/

MainWindow::~MainWindow()
{
    delete ui;
}

