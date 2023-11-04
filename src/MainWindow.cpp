#include "include/MainWindow.hpp"
#include "include/SpriteSelectorTab.hpp"
#include "include/AnimationScene.hpp"
#include "include/GuillotineBinPack.hpp"
#include "include/MaxRectsBinPack.hpp"
#include "include/Utils.hpp"

#include "include/tga.h"

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
#include <QLineEdit>
#include <QInputDialog>

#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Sprited");
    
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    QAction *saveProjectAct = new QAction(tr("&Save Project..."), this);
    saveProjectAct->setShortcuts(QKeySequence::Save);
    saveProjectAct->setStatusTip(tr("Save an existing project"));
    connect(saveProjectAct, &QAction::triggered, this, &MainWindow::saveProject);
    
    QAction *saveSpriteAct = new QAction(tr("&Save Spritesheet..."), this);
    //saveProjectAct->setShortcuts(QKeySequence::Save);
    saveSpriteAct->setStatusTip(tr("Save an existing spritesheet"));
    connect(saveSpriteAct, &QAction::triggered, this, &MainWindow::saveSpriteSheet);
    
    QAction *importAct = new QAction(tr("&Import Sprites..."), this);
    importAct->setShortcuts(QKeySequence::Italic);
    importAct->setStatusTip(tr("Import individual sprites"));
    connect(importAct, &QAction::triggered, this, &MainWindow::importSprites);

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveProjectAct);
    fileMenu->addAction(saveSpriteAct);
    fileMenu->addAction(importAct);
    //auto submenu = fileMenu->addMenu("Submenu");
    //submenu->addAction(new QAction("action1"));
    //submenu->addAction(new QAction("action2"));
    
    /*auto exportMenu = menuBar()->addMenu(tr("&Export"));
    QAction *exportJsonAct = new QAction(tr("&Export JSON..."), this);
    exportJsonAct->setStatusTip(tr("Export an json file"));
    connect(exportJsonAct, &QAction::triggered, this, &MainWindow::exportJson);
    exportMenu->addAction(exportJsonAct);*/
    
    QTabWidget *mainTabWidget = new QTabWidget(this);
    mainTabWidget->setTabPosition(QTabWidget::North);
    mainTabWidget->setGeometry(0, 0, 1000, 900);
    
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setGeometry(1450, 20, 200, 800);
    //tabWidget->setFixedSize(200, 200);
    
    QWidget *spriteTab = new QWidget();
    spriteTab->setFixedSize(1000, 900);
    
    scene = new SpriteScene(spriteTab);
    scene->setMainWindow(this);
    
    animationTab = new QTabWidget();
    animationTab->setTabPosition(QTabWidget::North);
    animationTab->setFixedSize(1000, 900);
    
    //QWidget *animationSubTab = new QWidget(animationTab);
    //animationSubTab->setFixedSize(1000, 900);
    
    animationPlusTab = new QWidget(animationTab);
    animationPlusTab->setFixedSize(1000, 900);
    connect(animationTab, &QTabWidget::tabBarClicked, this, &MainWindow::openNewAnim);
    
    QWidget *playerTab = new QWidget();
    playerTab->setFixedSize(1000, 900);
    
    //animationScenes.push_back(new AnimationScene(animationSubTab));
    playerScene = new AnimationPlayer(this, playerTab);

    QWidget *slicingTab = new QWidget();
    slicingTab->setFixedSize(200, 800);
    
    QWidget *editorTab = new QWidget();
    editorTab->setFixedSize(200, 800);
    
    // Create layouts for the tabs
    QVBoxLayout *sliceLayout = new QVBoxLayout(slicingTab);
    QVBoxLayout *editorLayout = new QVBoxLayout(editorTab);
    
    QHBoxLayout *hLayoutSplit = new QHBoxLayout(slicingTab);
    QHBoxLayout *hLayoutExport = new QHBoxLayout(slicingTab);

    if(areaSlice == nullptr)
        areaSlice = new QSpinBox(slicingTab);
    
    QPushButton *sliceButton = new QPushButton(tr("Slice Contour"), slicingTab);
    connect(sliceButton, &QPushButton::clicked, this, &MainWindow::sliceButtonAction);
    
    QPushButton *exportSpriteButton = new QPushButton(tr("Split Sprites"), slicingTab);
    connect(exportSpriteButton, &QPushButton::clicked, this, &MainWindow::saveIndividualImages);
    
    QPushButton *exportJsonButton = new QPushButton("Json", slicingTab);
    connect(exportJsonButton, &QPushButton::clicked, this, &MainWindow::exportJson);
    
    selectButton = new QPushButton("Select Box", slicingTab);
    connect(selectButton, &QPushButton::clicked, this, &MainWindow::enableSelection);
    
    QPushButton *mergeButton = new QPushButton("Merge", slicingTab);
    connect(mergeButton, &QPushButton::clicked, this, &MainWindow::merge);
    
    QPushButton *editorButton = new QPushButton("Open Editor", editorTab);
    connect(editorButton, &QPushButton::clicked, this, &MainWindow::openEditor);
    
    hLayoutSplit->addStretch();
    hLayoutSplit->addWidget(areaSlice);
    hLayoutSplit->addWidget(sliceButton);
    hLayoutSplit->addStretch();
    
    hLayoutExport->addStretch();
    hLayoutExport->addWidget(exportSpriteButton);
    hLayoutExport->addWidget(exportJsonButton);
    hLayoutExport->addStretch();
    
    QHBoxLayout *hLayoutSelect = new QHBoxLayout(slicingTab);
    hLayoutSelect->addStretch();
    hLayoutSelect->addWidget(selectButton);
    hLayoutSelect->addWidget(mergeButton);
    hLayoutSelect->addStretch();
    
    xGrid = new QSpinBox(slicingTab);
    yGrid = new QSpinBox(slicingTab);
    
    QPushButton *sliceGridButton = new QPushButton(tr("Slice Grid"), editorTab);
    connect(sliceGridButton, &QPushButton::clicked, this, &MainWindow::sliceGrid);
    
    QHBoxLayout *hLayoutSelect2 = new QHBoxLayout(slicingTab);
    hLayoutSelect2->addStretch();
    hLayoutSelect2->addWidget(xGrid);
    hLayoutSelect2->addWidget(yGrid);
    hLayoutSelect2->addWidget(sliceGridButton);
    hLayoutSelect2->addStretch();
    
    //QHBoxLayout *hLayoutMerge = new QHBoxLayout(tab1);
    //hLayoutMerge->addStretch();
    //hLayoutMerge->addStretch();
    
    sliceLayout->addLayout(hLayoutSplit);
    sliceLayout->addLayout(hLayoutSelect);
    sliceLayout->addLayout(hLayoutSelect2);
    //sliceLayout->addLayout(hLayoutMerge);
    QLabel *exportLabel = new QLabel(tr("Exporting"), slicingTab);
    QLineEdit *baseFilename = new QLineEdit(slicingTab);
    connect(baseFilename, &QLineEdit::textChanged, this, &MainWindow::updateBaseFilename);
    
    sliceLayout->addWidget(baseFilename);
    sliceLayout->addWidget(exportLabel);
    sliceLayout->addLayout(hLayoutExport);
    sliceLayout->addStretch();
    
    editorLayout->addWidget(editorButton);
    editorLayout->addStretch();
    
    //animationTab->addTab(animationSubTab, tr("current"));
    animationTab->addTab(animationPlusTab, tr("+"));
    
    //QObject::connect(animationPlusTab, &QTabWidget::tabBarClicked, openNewAnim);
    
    mainTabWidget->addTab(spriteTab, tr("Sprite"));
    mainTabWidget->addTab(animationTab, tr("Animation"));
    mainTabWidget->addTab(playerTab, tr("Player"));
    
    tabWidget->addTab(slicingTab, tr("Slicing"));
    tabWidget->addTab(editorTab, tr("Editor"));
    
    if(selectorTab == nullptr) {
        selectorTab = new SpriteSelectorTab();
    }
}

void MainWindow::updateBaseFilename(QString str) {
    baseFilename = str.toStdString();
}

void MainWindow::openNewAnim(int tabIndex) {
    if(tabIndex < (animationTab->count()-1))
        return;
    
    bool ok;
    QString name = QInputDialog::getText(this, "Enter a name for the animation", "Name:", QLineEdit::Normal, QString(), &ok);

    if(!ok)
        return;
    
    QWidget *animationSubTab = new QWidget(animationTab);
    animationSubTab->setFixedSize(1000, 900);
    
    animationScenes.push_back(new AnimationScene(name.toStdString(), animationSubTab));
    
    animationTab->removeTab(animationTab->count()-1);
    animationTab->addTab(animationSubTab, name);
    animationTab->addTab(animationPlusTab, "+");
    
    playerScene->regenCombo();
}

void MainWindow::sliceGrid() {
    int w = xGrid->value();
    int h = yGrid->value();
    
    if(w <= 0 || h <= 0)
        return;
    
    cv::Rect baseRect{0, 0, w, h};
    
    int numX = spritesheet.cols/w;
    int numY = spritesheet.rows/h;
    
    std::vector<std::pair<cv::Rect, int>> rects;
    rects.reserve((numX-1)*(numY-1));
    
    std::vector<std::pair<std::string,cv::Mat>> sprites;
    sprites.reserve((numX-1)*(numY-1));
    
    for(int i = 0; i < numX; i++) {
        for(int j = 0; j < numY; j++) {
            cv::Rect r{i*w, j*h, w, h};
            rects.push_back(std::make_pair(r, 0));
            std::cout << "r :" << rects.back().first.x << ", " << rects.back().first.y << ", " << rects.back().first.width << ", " << rects.back().first.height << std::endl;
            cv::Mat sprite = spritesheet(r).clone();
            sprites.push_back(std::make_pair(baseFilename+std::to_string(i), sprite));
        }
    }
    
    scene->setRect(rects);
    scene->setSprites(sprites);
    scene->update();
    
    if(selectorTab == nullptr) {
        selectorTab = new SpriteSelectorTab();
    }
    
    selectorTab->setRect(scene->getRect());
    selectorTab->setSprites(scene->getSprites());
    selectorTab->update();
}

void MainWindow::openEditor() {
    if(selectorTab == nullptr) {
        selectorTab = new SpriteSelectorTab();
    }
    
    selectorTab->show();
    
    selectorTab->setSpritesheet(spritesheet);
    selectorTab->setRect(scene->getRect());
    selectorTab->setSprites(scene->getSprites());
}

void MainWindow::writeTGA(std::string filepath, cv::Mat& ss) {
    uint8_t idLength = '\0';
    uint8_t colormapType     =  '\0';
    uint8_t imageType     =   '\x02';
    uint16_t colormapOrigin =       0;
    uint16_t colormapLength  =      0;
    uint8_t colormapDepth  =      '\0';
    uint16_t xOrigin    =    0;
    uint16_t yOrigin  =      0;
    uint16_t width    =    ss.cols;
    uint16_t height   =     ss.rows;
    uint8_t bitsPerPixel    =    '\x20'; // 20
    uint8_t imageDescriptor     =   ' ';
    std::string imageId    =    "";
    
    cv::Mat outputMat;
    cv::cvtColor(ss, outputMat, cv::COLOR_BGRA2RGBA);
    
    FILE* f = std::fopen(filepath.c_str(), "wb");
    tga::StdioFileInterface file(f);
    tga::Encoder encoder(&file);
    
    tga::Header header = {idLength, colormapType, imageType, colormapOrigin, colormapLength, colormapDepth, xOrigin, yOrigin, width , height, bitsPerPixel, imageDescriptor, imageId};
    
    tga::Image image;
    image.bytesPerPixel = header.bytesPerPixel();
    image.rowstride = header.width * header.bytesPerPixel();
    image.pixels = outputMat.data;
   
    encoder.writeHeader(header);
    encoder.writeImage(header, image);
    encoder.writeFooter();

    fclose(f);
}

void MainWindow::merge() {
    scene->mergeRect();
    
    if(selectorTab == nullptr) {
        selectorTab = new SpriteSelectorTab();
    }
    
    selectorTab->setSpritesheet(spritesheet);
    selectorTab->setRect(scene->getRect());
    selectorTab->setSprites(scene->getSprites());
    selectorTab->update();
}

void MainWindow::enableSelection() {
    selectEnabled = !selectEnabled;
    
    if(selectEnabled) {
        selectButton->setStyleSheet("background-color: #0046fc; border-width: 1px; border-radius: 6px; padding: 3px;");
    } else {
        selectButton->setStyleSheet("");
    }
}

void MainWindow::importSprites()
{
    QString directoryPath = QFileDialog::getExistingDirectory(this, "Select a Directory", QDir::homePath(), QFileDialog::ShowDirsOnly);
    
    QDir directory(directoryPath);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QStringList fileList = directory.entryList();
    
    std::vector<std::pair<cv::Rect, int>> rects;
    std::vector<std::pair<std::string,cv::Mat>> images;
    
    foreach (QString file, fileList) {
        //QFileInfo fileInfo(directory, file);
        QFileInfo fileInfo(directoryPath, file);
        //std::cout << "open the file : " << (directoryPath.toStdString() + "/" + file.toStdString()) << std::endl;
        auto img = cv::imread(directoryPath.toStdString() + "/" + file.toStdString(), cv::IMREAD_UNCHANGED);
        rects.push_back(std::make_pair(cv::Rect(0, 0, img.cols, img.rows), false));
        images.push_back(std::make_pair(fileInfo.baseName().toStdString(), img));
        //
    }
    
    int sizePack = 2;
    bool sucess = false;
    rbp::GuillotineBinPack bp;
    auto temp = images;
    std::vector<std::pair<cv::Rect, int>> temp_rects = rects;
    
    while(!sucess) {
        sucess = true;
        bp.Init(sizePack, sizePack);
        
        for (int i = 0; i < rects.size(); i++) {
            auto r = bp.Insert(rects[i].first.width, rects[i].first.height, false, rbp::GuillotineBinPack::RectBestShortSideFit, rbp::GuillotineBinPack::GuillotineSplitHeuristic::SplitMinimizeArea);
            
            temp_rects[i].first = cv::Rect(r.x, r.y, r.width, r.height);
            
            if(r.height > 0 && rects[i].first.width == r.height) {
                cv::Mat rotatedImage;
                cv::transpose(temp[i].second, rotatedImage);
                cv::flip(rotatedImage, rotatedImage, 1);
                temp[i].second = rotatedImage;
                temp_rects[i].second = true;
            }
            
            if (r.height == 0) {
                sucess = false;
                sizePack = sizePack*2;
                temp = images;
                temp_rects = rects;
                break;
            }
        }
    }
    
    auto temp2 = images;
    std::vector<std::pair<cv::Rect, int>> temp_rects2 = rects;
    
    rbp::MaxRectsBinPack bp2(sizePack, sizePack);
    bool success2 = true;
    
    for (int i = 0; i < rects.size(); i++) {
        auto r = bp2.Insert(rects[i].first.width, rects[i].first.height, rbp::MaxRectsBinPack::RectBestShortSideFit);
        
        temp_rects2[i].first = cv::Rect(r.x, r.y, r.width, r.height);
        
        if(r.height > 0 && rects[i].first.width == r.height) {
            cv::Mat rotatedImage;
            cv::transpose(temp[i].second, rotatedImage);
            cv::flip(rotatedImage, rotatedImage, 1);
            temp2[i].second = rotatedImage;
            temp_rects2[i].second = true;
        }
        
        if (r.height == 0) {
            success2 = false;
            break;
        }
    }
    
    std::vector<rbp::Rect> bpr;
    if(success2 && bp2.Occupancy() > bp.Occupancy()) {
        rects = temp_rects2;
        images = temp2;
        bpr = bp2.GetUsedRectangles();
    } else {
        rects = temp_rects;
        images = temp;
        bpr = bp.GetUsedRectangles();
    }
    
    std::cout << "res pack : " << bp.Occupancy() << ", " <<  bp2.Occupancy() << "% size : " << sizePack << std::endl;
    spritesheet = cv::Mat(sizePack, sizePack, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    
    for (int i = 0; i < bpr.size(); i++) {
        cv::Mat img = images[i].second;
        
        if(img.type() == CV_8UC3) {
            cv::Mat image4Channel(img.rows, img.cols, CV_8UC4);

            for (int y = 0; y < img.rows; y++) {
                for (int x = 0; x < img.cols; x++) {
                    cv::Vec3b bgr = img.at<cv::Vec3b>(y, x);
                    cv::Vec4b bgra(bgr[0], bgr[1], bgr[2], 255);
                    image4Channel.at<cv::Vec4b>(y, x) = bgra;
                }
            }
            images[i].second = image4Channel;
        }
        
        cv::Rect roi(bpr[i].x, bpr[i].y, bpr[i].width, bpr[i].height);
        //std::cout << "copy img(" << roi.x << ", " << roi.y << ", " << roi.width << ", " << roi.height << ") to (" << spritesheet.cols << ", " << spritesheet.rows << std::endl;
        images[i].second.copyTo(spritesheet(roi));
    }
    
    scene->setRect(rects);
    scene->setSprites(images);
    scene->setSpritesheet(spritesheet);
    
    QDir dirSS(directoryPath);
    dirSS.cdUp();
    
    filename = dirSS.path();
    filename.append("/spritesheet.png");
    
    QImage ss = toQImage(spritesheet);
    
    auto minHeight = std::min(ss.height(), 1080);
    auto smallImage = ss.scaledToHeight(minHeight);
    
    scene->setPixmap(QPixmap::fromImage(smallImage));
    scene->adjustSize();
}

void MainWindow::saveSpriteSheet()
{
    QString filenameSS = QFileDialog::getSaveFileName(this, tr("Save Spritesheet"), QDir::homePath());
                                                     // tr("Images (*.png *.jpg *.jp2 *tif *.tga *.gif *.bmp *pam *.ppm)"));
    
    if(filenameSS.size() > 0) {
        QFileInfo fileInfoSS(filenameSS);
        
        //if(fileInfoSS.isFile()) {
            std::cout << "ext : " << fileInfoSS.completeSuffix().toStdString() << std::endl;
            
            if(fileInfoSS.completeSuffix() == "png") {
                cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "jpg") {
                cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "jp2") {
                cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "tif") {
                cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "tga") {
                writeTGA(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "gif") { // ko
                //cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "bmp") { // ok
                cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "pam") { // ok
                cv::imwrite(filenameSS.toStdString(), spritesheet);
            } else if(fileInfoSS.completeSuffix() == "ppm") { // ok
                cv::Mat out;
                cv::cvtColor(spritesheet, out, cv::COLOR_BGRA2BGR);
                cv::imwrite(filenameSS.toStdString(), out);
            }
        //}
    }
}


void MainWindow::saveProject()
{
    filenameProject = QFileDialog::getSaveFileName(this, tr("Save Project"), QDir::homePath(), tr("Json Files (*.json)"));
    
    QJsonObject root;
    
    if(filename.size() > 0) {
        QFileInfo fileInfoProj(filenameProject);
        QFileInfo fileInfo(filename);
        QDir dir(fileInfoProj.dir().path());
        //std::cout << "file project : " << filenameProject.toStdString() << std::endl;
        //std::cout << "file : " << filename.toStdString() << std::endl;
        QString rel = dir.relativeFilePath(fileInfo.dir().path());
        
        QJsonValue val;
        
        if(rel == ".")
            val = fileInfo.fileName();
        else
            val = rel + "/" + fileInfo.fileName();
       
        root["spritesheet"] = val;
        
        auto rectA = scene->getRect();
        auto spriteA = scene->getSprites();
        
        if(rectA.size() > 0) {
            QJsonArray rectG;
            
            for(int i = 0; i < rectA.size(); i++) {
                QJsonArray rect;
                rect.push_back(QString::fromStdString(spriteA[i].first));
                rect.push_back(rectA[i].first.x);
                rect.push_back(rectA[i].first.y);
                rect.push_back(rectA[i].first.width);
                rect.push_back(rectA[i].first.height);
                rect.push_back(rectA[i].second);
            
                rectG.push_back(rect);
            }
            
            root["rect"] = rectG;
        }
        
        
        auto table = animationScenes[0]->getTable();
        
        if(table.size() > 0) {
            QJsonArray animG;
            
            for(int i = 0; i < table.size(); i++) {
                QJsonArray anim;
                anim.push_back(table[i].first);
                anim.push_back(table[i].second);
                animG.push_back(anim);
            }
            
            root["anim"] = animG;
            root["fps"] = playerScene->getFps();
        }
        
    }
    
    saveJson(root, filenameProject);
    
}

void MainWindow::loadJsonProject() {
    
    QFileInfo fileInfo(filenameProject);
    QFile jsonFile(filenameProject);

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open the JSON file.";
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON:" << parseError.errorString();
        return;
    }

    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        if(jsonObj.contains("spritesheet")) {
            filename = fileInfo.dir().path() + "/" + jsonObj["spritesheet"].toString();
            
        } else {
            filename = QFileDialog::getOpenFileName(this, tr("Open Spritesheet"), QDir::homePath(), tr("Images Files (*.*)"));
        }
        
        int res = loadImage();
        
        if(res == 0) {
            spritesheet = cv::imread(filename.toStdString(), cv::IMREAD_UNCHANGED);
            scene->setSpritesheet(spritesheet);
        }
        
        if(jsonObj.contains("rect")) {
            QJsonArray rectA = jsonObj["rect"].toArray();
            
            std::vector<std::pair<cv::Rect, int>> rects;
            rects.reserve(rectA.size());
            
            std::vector<std::pair<std::string, cv::Mat>> sprites;
            sprites.reserve(rectA.size());
            
            for(int i = 0; i < rectA.size(); i++) {
                QJsonArray rect = rectA[i].toArray();
                //std::cout << rect[0].toInt() << "," << rect[1].toInt() << "," << rect[2].toInt() << "," << rect[3].toInt() << std::endl;
                cv::Rect r{rect[1].toInt(), rect[2].toInt(), rect[3].toInt(), rect[4].toInt()};
                rects.push_back(std::make_pair(r, rect[4].toInt()));
            
                sprites.push_back(std::make_pair(rect[0].toString().toStdString(), spritesheet(r).clone()));
            }
            
            scene->setRect(rects);
            scene->setSprites(sprites);
            scene->update();
            
            openEditor();
        }
        
        if(jsonObj.contains("anim")) {
            QJsonArray sprA = jsonObj["anim"].toArray();
            
            std::vector<std::pair<int, int>> anims;
            anims.reserve(sprA.size());
            
            for(int i = 0; i < sprA.size(); i++) {
                QJsonArray spr = sprA[i].toArray();
                anims.push_back({spr[0].toInt(), spr[1].toInt()});
            }
            
            animationScenes[0]->setTable(anims);
            playerScene->setFps(jsonObj["fps"].toDouble());
        }

    } else {
        qWarning() << "JSON document is not an object.";
        return;
    }
}

int MainWindow::loadImage() {
    QImageReader reader(filename);
    reader.setAutoTransform(true);

    QImage spritesheet = reader.read();

    if (spritesheet.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return 1;
    }

    if (spritesheet.colorSpace().isValid())
        spritesheet.convertToColorSpace(QColorSpace::SRgb);
    
    this->spritesheet = cv::imread(filename.toStdString(), cv::IMREAD_UNCHANGED);

    auto minHeight = std::min(spritesheet.height(), 1080);
    auto smallImage = spritesheet.scaledToHeight(minHeight);
    
    scene->setPixmap(QPixmap::fromImage(smallImage));
    scene->adjustSize();
    return 0;
}

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    
    QStringList fileNames;
    
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    
    QFileInfo fileInfo(fileNames[0]);
    
    std::cout << "ext : " << fileInfo.suffix().toStdString() << std::endl;
    
    if(fileInfo.suffix() == "json") {
        filenameProject = fileNames[0];
        loadJsonProject();
    } else {
        filename = fileNames[0];
        loadImage();
    }

    //filename = QFileDialog::getOpenFileName(this, tr("Open"), "/", tr("Files (*)"));
  
}

void MainWindow::sliceButtonAction() {
    scene->getRect().clear();
    scene->getSprites().clear();
    
    splitSpriteSheet(areaSlice->value());
    scene->update();
    
    if(selectorTab == nullptr) {
        selectorTab = new SpriteSelectorTab();
    }
    
    selectorTab->setSpritesheet(spritesheet);
    selectorTab->setRect(scene->getRect());
    selectorTab->setSprites(scene->getSprites());
    selectorTab->update();
}

void MainWindow::saveIndividualImages() {
    if (scene->getSprites().size() == 0) {
        std::cerr << "Error: there are no sprites." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: there are no sprites."));
        return;
    }
    
    if(baseFilename.empty()) {
        std::cerr << "Error: you should specify a base name for the sprites." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: you should specify a base name for the sprites."));
        return;
    }
    
    QString dir = QFileDialog::getExistingDirectory(this, "Save directory", "/");
    
    int spriteIndex = 0;
    
    for (const std::pair<std::string, cv::Mat>& spr : scene->getSprites()) {
        std::string filename2 = dir.toStdString() + "/" + baseFilename + std::to_string(spriteIndex++) + ".png";
        
        cv::Mat image4Channel(spr.second.rows, spr.second.cols, CV_8UC4);

        if(spr.second.type() == CV_8UC3) {
            for (int y = 0; y < spr.second.rows; y++) {
                for (int x = 0; x < spr.second.cols; x++) {
                    cv::Vec3b bgr = spr.second.at<cv::Vec3b>(y, x);
                    cv::Vec4b bgra(bgr[0], bgr[1], bgr[2], 255);
                    image4Channel.at<cv::Vec4b>(y, x) = bgra;
                }
            }
        } else {
            image4Channel = spr.second;
        }
        
        cv::imwrite(filename2, image4Channel);
    }
}

void MainWindow::splitSpriteSheet(int area) {
    if (filename.size() == 0) {
        std::cerr << "Error: The filename is empty." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: The filename is empty."));
        return;
    }
    
    spritesheet = cv::imread(filename.toStdString(), cv::IMREAD_UNCHANGED);
    
    if (spritesheet.empty()) {
        std::cerr << "Error: Could not load spritesheet image." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: Could not load spritesheet image."));
        return;
    }
    
    scene->setSpritesheet(spritesheet);
    
    //QString dir = QFileDialog::getExistingDirectory(this, "Save directory", "/");
    
    cv::Mat gray;
    cv::cvtColor(spritesheet, gray, cv::COLOR_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(gray, binary, 254, 255, cv::THRESH_BINARY);
    
    //cv::imwrite("/Users/vulcain/Sprited/res/spriteBinary.png", binary);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::pair<std::string, cv::Mat>> individualSprites;
    individualSprites.reserve(contours.size());
    
    //int spriteIndex = 0;
    
    std::vector<std::pair<cv::Rect, int>> rect;
    rect.reserve(contours.size());
    int i = 0;
    
    for (const std::vector<cv::Point>& contour : contours) {
        cv::Rect boundingBox = cv::boundingRect(contour);
        
        cv::Mat sprite = spritesheet(boundingBox).clone();
            
        if (boundingBox.area() > area && (boundingBox.width < binary.cols && boundingBox.height < binary.rows)) {
            rect.push_back(std::make_pair(boundingBox, false));
            individualSprites.push_back(std::make_pair(baseFilename + std::to_string(i), sprite));
            //std::string filename = dir.toStdString() + "sprite" + std::to_string(spriteIndex++) + ".png";
            //cv::imwrite(filename, sprite);
        }
        ++i;
    }
    
    scene->setRect(rect);
    scene->setSprites(individualSprites);
}

void MainWindow::saveJson(QJsonObject root, QString filenameJson) {
    QByteArray ba = QJsonDocument(root).toJson(QJsonDocument::Compact);

    QFile fout(filenameJson);
    fout.open(QIODevice::WriteOnly);
    fout.write(ba);
}

void MainWindow::exportJson() {
    if (scene->getRect().empty()) {
        std::cerr << "Error: Could not find the bounding boxes." << std::endl;
        QMessageBox::warning(nullptr, tr("Error"), tr("Error: Could not find the bounding boxes."));
        return;
    }
    
    QString file = QFileDialog::getSaveFileName(this, tr("Save json"), QDir::homePath());
    
    QJsonObject root;
    
    auto rectA = scene->getRect();
    auto spriteA = scene->getSprites();
    
    if(rectA.size() > 0) {
        QJsonArray rectG;
        
        for(int i = 0; i < rectA.size(); i++) {
            QJsonArray rect;
            rect.push_back(QString::fromStdString(spriteA[i].first));
            rect.push_back(rectA[i].first.x);
            rect.push_back(rectA[i].first.y);
            rect.push_back(rectA[i].first.width);
            rect.push_back(rectA[i].first.height);
            rect.push_back(rectA[i].second);
        
            rectG.push_back(rect);
        }
        
        root["rect"] = rectG;
    }
    
    saveJson(root, file);
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

