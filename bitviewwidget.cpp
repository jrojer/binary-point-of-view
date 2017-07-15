#include "bitviewwidget.h"

BitViewWidget::BitViewWidget(QWidget* parent):
												QWidget(parent),
												default_grain_size_(10),
												grain_size_pixels_(10),
												scale_factor_(1),
												control_key_pressed_(false),
												mouse_pressed_(false),
												period_bits_(8)
{
	setMouseTracking(true);
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

void SetBit( uint8_t* mass, size_t bitpos, uint8_t bit)
{
	uint8_t mask = ~(1<<(bitpos%CHAR_BIT));
	mass[bitpos/CHAR_BIT] &= mask;
	mass[bitpos/CHAR_BIT] ^= (bit << (bitpos%CHAR_BIT));
}

void BitViewWidget::paintEvent(QPaintEvent* event)
{
	num_cols_on_widget_ = width()/grain_size_pixels_;
	num_rows_on_widget_ = height()/grain_size_pixels_;

	num_rows_in_data_ = (data_.size() * CHAR_BIT + (period_bits_ - 1))/period_bits_ ;
	num_cols_in_data_ = std::min(period_bits_ , data_.size()*CHAR_BIT);

	SetScrollBars();

	// column and row offset are coordinates (x,y) of top left bit
	size_t column_offset = 0;
	size_t row_offset = 0;
	if ( hor_scrollbar_ != nullptr && ver_scrollbar_ != nullptr)
	{
		column_offset 	= hor_scrollbar_->value();
		row_offset 		= ver_scrollbar_->value();
	}

	QPainter painter(this);

	if(data_.empty())
	{
		painter.fillRect(0,0,width(),height(),Qt::darkCyan);
	}
	else
	{
		// number of rows and columns to draw on the currently available space
		size_t num_rows = std::min(num_rows_on_widget_ , num_rows_in_data_);
		size_t num_cols = std::min(num_cols_on_widget_ , num_cols_in_data_);

		// Pen to draw grid
		QPen pen;
		pen.setWidth(0);
		pen.setColor(Qt::darkGray);
		painter.setPen(pen);

		// scale
		painter.scale(grain_size_pixels_,grain_size_pixels_);

		// fill background rectangle except the last row
		painter.fillRect(0,0,num_cols,num_rows-1,Qt::black);

		// draw green squares
		for (size_t i = 0; i < num_rows-1; ++i) // all rows except the last one
		{
			for (size_t j = 0; j < num_cols; ++j)
			{
				if ( GetBit( (const uint8_t*)data_.data(),column_offset + j + period_bits_*(row_offset + i)) == 1)
				{
					painter.fillRect(j,i,1,1,Qt::green);
				}
			}
		}

		// draw grid except for the last row
		if ( grain_size_pixels_ >= default_grain_size_ )
		{
			for (size_t i = 0; i < num_rows-1; ++i) // all rows except the last one
			{
				for (size_t j = 0; j < num_cols; ++j)
				{
					painter.drawRect(j,i,1,1);
				}
			}
		}

		// taking care of the last row exclusively
		size_t i = num_rows-1;
		for (size_t j = 0; j < num_cols; ++j)
		{
			size_t bitpos = column_offset + j + period_bits_*(row_offset + i);
			if (bitpos >= data_.size()*CHAR_BIT)
			{
				return;
			}
			if ( GetBit( (const uint8_t*)data_.data(),bitpos) == 1)
			{
				// one
				painter.fillRect(j,i,1,1,Qt::green);
				// grid
				if ( grain_size_pixels_ >= default_grain_size_ )
				{
					painter.drawRect(j,i,1,1);
				}
			}
			else
			{
				// zero
				painter.fillRect(j,i,1,1,Qt::black);
				// grid
				if ( grain_size_pixels_ >= default_grain_size_ )
				{
					painter.drawRect(j,i,1,1);
				}
			}
		}
	}

}

void BitViewWidget::setPeriod(int value)
{
	if ( hor_scrollbar_ != nullptr && ver_scrollbar_ != nullptr)
	{
		hor_scrollbar_->setValue(0);
		ver_scrollbar_->setValue(0);
	}
	period_bits_ = value;
	update();
}

void BitViewWidget::CaptureScrollBars(QScrollBar* vertical, QScrollBar* horizontal)
{
	if ( vertical != nullptr && horizontal != nullptr)
	{
		ver_scrollbar_ = vertical;
		hor_scrollbar_ = horizontal;
	}
	QObject::connect(vertical, SIGNAL(valueChanged(int)), this, SLOT(ScrollBarValueChangedSlot(int)));
	QObject::connect(horizontal, SIGNAL(valueChanged(int)), this, SLOT(ScrollBarValueChangedSlot(int)));
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

void BitViewWidget::wheelEvent(QWheelEvent* event)
{
	int num_steps = event->angleDelta().y() / 120;

	if (control_key_pressed_)
	{
		for ( int i = 0; i < num_steps; ++i)
		{
			ZoomIn();
		}
		for ( int i = 0; i > num_steps; --i)
		{
			ZoomOut();
		}
	}
	else
	{
		int val = num_steps*height()/(int)grain_size_pixels_/10;
		ver_scrollbar_->setValue( ver_scrollbar_->value() - val );
	}

	event->accept();
}

void BitViewWidget::ScrollBarValueChangedSlot(int dummy) // just to call update()
{
	update();
}

void BitViewWidget::ZoomIn()
{
	if (scale_factor_ * 1.25 <= 4)
	{
		scale_factor_ *= 1.25;

		int value = (int)(scale_factor_*10);

		grain_size_pixels_ = value;
		update();
	}
	emit ZoomChanged(scale_factor_);
}

void BitViewWidget::ZoomOut()
{
	if (scale_factor_ * 0.8 >= 0.1)
	{
		scale_factor_ *= 0.8;

		int value = (int)(scale_factor_*10);

		if (value != 0)
		{
			grain_size_pixels_ = value;
			update();
		}
	}
	emit ZoomChanged(scale_factor_);
}


void BitViewWidget::SetControlKeyPressed(bool val)
{
	control_key_pressed_ = val;
}

void BitViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	// probably useless
	current_column_under_cursor_in_widget_ = event->pos().x()/grain_size_pixels_;
	// probably useless
	current_row_under_cursor_in_widget_ = event->pos().y()/grain_size_pixels_;

	current_column_under_cursor_in_data_ 	= current_column_under_cursor_in_widget_ 	+ hor_scrollbar_->value();
	current_row_under_cursor_in_data_ 		= current_row_under_cursor_in_widget_ 		+ ver_scrollbar_->value();

	if ( current_column_under_cursor_in_data_ 	>= num_cols_in_data_ ||
		 current_row_under_cursor_in_data_ 		>= num_rows_in_data_	)
	{
		current_column_under_cursor_in_data_ = -1;
		current_row_under_cursor_in_data_ = -1;
	}
	else
	{
		if (mouse_pressed_)
		{
			SetBit(data_.data(), current_column_under_cursor_in_data_ + current_row_under_cursor_in_data_ * period_bits_, 1);
			update();
		}
	}

	emit CursorPositionChanged(current_column_under_cursor_in_data_, current_row_under_cursor_in_data_);

}

void BitViewWidget::mousePressEvent(QMouseEvent* event)
{
	mouse_pressed_ = true;
}

void BitViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	mouse_pressed_ = false;
}
