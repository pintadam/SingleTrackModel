#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T19:26:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SingleTrackModel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    mysignal.cpp \
    MDF/MDFBaseData.cpp \
    MDF/MDFCC.cpp \
    MDF/MDFCCLinear.cpp \
    MDF/MDFCCTextTable.cpp \
    MDF/MDFCG.cpp \
    MDF/MDFChannel.cpp \
    MDF/MDFConversion.cpp \
    MDF/MDFDG.cpp \
    MDF/MDFHeader.cpp \
    MDF/MDFID.cpp \
    MDF/MDFLib.cpp \
    MDF/MDFPR.cpp \
    MDF/MDFTX.cpp

HEADERS  += mainwindow.h \
    qcustomplot/qcustomplot.h \
    qcustomplot.h \
    mysignal.h \
    MDF/MDFBaseData.h \
    MDF/MDFCC.h \
    MDF/MDFCCLinear.h \
    MDF/MDFCCTextTable.h \
    MDF/MDFCG.h \
    MDF/MDFChannel.h \
    MDF/MDFConversion.h \
    MDF/MDFData.h \
    MDF/MDFDG.h \
    MDF/MDFHeader.h \
    MDF/MDFID.h \
    MDF/MDFLib.h \
    MDF/MDFPR.h \
    MDF/MDFTX.h

FORMS    += mainwindow.ui
