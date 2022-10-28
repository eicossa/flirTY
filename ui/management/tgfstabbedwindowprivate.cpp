#include "tabbedwindow.h"
#include "tgfstabbedwindowprivate.h"
#include "tabview.h"

tgfsTabbedWindowPrivate::tgfsTabbedWindowPrivate(tabbedWindow* q_ptr)
{
    // Create tab widget
    tabs = new tabView();
    tabs->setDocumentMode(true);

    // Set up main window
    this->q_ptr = q_ptr;
    this->q_ptr->setCentralWidget(tabs);
}

QWidget* tgfsTabbedWindowPrivate::currentView()
{
    return tabs->currentWidget();
}

void tgfsTabbedWindowPrivate::setCurrentView(int index)
{
    tabs->setCurrentIndex(index);
}


int tgfsTabbedWindowPrivate::insertView(const QPoint &pos, QWidget *page,
                                    const QString &text)
{
    // Get tab's index at the given global postition
    int index = tabs->tabAt(tabs->mapFromGlobal(pos));

    // Insert new tab
    return tabs->insertTab(index, page, text);
}


int tgfsTabbedWindowPrivate::addView(QWidget *view, const QString &title)
{
    return tabs->addTab(view, title);
}


void tgfsTabbedWindowPrivate::removeView(int index)
{
    tabs->removeTab(index);
}
