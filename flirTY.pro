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


HEADERS += \
    flirdata/enumtypes.h \
    flirdata/errmsgs2.h \
    flirdata/flirmat.h \
    flirdata/flirtags.h \
    flirdata/flirtemp2.h \
    flirdata/paths.h \
    flirdata/utilfuncs.h \
    flirimg/environment.h \
    flirimg/errmsgs.h \
    flirimg/exec.h \
    flirimg/utilfunc.h \
    flirimg/flirimg.h \
    imgproc/blob.h \
    imgproc/opencv_headers.h \
    imgproc/segmentor.h \
    imgproc/segmentorparams.h \
    ml/leafdetector.h \
    ui/elems/rangeslider.h \
    ui/elems/superslider.h \
    ui/frames/flirimgframe.h \
    ui/frames/flirimgwindow.h \
    ui/frames/progressbarframe.h \
    ui/frames/segmentorframe.h \
    ui/frames/segmentorwindow.h \
    ui/management/tabbar.h \
    ui/management/tabbedwindow.h \
    ui/management/tabberghostwindow.h \
    ui/management/tabview.h \
    ui/management/tgfsghostwindow.h \
    ui/management/tgfstabbedwindowprivate.h \



SOURCES += \
        flirdata/errmsgs2.cpp \
        flirdata/flirmat.cpp \
        flirdata/flirtags.cpp \
        flirdata/flirtemp2.cpp \
        flirdata/paths.cpp \
        flirdata/utilfuncs.cpp \
        flirimg/environment.cpp \
        flirimg/errmsgs.cpp \
        flirimg/exec.cpp \
        flirimg/flirimg.cpp \
        flirimg/flirimg_calc.cpp \
        flirimg/flirimg_exec.cpp \
        flirimg/flirimg_extract.cpp \
        flirimg/flirimg_io.cpp \
        imgproc/blob.cpp \
        imgproc/segmentor.cpp \
        imgproc/segmentorparams.cpp \
        ui/elems/rangeslider.cpp \
        ui/elems/superslider.cpp \
        ui/frames/flirimgframe.cpp \
        ui/frames/flirimgframe_ui.cpp \
        ui/frames/flirimgwindow.cpp \
        ui/frames/segmentor_ui.cpp \
        ui/frames/segmentorframe.cpp \
        ui/management/tabbar.cpp \
        ui/management/tabbedwindow.cpp \
        ui/management/tabberghostwindow.cpp \
        ui/management/tabview.cpp \
        ui/management/tgfstabbedwindowprivate.cpp \
        main.cpp \



FORMS +=


#INCLUDEPATH += C:\Users\eicossa\opencv4bild\install\include
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_core400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_dnn400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_features2d400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_flann400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_highgui400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_imgcodecs400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_imgproc400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_ml400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_objdetect400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_photo400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_shape400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_stitching400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_superres400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_video400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_videostab400.dll
#LIBS += C:\Users\eicossa\opencv4bild\bin\libopencv_calib3d400.dll
#PKGCONFIG += opencv


#PKGCONFIG += opencv4

win32: {
  INCLUDEPATH += C:\Users\eicossa\Downloads\opencv410bild\install\include
  #LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_world410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_core410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_dnn410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_features2d410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_flann410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_highgui410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_imgcodecs410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_imgproc410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_ml410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_objdetect410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_photo410.dll
  #LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_shape410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_stitching410.dll
  #LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_superres410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_video410.dll
  #LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_videostab410.dll
  LIBS        += C:\Users\eicossa\Downloads\opencv410bild\bin\libopencv_calib3d410.dll
  PKGCONFIG   += opencv
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

SUBDIRS += \
    flirdata/flirdata.pro

DISTFILES += \
    flirdata/flirdata.pro.user
