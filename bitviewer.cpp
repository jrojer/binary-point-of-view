#include "bitviewer.h"

BitViewer::BitViewer(): QMainWindow(), scale_(10)
{
	ui.setupUi(this);

	QObject::connect(ui.verticalScrollBar, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setVerticalScrollBarValue(int)));
	QObject::connect(ui.horizontalScrollBar, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setHorizontalScrollBarValue(int)));
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
	// TODO scale must respect current top left coordinates

	// TODO multiply
	if (scale_ + 1 <= 32)
	{
		++scale_;
		ui.bitview_widget->SetGrainSize(scale_);
		ui.label_zoom_value->setText(QString::number(scale_));
	}

}
void BitViewer::on_pushButton_minus_clicked()
{
	// TODO scale must respect current top left coordinates

	// TODO multiply
	if (scale_ - 1 >= 1)
	{
		--scale_;
		ui.bitview_widget->SetGrainSize(scale_);
		ui.label_zoom_value->setText(QString::number(scale_));
	}
}
