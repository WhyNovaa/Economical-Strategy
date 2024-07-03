QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    credit_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    pass_check.cpp \
    password_menu.cpp \
    player.cpp \
    bank.cpp \
    sha1.cpp

HEADERS += \
    credit_dialog.h \
    factory.h \
    mainwindow.h \
    pass_check.h \
    password_menu.h \
    player.h \
    bank.h \
    sha1.h

FORMS += \
    mainwindow.ui \
    pass_check.ui \
    password_menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    style.qrc
