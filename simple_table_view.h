#ifndef SIMPLE_TABLE_VIEW_H
#define SIMPLE_TABLE_VIEW_H

#include <QWidget>
#include "ui_simple_table_view.h"

class simple_table_view : public QWidget
{
	Q_OBJECT

public:
	simple_table_view(QWidget *parent = 0)
	{
		ui.setupUi(this);
	}

	~simple_table_view() {}

private:
	Ui::simple_table_view ui;
};

#endif // SIMPLE_TABLE_VIEW_H
