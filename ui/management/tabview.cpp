#include "tabview.h"
#include "tabbar.h"

tabView::tabView(QWidget *parent) :
    QTabWidget(parent)
{
    // Set custom tabbar
    setTabBar(new tgfsTabBar);
}


int tabView::tabAt(const QPoint &pos)
{
    return tabBar()->tabAt(pos);
}

