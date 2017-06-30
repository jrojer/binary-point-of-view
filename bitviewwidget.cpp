#include "bitviewwidget.h"

BitViewWidget::BitViewWidget(QWidget* parent):
												QWidget(parent),
												scale_(10),
												hor_val_(0),
												ver_val_(0),
												period_bits_(8)
{
}

size_t BitViewWidget::GetDataSize()
{
	return data_.size();
}
size_t BitViewWidget::GetScale()
{
	return scale_;
}
size_t BitViewWidget::GetPeriod()
{
	return period_bits_;
}

void BitViewWidget::ReadFile(QFile* file)
{
	data_.resize(file->size());
	QDataStream data_stream(file);
	data_stream.readRawData((char*)data_.data(), file->size());
	update();
}

int GetBit(const uint8_t* mass, size_t bitpos)
{
	return (mass[bitpos/CHAR_BIT] >> (bitpos%CHAR_BIT)) & 1;
}

void BitViewWidget::paintEvent(QPaintEvent* event)
{
	size_t area_height = height();
	size_t area_width = width();
	QPainter painter(this);

	if(data_.empty())
	{
		painter.fillRect(0,0,width(),height(),Qt::darkCyan);
	}
	else
	{
		size_t starting_row_id = ver_val_;
		size_t starting_col_id = hor_val_;

		size_t num_cols_available = area_width/scale_;
		size_t num_rows_available = area_height/scale_;

		size_t num_rows_total = (data_.size() * CHAR_BIT + (period_bits_ - 1))/period_bits_ ;
		size_t num_cols_total = (period_bits_ < data_.size()*CHAR_BIT) ? period_bits_ : data_.size()*CHAR_BIT;

		size_t num_rows = (num_rows_available < num_rows_total) ? num_rows_available : num_rows_total;
		size_t num_cols = (num_cols_available < num_cols_total) ? num_cols_available : num_cols_total;


		painter.scale(scale_,scale_);

		painter.fillRect(0,0,num_cols,num_rows,Qt::black);

		for (size_t i = 0; i < num_rows; ++i)
		{
			for (size_t j = 0; j < num_cols; ++j)
			{
				if ( GetBit( (const uint8_t*)data_.data(),starting_col_id + j + period_bits_*(starting_row_id + i)) == 1)
				{
					painter.fillRect(j,i,1,1,Qt::green);
				}
			}
		}
	}

}

void BitViewWidget::setHorizontalScrollBarValue(int value)
{
	hor_val_ = value;
	update();
}
void BitViewWidget::setVerticalScrollBarValue(int value)
{
	ver_val_ = value;
	update();
}
void BitViewWidget::setPeriod(int value)
{
	ver_val_ = 0;
	hor_val_ = 0;
	period_bits_ = value;
	update();
}
void BitViewWidget::setScale(int value)
{
	ver_val_ = 0;
	hor_val_ = 0;
	scale_ = value;
	update();
}
