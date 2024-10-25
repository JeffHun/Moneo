QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analysispage.cpp \
    budgetpage.cpp \
    buttonutility.cpp \
    filedropzone.cpp \
    main.cpp \
    mainwindow.cpp \
    settingspage.cpp \
    transaction.cpp \
    transactiondelegate.cpp \
    transactionmodel.cpp \
    transactionspage.cpp \
    welcomepage.cpp

HEADERS += \
    analysispage.h \
    budgetpage.h \
    buttonutility.h \
    filedropzone.h \
    mainwindow.h \
    settingspage.h \
    transaction.h \
    transactiondelegate.h \
    transactionmodel.h \
    transactionspage.h \
    welcomepage.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
