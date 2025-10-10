# 贪吃蛇游戏模块配置文件

# 源文件
SNAKE_SOURCES = \
    $$PWD/gameboard.cpp \
    $$PWD/snake.cpp \
    $$PWD/mainwindow.cpp

# 头文件
SNAKE_HEADERS = \
    $$PWD/gameboard.h \
    $$PWD/snake.h \
    $$PWD/mainwindow.h

# UI 文件
SNAKE_FORMS = \
    $$PWD/mainwindow.ui

# 将文件添加到全局变量中
SOURCES += $${SNAKE_SOURCES}
HEADERS += $${SNAKE_HEADERS}
FORMS += $${SNAKE_FORMS}