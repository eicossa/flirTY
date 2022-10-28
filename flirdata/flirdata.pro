QT += gui
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

#unix: !mac {
#    INCLUDEPATH += /usr/local/include/opencv4
#    LIBS += -L/usr/local/lib/ -lopencv_core -l opencv_imgproc
#}

unix: !mac {
    CONFIG      += link_pkgconfig
    PKGCONFIG   += opencv4
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        errmsgs.cpp \
        flirmat.cpp \
        flirtags.cpp \
        flirtemp.cpp \
        flirtemp2.cpp \
        input_parser.cpp \
        main.cpp \
        paths.cpp \
        utilfuncs.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    enumtypes.h \
    errmsgs.h \
    flirmat.h \
    flirtags.h \
    flirtemp.h \
    flirtemp2.h \
    input_parser.h \
    paths.h \
    tclap/Arg.h \
    tclap/ArgContainer.h \
    tclap/ArgException.h \
    tclap/ArgGroup.h \
    tclap/ArgTraits.h \
    tclap/CmdLine.h \
    tclap/CmdLineInterface.h \
    tclap/CmdLineOutput.h \
    tclap/Constraint.h \
    tclap/DeferDelete.h \
    tclap/DocBookOutput.h \
    tclap/HelpVisitor.h \
    tclap/IgnoreRestVisitor.h \
    tclap/MultiArg.h \
    tclap/MultiSwitchArg.h \
    tclap/OptionalUnlabeledTracker.h \
    tclap/StandardTraits.h \
    tclap/StdOutput.h \
    tclap/SwitchArg.h \
    tclap/UnlabeledMultiArg.h \
    tclap/UnlabeledValueArg.h \
    tclap/ValueArg.h \
    tclap/ValuesConstraint.h \
    tclap/VersionVisitor.h \
    tclap/Visitor.h \
    tclap/sstream.h \
    utilfuncs.h

DISTFILES += \
    tclap/CMakeLists.txt

DEFINES += TEST_INPUT_DIR=\\\"/home/eicossa/Data/nabtd/new\\\"
DEFINES += TEST_OUTPUT_DIR=\\\"/home/eicossa/Data/nabtd/processed\\\"

DEFINES += IP_IMG=\\\"/home/eicossa/Data/nabtd/old/FLIR1097/FLIR1097.jpg\\\"
# DEFINES += OP_IMG=\\\"$$_PRO_FILE_PWD_/output.png\\\"
