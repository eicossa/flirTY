#ifndef TGFSTABBEDWINDOWPRIVATE_H
#define TGFSTABBEDWINDOWPRIVATE_H

#include "tgfstabbedwindow.h"

class tgfsTabView;

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
    tgfsTabbedWindowPrivate(tgfsTabbedWindow* q_ptr);

    int addView(QWidget* view, const QString &title);
    int insertView(const QPoint &pos, QWidget *page, const QString &text);
    void removeView(int index);
    void setCurrentView(int index);
    QWidget* currentView();

private:
    tgfsTabbedWindow *q_ptr;
    tgfsTabView* tabs;
};

#endif // TGFSTABBEDWINDOWPRIVATE_H
