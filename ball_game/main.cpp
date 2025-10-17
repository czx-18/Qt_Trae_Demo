#include "ballgame.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    // 先创建QApplication对象
    QApplication a(argc, argv);
    
    // 针对Qt 5的中文显示设置
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    
    // 设置应用程序默认字体 - 使用更安全的方法
    QFont font;
    font.setFamily(QStringLiteral("SimHei")); // 使用QStringLiteral避免编码问题
    font.setPointSize(9);
    a.setFont(font);
    
    BallGame w;
    w.show();
    
    return a.exec();
}
