#ifndef TFGSTABBEDWINDOW_H
#define TFGSTABBEDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "tgfsmainwindow.h"
#include "segmentorwindow.h"
#include "flirimgwindow.h"
#include "opencv_headers.h"

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
private slots:
    void openImagesFileDialog();
public slots:
    void loadDefaultImage();
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
    void                         setCurrentView(int index);

    // Returns the current view
    QWidget*                     currentView();

    // Setup the menubars
    void                         setupMenubars();
    // Process images
    //    void                         buildFlirImgObjAndSegmentorObj(QString);
    //    void                         addAnotherFlirImgAndSegmentorObj(QString);
    void                         makeEverythingHappenANewWay(QString);

    void                         buildObjects(QString);
    void                         loadUi();
    void                         addAnotherTab(QString);

private:
    tgfsTabbedWindowPrivate      *d_ptr;
    int                           numOfImagesDisplayed;
    QToolBar*                     mainMenuToolbar;
    QMenu*                        fileMenu;
    QMenu*                        editMenu;
    QMenu*                        helpMenu;
};

#endif // TFGSMAINWINDOW_H
