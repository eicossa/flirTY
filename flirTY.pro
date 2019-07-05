#-------------------------------------------------
#
# Project created by QtCreator 2019-03-19T12:55:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flirTY
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
        segmentor.cpp \
        segmentorparams.cpp \
        flirimg.cpp \
        errmsgs.cpp \
        exec.cpp \
        blob.cpp \
        imgoperatorwindows.cpp \
        ui/flirimgframe.cpp \
        ui/flirimgwindow.cpp \
        ui/tgfstabview.cpp \
        ui/tgfstabbar.cpp \
        ui/tgfstabbedwindow.cpp \
        ui/tgfstabbedwindowprivate.cpp \
        ui/tabberghostwindow.cpp \
        ui/segmentorframe.cpp \
        ui/tgfsmainwindow.cpp \
        ui/progressbarframe.cpp \
        ui/superslider.cpp \
        ui/rangeslider.cpp \
    flirimg_extract.cpp \
    flirimg_calc.cpp \
    flirimg_exec.cpp \
    flirimg_io.cpp \
    ui/flirimgframe_ui.cpp \
    ui/segmentor_ui.cpp

HEADERS += \
    segmentor.h \
    segmentorparams.h \
    flirimg.h \
    utilfunc.h \
    errmsgs.h \
    exec.h \
    blob.h \
    imgoperatorwindows.h \
    ui/flirimgwindow.h \
    ui/tgfstabview.h \
    ui/tgfstabbar.h \
    ui/tgfstabbedwindow.h \
    ui/tgfstabbedwindowprivate.h \
    ui/tgfsghostwindow.h \
    ui/segmentorwindow.h \
    ui/tgfsmainwindow.h \
    ui/progressbarframe.h \
    ui/superslider.h \
    ui/rangeslider.h \
    ui/segmentorframe.h \
    ui/flirimgframe.h \

FORMS +=

win32: {
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
}

unix: !macx{
  #INCLUDEPATH += "/usr/local/include"
  CONFIG      += link_pkgconfig
  PKGCONFIG   += opencv4
}

unix: macx{
  INCLUDEPATH += "/usr/local/include"

  QT_CONFIG -= no-pkg-config
  CONFIG += link_pkgconfig
  PKGCONFIG += opencv4
}
