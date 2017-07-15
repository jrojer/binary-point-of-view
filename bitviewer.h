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

public slots:
	void ZoomChanged(double);

private slots:
	void on_pushButton_open_file_clicked();

protected:
	void dropEvent(QDropEvent* event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;

private:
	void ProcessFile(const QString& filename);
	Ui::BitViewerMainWindow ui;
};

