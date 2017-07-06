#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(":/font/Noto/NotoSans-Regular.ttf");
    if(fontId >= 0 && !QFontDatabase::applicationFontFamilies(fontId).isEmpty()){
        QFont font(QFontDatabase::applicationFontFamilies(fontId).first());
        font.setStyleStrategy(QFont::PreferAntialias);
//        font.setPointSize(fontSize);
        qApp->setFont(font);
        QApplication::setFont(font);
        qDebug() << qApp->font().family();

        qDebug() << "font " << qApp->font().family() << qApp->font().pointSize();

    }
    QFont font4log = qApp->font();

    fontId = QFontDatabase::addApplicationFont(":/font/UbuntuMono-R.ttf");
    if(fontId >= 0 && !QFontDatabase::applicationFontFamilies(fontId).isEmpty()){
        QFont font(QFontDatabase::applicationFontFamilies(fontId).first());
        font.setStyleStrategy(QFont::PreferAntialias);
        font4log = font;
    }


    MainWindow w(font4log);
    w.show();

    return a.exec();
}
