/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/

#include "ProjectMainWindow.h"
#include <QTextCodec>

int main( int argc, char **argv ) 
{
	QApplication *app = new QApplication(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

	ProjectMainWindow *window = new ProjectMainWindow();
	window->show();
	return app->exec();
};

