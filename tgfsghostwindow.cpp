#include "tgfsghostwindow.h"
#include "tgfstabbar.h"
#include <QApplication>

tgfsGhostWindow::tgfsGhostWindow(tgfsTabBar *tabbar, const QPoint& pos) :
    QWidget()
{
    QWidget *wnd = tabbar->window();
    QPalette palette;

#if QT_VERSION >= 0x050000
    palette.setBrush(this->backgroundRole(), QBrush(wnd->grab()));
#else
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap::grabWidget(wnd)));
#endif

    this->setPalette(palette);
    this->setGeometry(wnd->geometry());
    this->setWindowOpacity(0.5);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    m_offset = tabbar->mapToGlobal(pos) - wnd->pos();
    m_index = tabbar->tabAt(pos);
    m_originPos = tabbar->mapToGlobal(pos);
}


void tgfsGhostWindow::moveWithOffset(const QPoint& pos)
{
    QWidget::move(pos - m_offset);
}


bool tgfsGhostWindow::dragStarted(const QPoint &pos)
{
    return (pos - m_originPos).manhattanLength()
            >= QApplication::startDragDistance();
}
