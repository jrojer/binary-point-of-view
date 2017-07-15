#pragma once

#include "ui_bitviewer.h"
#include "QtWidgets/QMainWindow"

#include "QFileDialog"
#include "QFile"
#include "QPixmap"
#include "QPainter"
#include "QMimeData"
#include "QStringList"

class BitViewer : public QMainWindow
{
	Q_OBJECT
public:
	BitViewer();

public slots:
	void ZoomChanged(double);
	void CursorPositionChanged(int x, int y);

private slots:
	void on_pushButton_open_file_clicked();
	void on_pushButton_save_file_clicked();

protected:
	void dropEvent(QDropEvent* event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	void ProcessFile(const QString& filename);
	Ui::BitViewerMainWindow ui;
};

