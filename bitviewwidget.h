#pragma once

#include "QtWidgets/QWidget"
#include "QPainter"
#include "QFile"
#include "QDataStream"

#include <vector>

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

class BitViewWidget : public QWidget
{
	Q_OBJECT
public:

	BitViewWidget(QWidget* parent);

	size_t GetDataSize();
	size_t GetScale();
	size_t GetPeriod();
	void ReadFile(QFile* file);

public slots:
	void setHorizontalScrollBarValue(int value);
	void setVerticalScrollBarValue(int value);
	void setPeriod(int value);
	void setScale(int value);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	size_t hor_val_;
	size_t ver_val_;
	size_t period_bits_;

	std::vector<uint8_t> data_;
	size_t scale_;
};


