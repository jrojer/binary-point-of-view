#include "bitviewer.h"

#define MAX_FILE_SIZE 700 * 1024 * 1024

BitViewer::BitViewer(): QMainWindow()
{
	ui.setupUi(this);

	QObject::connect(ui.spinBox_period, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setPeriod(int)));
	QObject::connect(ui.pushButton_minus, SIGNAL(clicked()), ui.bitview_widget, SLOT(ZoomOut()));
	QObject::connect(ui.pushButton_plus, SIGNAL(clicked()), ui.bitview_widget, SLOT(ZoomIn()));
	QObject::connect(ui.bitview_widget, SIGNAL(ZoomChanged(double)), this, SLOT(ZoomChanged(double)));
	QObject::connect(ui.bitview_widget, SIGNAL(CursorPositionChanged(int, int)), this, SLOT(CursorPositionChanged(int, int)));

	ui.bitview_widget->CaptureScrollBars(ui.verticalScrollBar, ui.horizontalScrollBar);

	setAcceptDrops(true);
	setFocusPolicy(Qt::StrongFocus);

	// hide
	ui.label_pos_x->	setHidden(true);
	ui.label_pos_colon->setHidden(true);
	ui.label_pos_y->	setHidden(true);
	ui.label_position->	setHidden(true);
}

void BitViewer::on_pushButton_open_file_clicked()
{
	QString filename =  QFileDialog::getOpenFileName(this);

	ProcessFile(filename);
}
void BitViewer::ProcessFile(const QString& filename)
{
	if (filename.isEmpty())
	{
		return;
	}

	QFile file(filename);

	if ( file.open(QIODevice::ReadOnly))
	{
		if ( file.size() > MAX_FILE_SIZE)
		{
			this->setWindowTitle("Bit Viewer - File is too big!");
			return;
		}
		ui.bitview_widget->ReadFile(&file);

		file.close();
		this->setWindowTitle("Bit Viewer - " + filename);
	}
}
void BitViewer::on_pushButton_save_file_clicked()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setDefaultSuffix(".bin");
	dialog.setViewMode(QFileDialog::Detail);

	QStringList filenames;
	if (dialog.exec())
	{
	    filenames = dialog.selectedFiles();
	}

	if (!filenames.empty())
	{
		QFile file(filenames[0]);
		if ( file.open(QIODevice::WriteOnly))
		{
			ui.bitview_widget->WriteFile(&file);

			file.close();
			this->setWindowTitle("Bit Viewer - " + filenames[0]);
		}
	}
}

void BitViewer::dropEvent(QDropEvent* event)
{
	QString filename = event->mimeData()->urls().at(0).toLocalFile();

	ProcessFile(filename);
}

void BitViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}
void BitViewer::ZoomChanged(double val)
{
	ui.label_zoom_value->setText(QString::number(val));
}

void BitViewer::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Control)
	{
		ui.bitview_widget->SetControlKeyPressed(true);
	}
	else
	{
		QWidget::keyPressEvent(event);
	}
}
void BitViewer::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Control)
	{
		ui.bitview_widget->SetControlKeyPressed(false);
	}
	else
	{
		QWidget::keyPressEvent(event);
	}
}

void BitViewer::CursorPositionChanged(int x, int y)
{
	bool hidden = (x==-1 || y == -1);
	ui.label_position->	setHidden(hidden);
	ui.label_pos_x->	setHidden(hidden);
	ui.label_pos_colon->setHidden(hidden);
	ui.label_pos_y->	setHidden(hidden);

	ui.label_pos_x->setText(QString::number(x));
	ui.label_pos_y->setText(QString::number(y));
}
