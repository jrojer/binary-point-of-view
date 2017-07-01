#include "bitviewer.h"

BitViewer::BitViewer(): QMainWindow(), scale_(1)
{
	ui.setupUi(this);

	QObject::connect(ui.spinBox_period, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setPeriod(int)));

	ui.bitview_widget->CaptureScrollBars(ui.verticalScrollBar, ui.horizontalScrollBar);
}

void BitViewer::on_pushButton_open_file_clicked()
{
	QString filename =  QFileDialog::getOpenFileName(this);

	if (filename.isEmpty())
	{
		return;
	}

	this->setWindowTitle("Bit Viewer - " + filename);

	QFile file(filename);

	if ( file.open(QIODevice::ReadOnly))
	{
		ui.bitview_widget->ReadFile(&file);

		file.close();
	}
}

void BitViewer::on_pushButton_plus_clicked()
{
	if (scale_ * 1.25 <= 4)
	{
		scale_ *= 1.25;
		ui.bitview_widget->SetGrainSize((int)(scale_ * 10));
		ui.label_zoom_value->setText(QString::number(scale_));
	}

}
void BitViewer::on_pushButton_minus_clicked()
{
	if (scale_ * 0.8 >= 0.1)
	{
		scale_ *= 0.8;
		ui.bitview_widget->SetGrainSize((int)(scale_*10));
		ui.label_zoom_value->setText(QString::number(scale_));
	}
}
