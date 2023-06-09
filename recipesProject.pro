QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CalendarDialog.cpp \
    EventDialog.cpp \
    ListViewDelegate.cpp \
    LoginDialog.cpp \
    Recipe.cpp \
    RecipeDialog.cpp \
    RecipeInfoDialog.cpp \
    RecipeListModel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CalendarDialog.h \
    EventDialog.h \
    ListViewDelegate.h \
    LoginDialog.h \
    Recipe.h \
    RecipeDialog.h \
    RecipeInfoDialog.h \
    RecipeListModel.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
