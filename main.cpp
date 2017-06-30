#include "QtWidgets/QApplication"
#include "QtWidgets/QWidget"
#include "bitviewer.h"

int main(int argc,char ** argv)
{
	QApplication app(argc, argv);

	BitViewer w;
	w.show();
	//QString filename =  QFileDialog::getOpenFileName();

	return app.exec();
}
