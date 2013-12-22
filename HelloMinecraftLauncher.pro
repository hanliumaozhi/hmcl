#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T12:16:39
#
#-------------------------------------------------

QT       += core gui
QT       += script
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloMinecraftLauncher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    minecraftloader.cpp \
    json/json_writer.cpp \
    json/json_valueiterator.inl \
    json/json_value.cpp \
    json/json_reader.cpp \
    json/json_internalmap.inl \
    json/json_internalarray.inl \
    settingsmanager.cpp \
    systemmanager.cpp \
    minecraftoldversionincluder.cpp \
    launcherprofileloader.cpp \
    versionmanagerdialog.cpp \
    login.cpp \
    minecraftversionsloader.cpp \
    httphelper.cpp

HEADERS  += mainwindow.h \
    minecraftloader.h \
    json/writer.h \
    json/value.h \
    json/reader.h \
    json/json_batchallocator.h \
    json/json.h \
    json/forwards.h \
    json/features.h \
    json/config.h \
    json/autolink.h \
    FileUtilities.h \
    settingsmanager.h \
    systemmanager.h \
    minecraftoldversionincluder.h \
    zlib-1.2.8/zlib.h \
    launcherprofileloader.h \
    versionmanagerdialog.h \
    login.h \
    minecraftversionsloader.h \
    httphelper.h

FORMS    += mainwindow.ui \
    versionmanagerdialog.ui
