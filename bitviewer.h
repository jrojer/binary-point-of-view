#pragma once

#include "ui_bitviewer.h"
#include "QtWidgets/QMainWindow"

#include "QFileDialog"
#include "QFile"
#include "QPixmap"
#include "QPainter"

class BitViewer : public QMainWindow
{
	Q_OBJECT
public:
	BitViewer();

private slots:
	void on_pushButton_open_file_clicked();
	void on_pushButton_plus_clicked();
	void on_pushButton_minus_clicked();

private:
	size_t scale_;
	Ui::BitViewerMainWindow ui;
};

