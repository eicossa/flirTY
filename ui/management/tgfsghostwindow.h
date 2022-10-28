#ifndef TGFSGHOSTWINDOW_H
#define TGFSGHOSTWINDOW_H

#include <QWidget>
class tgfsTabBar;

/*
 * This widget is a static screenshot of the original tab view.
 *
 * It will be created during the Drag&Drop action as a visual feedback for the
 * user.
 *
 * When moving this widget on the screen use moveWithOffest() instead of move()
 * so the mouse cursor will maintain the current offset from the upper left
 * corner of the window as when the D&D operation has started.
 */
class tabberGhostWindow : public QWidget
{
    Q_OBJECT

public:
    // Constructor accepts the reference to the tab bar widget and the position
    // of the cursor local to the tab bar itself
    explicit tabberGhostWindow(tgfsTabBar *tabbar, const QPoint& pos);

    // Offset between the position of the cursor on the tab bar and the upper
    // left position of the tab bar's window.
    const QPoint offset() { return m_offset; }

    // Index of the original tab in the tab bar
    const int index() { return m_index; }

    // Move the widget into the given position taking in account the current
    // offset() value
    void moveWithOffset(const QPoint&);

    // Return True if the difference between the position of the original
    // widget and given point is greater than the
    // QApplication::startDragDistance() value
    bool dragStarted(const QPoint&);

private:
    QPoint m_offset;
    int m_index;
    QPoint m_originPos;
};

#endif // TGFSGHOSTWINDOW_H
