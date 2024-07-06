QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bet_dialog.cpp \
    credit_dialog.cpp \
    fabric_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    month_end_dialog.cpp \
    pass_check.cpp \
    password_menu.cpp \
    player.cpp \
    bank.cpp \
    product_dialog.cpp \
    sha256.cpp

HEADERS += \
    bet_dialog.h \
    credit_dialog.h \
    fabric_dialog.h \
    factory.h \
    mainwindow.h \
    month_end_dialog.h \
    pass_check.h \
    password_menu.h \
    player.h \
    bank.h \
    product_dialog.h \
    sha256.h

FORMS += \
    mainwindow.ui \
    pass_check.ui \
    password_menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/bg.jpg

RESOURCES += \
    style.qrc
