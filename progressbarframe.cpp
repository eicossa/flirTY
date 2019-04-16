#include "progressbarframe.h"

progressBarFrame::progressBarFrame(QWidget *parent)
       : QFrame(parent)
{
    pBar = new QProgressBar();
    pBar->setMinimum(0);
    pBar->setMaximum(100);
    pBar->setVisible(true);
    pBar->setEnabled(true);
    //pBar->set


    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(pBar);




    setLayout(topLayout);
}
