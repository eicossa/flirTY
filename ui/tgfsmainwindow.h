#ifndef TGFSMAINWINDOW_H
#define TGFSMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QFrame>
#include <QProgressBar>

class tgfsMainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void signalProgress(int);
public slots:

public:
    tgfsMainWindow(QMainWindow *parent = 0);
    void setupImgReader(QAction*);
    QProgressBar* getMetadataProgressBar() { return pBarMetadata; }
    QProgressBar* getTempdataProgressBar() { return pBarTempdata; }
    QProgressBar* getSegmentrProgressBar() { return pBarSegmentr; }
    void refreshProgressBars();
private:
    QString fileName;
    QFrame  *pBarFrame;
    QProgressBar *pBarMetadata;
    QProgressBar *pBarTempdata;
    QProgressBar *pBarSegmentr;
};

#endif // TGFSMAINWINDOW_H
