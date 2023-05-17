QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosecasedialog.cpp \
    choosecpudialog.cpp \
    choosegpudialog.cpp \
    choosemotherboarddialog.cpp \
    choosenetworkadapterdialog.cpp \
    choosepowersupplydialog.cpp \
    chooseramdialog.cpp \
    choosesoundcarddialog.cpp \
    choosestoragedialog.cpp \
    dialog.cpp \
    item.cpp \
    main.cpp \
    mainwindow.cpp \
    motherboardwidget.cpp

HEADERS += \
    choosecasedialog.h \
    choosecpudialog.h \
    choosegpudialog.h \
    choosemotherboarddialog.h \
    choosenetworkadapterdialog.h \
    choosepowersupplydialog.h \
    chooseramdialog.h \
    choosesoundcarddialog.h \
    choosestoragedialog.h \
    dialog.h \
    item.h \
    mainwindow.h \
    motherboardwidget.h

FORMS += \
    choosecasedialog.ui \
    choosecpudialog.ui \
    choosegpudialog.ui \
    choosemotherboarddialog.ui \
    choosenetworkadapterdialog.ui \
    choosepowersupplydialog.ui \
    chooseramdialog.ui \
    choosesoundcarddialog.ui \
    choosestoragedialog.ui \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
