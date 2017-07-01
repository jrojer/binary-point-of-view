#pragma once

#include "QtWidgets/QWidget"
#include "QPainter"
#include "QFile"
#include "QDataStream"
#include "QScrollBar"

#include <vector>
#include <algorithm>

#include <stdint.h>
#include <stddef.h>
#include <limits.h>


/*
 * This widget renders bit view.
 * Every bit is depicted as a square: black square is for 0, green square is for 1.
 *
 * How to use:
 *
 * - Give it data to display:
 *
 * 		through ReadFile(QFile*) method
 *
 * - If you want to be able to scroll,
 * 	 provide the widget access to scrollbars
 *
 * 	  through CaptureScrollBars() method
 *
 * - Then provide it with scrollbar values when needed
 *
 * 	  through setHorizontalScrollBarValue(int) and setVerticalScrollBarValue(int)
 *
 * - Give the widget the period value and the grain size.
 *    (Grain size is the size of square's side in pixels, default is 10 pixels,
 *     grain size can be used to zoom in and out)
 *
 *    through SetPeriod(int) and SetGrainSize(int)
 */

class BitViewWidget : public QWidget
{
	Q_OBJECT
public:

	BitViewWidget(QWidget* parent);

	void ReadFile(QFile* file);
	void CaptureScrollBars(QScrollBar* vertical, QScrollBar* horizontal);

public slots:
	void setHorizontalScrollBarValue(int value);
	void setVerticalScrollBarValue(int value);
	void setPeriod(int value);
	void SetGrainSize(int value);

protected:
	void paintEvent(QPaintEvent* event) override;

private:

	void SetScrollBars();

	std::vector<uint8_t> data_;
	size_t grain_size_pixels_;
	size_t period_bits_;

	QScrollBar* hor_scrollbar_ = nullptr;
	QScrollBar* ver_scrollbar_ = nullptr;

	// column and row offset are coordinates (x,y) of top left bit
	size_t column_offset_;
	size_t row_offset_;

	// number of rows and columns that can be rendered with current width() and height()
	size_t num_cols_on_widget_;
	size_t num_rows_on_widget_;

	// total number of rows and columns that can be rendered with this data
	size_t num_rows_in_data_;
	size_t num_cols_in_data_;
};


