#include "bitviewer.h"

BitViewer::BitViewer(): QMainWindow(), scale_(10)
{
	ui.setupUi(this);

	QObject::connect(ui.verticalScrollBar, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setVerticalScrollBarValue(int)));
	QObject::connect(ui.horizontalScrollBar, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setHorizontalScrollBarValue(int)));
	QObject::connect(ui.spinBox_period, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setPeriod(int)));
	QObject::connect(ui.spinBox_period, SIGNAL(valueChanged(int)), this, SLOT(SetScrollBars(int)));
}
void BitViewer::on_pushButton_open_file_clicked()
{
	QString filename =  QFileDialog::getOpenFileName(this);

	// TODO To window title

	if (filename.isEmpty())
	{
		return;
	}

	QFile file(filename);

	if ( file.open(QIODevice::ReadOnly))
	{
		ui.bitview_widget->ReadFile(&file);

		file.close();
	}
	SetScrollBars(0);
}


/*
int GetBit(const uint8_t* mass, size_t bitpos)
{
	return (mass[bitpos/CHAR_BIT] >> (bitpos%CHAR_BIT)) & 1;
}
void BitViewer::RenderBitView(int period_bits)
{
	size_t area_height = ui.label_image->height();
	size_t area_width = ui.label_image->width();
	QPixmap image(area_width, area_height);

	if(data_.empty())
	{
		image.fill(Qt::darkCyan);
	}
	else
	{
		size_t starting_row_id = ui.verticalScrollBar->value();
		size_t starting_col_id = ui.horizontalScrollBar->value();

		size_t num_cols_available = area_width/scale_;
		size_t num_rows_available = area_height/scale_;

		size_t num_rows_total = (data_.size() * CHAR_BIT + (period_bits - 1))/period_bits ;
		size_t num_cols_total = (period_bits < data_.size()*CHAR_BIT) ? period_bits : data_.size()*CHAR_BIT;

		size_t num_rows = (num_rows_available < num_rows_total) ? num_rows_available : num_rows_total;
		size_t num_cols = (num_cols_available < num_cols_total) ? num_cols_available : num_cols_total;

		QPainter painter(&image);
		painter.scale(scale_,scale_);
		for (size_t i = starting_row_id; i < starting_row_id+num_rows; ++i)
		{
			for (size_t j = starting_col_id; j < starting_col_id+num_cols; ++j)
			{
				if ( GetBit( (const uint8_t*)data_.data(),j + period_bits*i) == 1)
				{
					painter.fillRect(i,j,1,1,Qt::green);
				}
			}
		}
	}
	ui.label_image->setPixmap(image);
}
void BitViewer::paintEvent(QPaintEvent* event)
{
	//RenderBitView(ui.spinBox_period->value());
}
*/

void BitViewer::resizeEvent(QResizeEvent* event)
{
	SetScrollBars(0);
	//RenderBitView(ui.spinBox_period->value());
}

void BitViewer::SetScrollBars(int dummy)
{
	//


	// TODO


	// set old values or maximum


	//

	size_t data_size = ui.bitview_widget->GetDataSize();

	size_t period_bits = ui.spinBox_period->value();
	size_t area_height = ui.bitview_widget->height();
	size_t area_width = ui.bitview_widget->width();

	size_t num_cols_available = area_width/scale_;
	size_t num_rows_available = area_height/scale_;

	size_t num_rows_total = (data_size * CHAR_BIT + (period_bits - 1))/period_bits;
	size_t num_cols_total = (period_bits < data_size*CHAR_BIT) ? period_bits : data_size*CHAR_BIT;
	
	if (num_cols_available < num_cols_total)
	{
		ui.horizontalScrollBar->setSingleStep(1);
		ui.horizontalScrollBar->setMinimum(0);
		ui.horizontalScrollBar->setMaximum(num_cols_total - num_cols_available);
		ui.horizontalScrollBar->setEnabled(true);
	}
	else
	{
		ui.horizontalScrollBar->setValue(0);
		ui.horizontalScrollBar->setDisabled(true);
	}
	
	if (num_rows_available < num_rows_total)
	{
		ui.verticalScrollBar->setSingleStep(1);
		ui.verticalScrollBar->setMinimum(0);
		ui.verticalScrollBar->setMaximum(num_rows_total - num_rows_available);
		ui.verticalScrollBar->setEnabled(true);
	}
	else
	{
		ui.verticalScrollBar->setValue(0);
		ui.verticalScrollBar->setDisabled(true);
	}
}

void BitViewer::on_pushButton_plus_clicked()
{
	// TODO scale must respect current top left coordinates

	// TODO multiply
	if (scale_ + 1 <= 32)
	{
		++scale_;
		SetScrollBars(0);
		ui.bitview_widget->setScale(scale_);
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
		SetScrollBars(0);
		ui.bitview_widget->setScale(scale_);
		ui.label_zoom_value->setText(QString::number(scale_));
	}
}

/*
void BitViewer::ScrollBarsValueChanged(int dummy)
{
	RenderBitView(ui.spinBox_period->value());
}
*/
