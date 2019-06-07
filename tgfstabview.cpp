#include "tgfstabview.h"
#include "tgfstabbar.h"

tgfsTabView::tgfsTabView(QWidget *parent) :
    QTabWidget(parent)
{
    // Set custom tabbar
    setTabBar(new tgfsTabBar);
}


int tgfsTabView::tabAt(const QPoint &pos)
{
    return tabBar()->tabAt(pos);
}

