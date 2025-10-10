# Qt贪吃蛇游戏

这是一个使用Qt框架开发的经典贪吃蛇游戏。通过将项目模块化处理为.pri文件，可以方便地集成到其他Qt项目中。

## 功能特点

- 完整的贪吃蛇游戏逻辑
- 键盘控制（方向键移动，空格开始/暂停）
- 分数计算和显示
- 游戏状态提示（开始、暂停、游戏结束）
- 自适应窗口大小
- 中文界面支持

## 项目结构

项目使用模块化设计，将贪吃蛇游戏相关代码封装为snake_game.pri，可以方便地在其他Qt项目中引用：
├── gameboard.cpp       // 游戏主界面实现
├── gameboard.h         // 游戏主界面对象定义
├── main.cpp            // 程序入口
├── mainwindow.cpp      // 主窗口实现
├── mainwindow.h        // 主窗口定义
├── mainwindow.ui       // 主窗口UI设计
├── snake.cpp           // 贪吃蛇逻辑实现
├── snake.h             // 贪吃蛇对象定义
├── snake_game.pri      // 贪吃蛇游戏模块定义
└── untitled1.pro       // 项目主配置文件

## 开发环境

- Qt 5.15.2
- MSVC 2019 64-bit
- Windows 操作系统

## License

MIT License

## 编译运行

1. 确保已安装Qt 5.15.2或更高版本
2. 使用Qt Creator打开untitled1.pro文件
3. 构建并运行项目

## 集成到其他项目

如需将贪吃蛇游戏集成到其他Qt项目中，只需在目标项目的.pro文件中添加以下行：
include(snake_game.pri)
