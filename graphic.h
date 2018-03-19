#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QWidget>
#include "ui_graphic.h"
#include "ctrl.h"
#include "qcustomplot.h"
#include <vector>

class graphic : public QWidget
{
	Q_OBJECT

public:
	graphic(PetController* c,QWidget *parent = 0)
	{
		this->c = c;
		this->customPlot = new QCustomPlot();
		this->setAttribute(Qt::WA_DeleteOnClose);
		ui.setupUi(this);
		this->showMaximized();

		std::vector<std::string> breeds;
		std::vector<int>quantities;

		for (int i = 0; i < this->c->getLength(); i++)
		{
			Pet g = this->c->getPetOnPos(i);
			if (!(std::find(breeds.begin(), breeds.end(), g.getBreed()) != breeds.end()))
			{
				breeds.push_back(g.getBreed());
				quantities.push_back(0);
			}
		}

		for (int i = 0; i < breeds.size(); i++)
		{
			for (int j = 0; j < this->c->getLength(); j++)
				if (breeds[i] == this->c->getPetOnPos(j).getBreed())
					quantities[i]++;
		}

		// set dark background gradient:
		QLinearGradient gradient(0, 0, 0, 400);
		gradient.setColorAt(0, QColor(90, 90, 90));
		gradient.setColorAt(0.38, QColor(105, 105, 105));
		gradient.setColorAt(1, QColor(70, 70, 70));
		customPlot->setBackground(QBrush(gradient));

		// create empty bar chart objects:
		QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
		// gives more crisp, pixel aligned bar borders
		fossil->setAntialiased(false);
		fossil->setStackingGap(1);
		// set names and colors:
		fossil->setName("Dog breeds");
		fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
		fossil->setBrush(QColor(111, 9, 176));

		// prepare x axis with country labels:
		QVector<double> ticks;
		QVector<QString> labels;

		for (int i = 0; i < breeds.size(); i++)
		{
			ticks << i + 1;
			labels << QString::fromStdString(breeds[i]);
		}

		QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
		textTicker->addTicks(ticks, labels);
		customPlot->xAxis->setTicker(textTicker);
		customPlot->xAxis->setTickLabelRotation(60);
		customPlot->xAxis->setSubTicks(false);
		customPlot->xAxis->setTickLength(0, 4);
		customPlot->xAxis->setRange(0, 8);
		customPlot->xAxis->setBasePen(QPen(Qt::white));
		customPlot->xAxis->setTickPen(QPen(Qt::white));
		customPlot->xAxis->grid()->setVisible(true);
		customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
		customPlot->xAxis->setTickLabelColor(Qt::white);
		customPlot->xAxis->setLabelColor(Qt::white);

		// prepare y axis:
		customPlot->yAxis->setRange(0, 12.1);
		customPlot->yAxis->setPadding(5); // a bit more space to the left border
		customPlot->yAxis->setLabel("Number of dogs");
		customPlot->yAxis->setBasePen(QPen(Qt::white));
		customPlot->yAxis->setTickPen(QPen(Qt::white));
		customPlot->yAxis->setSubTickPen(QPen(Qt::white));
		customPlot->yAxis->grid()->setSubGridVisible(true);
		customPlot->yAxis->setTickLabelColor(Qt::white);
		customPlot->yAxis->setLabelColor(Qt::white);
		customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
		customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

		// Add data:
		QVector<double> fossilData;
		for (int i = 0; i < breeds.size(); i++)
			fossilData << quantities[i];

		fossil->setData(ticks, fossilData);

		// setup legend:
		customPlot->legend->setVisible(true);
		customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
		customPlot->legend->setBrush(QColor(255, 255, 255, 100));
		customPlot->legend->setBorderPen(Qt::NoPen);
		QFont legendFont = font();
		legendFont.setPointSize(10);
		customPlot->legend->setFont(legendFont);
		customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

		QHBoxLayout* layout = new QHBoxLayout{ this };
		layout->addWidget(this->customPlot);
	}
	~graphic()
	{
		delete this->customPlot;
	}

private:
	Ui::graphic ui;
	PetController* c;
	QCustomPlot* customPlot;
};

#endif // GRAPHIC_H
