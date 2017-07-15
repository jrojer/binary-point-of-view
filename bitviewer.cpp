#include "bitviewer.h"

BitViewer::BitViewer(): QMainWindow()
{
	ui.setupUi(this);

	QObject::connect(ui.spinBox_period, SIGNAL(valueChanged(int)), ui.bitview_widget, SLOT(setPeriod(int)));
	QObject::connect(ui.pushButton_minus, SIGNAL(clicked()), ui.bitview_widget, SLOT(ZoomOut()));
	QObject::connect(ui.pushButton_plus, SIGNAL(clicked()), ui.bitview_widget, SLOT(ZoomIn()));
	QObject::connect(ui.bitview_widget, SIGNAL(ZoomChanged(double)), this, SLOT(ZoomChanged(double)));

	ui.bitview_widget->CaptureScrollBars(ui.verticalScrollBar, ui.horizontalScrollBar);

	setAcceptDrops(true);
	setFocusPolicy(Qt::StrongFocus);
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
		ui.bitview_widget->ReadFile(&file);

		file.close();
		this->setWindowTitle("Bit Viewer - " + filename);
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
