#ifndef TFGSTABBEDWINDOW_H
#define TFGSTABBEDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "tgfsmainwindow.h"
#include "segmentorwindow.h"
#include "flirbabawindow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
using namespace cv;


class tgfsTabbedWindowPrivate;
/*
 * Subclass of QMainWindow, contains a tab bar to manage a per-window list of
 * tabbed views and allows to add, insert or remove a view.
 *
 * Views can be standard QWidget instances or even instances of QMainWindow
 * with menu bar and tool bars as well.
 *
 * Thes features will be displayed automatically when the view's tab will be
 * activated and hidden when it'll be deactivated.
 */

class tgfsTabbedWindow : public QMainWindow
{    
    Q_OBJECT
public slots:
    void openImagesFileDialog();
    void loadDefaultImage();
    void load5Images();
    void displayOverlaidImageForOneImage(bool);
    void displayOverlaidImageFor5ImagesAtATime(bool);
    void aSignalHasBeenReceived(int);
    void isPointKeBlobKaTempNikaaloMadarchod(QPoint p);
signals:
    void signalProgress(int);
    void blobAvgTempLeleBhosdike(double);
public:
    explicit tgfsTabbedWindow(QWidget *parent = 0);
    ~tgfsTabbedWindow();

    // Add a new view
    int addView(QWidget* view, const QString &title);

    // Insert a view at the given global position
    // Returns -1 if the insert view operations fails otherwise returns the
    // view's index
    int insertView(const QPoint &pos, QWidget *page, const QString &text);

    // Remove the view at the given index
    void removeView(int index);

    // Set the view at the given index as the current view
    void setCurrentView(int index);

    // Returns the current view
    QWidget* currentView();

    // Setup the menubars
    void setupMenubars();
    // Process images
    void buildFlirImgObjAndSegmentorObj(QString);
    void addAnotherFlirImgAndSegmentorObj(QString);
    void makeEverythingHappenWithOneImageAtATime(QString);
    void makeEverythingHappenANewWay(QString);


    void connectEverythingForOneImage();
    void connectEverythingFor5Images();

    void makeEverythingHappenWith5ImagesAtATime(QString);

    flirImg* getFlirImg() { return this->fimg; }
    Segmentor* getSegmentor() { return this->segmentor; }

private:
    tgfsTabbedWindowPrivate  *d_ptr;
    tgfsMainWindow           *menubarWindow;
    flirbabaWindow           *fW;
    segmentorWindow          *sW;
    QProgressBar             *pBar;
    flirImg                  *fimg;
    Segmentor                *segmentor;
    cv::Mat                  originalMat;
    int                      numOfImagesDisplayed;

    std::vector<flirbabaWindow*>  fWindows;
    std::vector<segmentorWindow*> sWindows;
    std::vector<flirImg*>         fImgs;
    std::vector<Segmentor*>       segmentors;
};

#endif // TFGSMAINWINDOW_H
