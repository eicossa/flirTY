#ifndef TGFSMAINWINDOW_H
#define TGFSMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QFrame>

class tgfsMainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void signalProgress(int);
public slots:

public:
    tgfsMainWindow(QMainWindow *parent = 0);
    void setupImgReader(QAction*);

    void refreshProgressBars();
private:
    QString fileName;
    QFrame  *pBarFrame;

};

#endif // TGFSMAINWINDOW_H
