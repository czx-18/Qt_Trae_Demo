# 贪吃蛇游戏模块配置文件
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 主程序入口文件
SOURCES += $$PWD/main.cpp \
    $$PWD/gameboard.cpp \
    $$PWD/snake.cpp \
    $$PWD/mainwindow.cpp

# 头文件
HEADERS += \
    $$PWD/gameboard.h \
    $$PWD/snake.h \
    $$PWD/mainwindow.h

# UI 文件
FORMS = \
    $$PWD/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target