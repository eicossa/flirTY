#include "tgfstabbar.h"
#include "tgfstabview.h"
#include "tgfsghostwindow.h"
#include "tgfstabbedwindow.h"

#include <QMouseEvent>
#include <QApplication>


tgfsTabBar::tgfsTabBar(QWidget *parent) :
    QTabBar(parent),
    m_ghost(NULL)
{
}


tgfsTabBar::~tgfsTabBar()
{
    delete m_ghost;
}


void tgfsTabBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_ghost) {
        m_ghost->moveWithOffset(event->globalPos());
    }
}


void tgfsTabBar::mousePressEvent(QMouseEvent *event)
{
    // If left button is pressed start tab move event
    if (event->button() == Qt::LeftButton && tabAt(event->pos()) > -1) {
        m_ghost = new tgfsGhostWindow(this, event->pos());
        m_ghost->show();
    }

    // Call superclass
    QTabBar::mousePressEvent(event);
}


void tgfsTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    // Call superclass if a button different than the left one was released
    // and return
    if (event->button() != Qt::LeftButton) {
        QTabBar::mouseReleaseEvent(event);
        return;
    }

    // Execute drag code only if far enough
    if (m_ghost->dragStarted(event->globalPos())) {
        tgfsTabBar *w = dynamic_cast<tgfsTabBar*>(
                    QApplication::widgetAt(event->globalPos()));

        // Choose action by the widget under the mouse's coordinates
        if (w == NULL) {
            if (count() == 1) {
                // Move the current window into the new position
                window()->move(m_ghost->pos());
            } else {
                // Creates a new window with the dragged tab
                createNewWindow(m_ghost);
            }
        } else {
            // Move the dragged tab into the window under the cursor
            tgfsTabbedWindow *wnd = dynamic_cast<tgfsTabbedWindow*>(w->window());

            if (wnd != NULL) {
                moveToWindow(wnd, event->globalPos(), m_ghost);
            }
        }
    }

    // Close ghost
    m_ghost->close();
}


void tgfsTabBar::moveToWindow(tgfsTabbedWindow *wnd,
                              const QPoint &pos,
                              tgfsGhostWindow *ghost)
{
    // Remove view from this window
    tgfsTabView *view = static_cast<tgfsTabView*>(parent());
    int index = ghost->index();
    QString text = tabText(index);
    QWidget *page = view->widget(index);

    view->removeTab(index);

    // Insert tab into the new window at the given cursor's position
    index = wnd->insertView(pos, page, text);

    // Set it as the current tab and move focus to the new window
    wnd->setCurrentView(index);
    wnd->raise();
}


void tgfsTabBar::tabRemoved(int index)
{
    if (count() == 0) {
        window()->close();
    }
}


void tgfsTabBar::createNewWindow(tgfsGhostWindow *ghost)
{
    // Create the new window with the same size and centered under the cursor
    tgfsTabbedWindow *wnd = new tgfsTabbedWindow();

    wnd->setGeometry(ghost->geometry());

    // Move widget to the new window
    tgfsTabView *view = static_cast<tgfsTabView*>(parent());
    int index = ghost->index();
    QWidget *tab = view->widget(index);
    QString text = view->tabText(index);

    view->removeTab(index);
    wnd->addView(tab, text);

    // Show the new window
    wnd->show();
}
