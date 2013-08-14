#include"main_window.h"
#include<QApplication>
#include<QTranslator>
#include <QTextCodec>
int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
//         QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

//	QTranslator translator;
//  translator.load(":/mine_sweeper_zh.qm");
//  app.installTranslator(&translator);
	MainWindow window;
	window.show();
	return app.exec();
}
