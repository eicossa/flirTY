#ifndef TGFSTABBEDWINDOWPRIVATE_H
#define TGFSTABBEDWINDOWPRIVATE_H

#include "tabbedwindow.h"

class tabView;

/*
 * Private implementation of the TabbedWindow class (PIMPL pattern)
 *
 * See tabbedwindow.h for more documentation
 */
class tgfsTabbedWindowPrivate : public QObject
{ 
    Q_OBJECT
public:
    // Constructor accepts the pointer of the public implementation
    tgfsTabbedWindowPrivate(tabbedWindow* q_ptr);

    int addView(QWidget* view, const QString &title);
    int insertView(const QPoint &pos, QWidget *page, const QString &text);
    void removeView(int index);
    void setCurrentView(int index);
    QWidget* currentView();

private:
    tabbedWindow *q_ptr;
    tabView* tabs;
};

#endif // TGFSTABBEDWINDOWPRIVATE_H
