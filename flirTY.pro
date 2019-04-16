#-------------------------------------------------
#
# Project created by QtCreator 2019-03-19T12:55:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tabbedFlirbabaGuiSegmentor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    tgfstabview.cpp \
    tgfstabbar.cpp \
    tgfstabbedwindow.cpp \
    tgfstabbedwindowprivate.cpp \
    tgfsghostwindow.cpp \
    segmentorwindow.cpp \
    segmentor.cpp \
    segmentorparams.cpp \
    segmentorframe.cpp \
    flirimg.cpp \
    flirimgmetadata.cpp \
    errmsgs.cpp \
    exec.cpp \
    flirbabawindow.cpp \
    flirimgframe.cpp \
    cmdstrings.cpp \
    tgfsmainwindow.cpp \
    progressbarframe.cpp \
    superslider.cpp \
    rangeslider.cpp \
    blob.cpp \
    imgoperatorwindows.cpp

HEADERS += \
    tgfstabview.h \
    tgfstabbar.h \
    tgfstabbedwindow.h \
    tgfstabbedwindowprivate.h \
    tgfsghostwindow.h \
    segmentorwindow.h \
    segmentor.h \
    segmentorparams.h \
    segmentorframe.h \
    flirimg.h \
    flirimgmetadata.h \
    utilfunc.h \
    errmsgs.h \
    exec.h \
    flirbabawindow.h \
    flirimgframe.h \
    cmdstrings.h \
    tgfsmainwindow.h \
    progressbarframe.h \
    superslider.h \
    rangeslider.h \
    blob.h \
    imgoperatorwindows.h

FORMS +=

INCLUDEPATH += C:\Users\eicossa\opencv4bild\install\include
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_core400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_dnn400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_features2d400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_flann400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_highgui400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_imgcodecs400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_imgproc400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_ml400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_objdetect400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_photo400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_shape400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_stitching400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_superres400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_video400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_videostab400.dll
LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_calib3d400.dll
PKGCONFIG += opencv
