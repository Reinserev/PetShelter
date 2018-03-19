#include <QWidget>
#include "new_repo.h"
#include "MyTableModel.h"
#include "ui_tableviewexample.h"

class TableViewExample : public QWidget
{
	Q_OBJECT

public:

	TableViewExample(PetRepo& r, QWidget *parent = 0) : repo{ r }, QWidget(parent)
	{
			ui.setupUi(this);

			MyTableModel* myModel = new MyTableModel{ this->repo };
			ui.genesTableView->setModel(myModel);
	}

	TableViewExample::~TableViewExample()
	{

	}

private:
	PetRepo& repo;
	Ui::TableViewExample ui;
};
