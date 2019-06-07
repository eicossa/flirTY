#ifndef TGFSTABVIEW_H
#define TGFSTABVIEW_H

#include <QTabWidget>

class tgfsTabView : public QTabWidget
{
    Q_OBJECT

public:
    explicit tgfsTabView(QWidget *parent = 0);

    // Implementation of the QTabBar::tabAt method
    // This should be exposed here because the original one is a protected
    // method and in the TabbedWindow class we need to retrieve the tab's index
    // by the cursor position.
    int tabAt(const QPoint &pos);
};

#endif // TGFSTABVIEW_H
