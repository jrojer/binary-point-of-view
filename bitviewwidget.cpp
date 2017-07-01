#include "bitviewwidget.h"

BitViewWidget::BitViewWidget(QWidget* parent):
												QWidget(parent),
												grain_size_pixels_(10),
												column_offset_(0),
												row_offset_(0),
												period_bits_(8)
{
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
	num_cols_on_widget_ = width()/grain_size_pixels_;
	num_rows_on_widget_ = height()/grain_size_pixels_;

	num_rows_in_data_ = (data_.size() * CHAR_BIT + (period_bits_ - 1))/period_bits_ ;
	num_cols_in_data_ = std::min(period_bits_ , data_.size()*CHAR_BIT);

	SetScrollBars();

	column_offset_ = hor_scrollbar_->value();
	row_offset_ = ver_scrollbar_->value();

	QPainter painter(this);

	if(data_.empty())
	{
		painter.fillRect(0,0,width(),height(),Qt::darkCyan);
	}
	else
	{
		size_t num_rows = std::min(num_rows_on_widget_ , num_rows_in_data_);
		size_t num_cols = std::min(num_cols_on_widget_ , num_cols_in_data_);

		painter.scale(grain_size_pixels_,grain_size_pixels_);

		painter.fillRect(0,0,num_cols,num_rows-1,Qt::black);

		for (size_t i = 0; i < num_rows-1; ++i) // all rows except the last one
		{
			for (size_t j = 0; j < num_cols; ++j)
			{
				if ( GetBit( (const uint8_t*)data_.data(),column_offset_ + j + period_bits_*(row_offset_ + i)) == 1)
				{
					painter.fillRect(j,i,1,1,Qt::green);
				}
			}
		}

		// taking care of the last row exclusively
		size_t i = num_rows-1;
		for (size_t j = 0; j < num_cols; ++j)
		{
			size_t bitpos = column_offset_ + j + period_bits_*(row_offset_ + i);
			if (bitpos >= data_.size()*CHAR_BIT)
			{
				return;
			}
			if ( GetBit( (const uint8_t*)data_.data(),bitpos) == 1)
			{
				// one
				painter.fillRect(j,i,1,1,Qt::green);
			}
			else
			{
				// zero
				painter.fillRect(j,i,1,1,Qt::black);
			}
		}
	}

}

void BitViewWidget::setHorizontalScrollBarValue(int value)
{
	//column_offset_ = value;
	update();
}
void BitViewWidget::setVerticalScrollBarValue(int value)
{
	//row_offset_ = value;
	update();
}
void BitViewWidget::setPeriod(int value)
{
	//row_offset_ = 0;
	//column_offset_ = 0;
	hor_scrollbar_->setValue(0);
	ver_scrollbar_->setValue(0);
	period_bits_ = value;
	update();
}
void BitViewWidget::SetGrainSize(int value)
{
	if (value != 0)
	{
		row_offset_ = 0;
		column_offset_ = 0;
		grain_size_pixels_ = value;
		update();
	}
}

void BitViewWidget::CaptureScrollBars(QScrollBar* vertical, QScrollBar* horizontal)
{
	if ( vertical != nullptr && horizontal != nullptr)
	{
		ver_scrollbar_ = vertical;
		hor_scrollbar_ = horizontal;
	}
}

void BitViewWidget::SetScrollBars()
{
	if (hor_scrollbar_ == nullptr || ver_scrollbar_ == nullptr)
	{
		return;
	}
	if (num_cols_on_widget_ < num_cols_in_data_)
	{
		hor_scrollbar_->setSingleStep(1);
		hor_scrollbar_->setMinimum(0);
		hor_scrollbar_->setMaximum(num_cols_in_data_ - num_cols_on_widget_);
		hor_scrollbar_->setEnabled(true);
	}
	else
	{
		hor_scrollbar_->setValue(0);
		hor_scrollbar_->setMinimum(0);
		hor_scrollbar_->setMaximum(0);
		hor_scrollbar_->setDisabled(true);
	}

	if (num_rows_on_widget_< num_rows_in_data_)
	{
		ver_scrollbar_->setSingleStep(1);
		ver_scrollbar_->setMinimum(0);
		ver_scrollbar_->setMaximum(num_rows_in_data_ - num_rows_on_widget_);
		ver_scrollbar_->setEnabled(true);
	}
	else
	{
		ver_scrollbar_->setValue(0);
		ver_scrollbar_->setMinimum(0);
		ver_scrollbar_->setMaximum(0);
		ver_scrollbar_->setDisabled(true);
	}
}
