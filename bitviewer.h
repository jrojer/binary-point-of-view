#pragma once

#include "ui_bitviewer.h"
#include "QtWidgets/QMainWindow"

#include "QFileDialog"
#include "QFile"
#include "QPixmap"
#include "QPainter"
#include "QMimeData"

class BitViewer : public QMainWindow
{
	Q_OBJECT
public:
	BitViewer();

private slots:
	void on_pushButton_open_file_clicked();
	void on_pushButton_plus_clicked();
	void on_pushButton_minus_clicked();

protected:
	void dropEvent(QDropEvent* event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;

private:
	void ProcessFile(const QString& filename);
	double scale_;
	Ui::BitViewerMainWindow ui;
};

