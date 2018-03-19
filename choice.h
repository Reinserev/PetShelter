#ifndef CHOICE_H
#define CHOICE_H

#include <QWidget>
#include "ui_choice.h"
#include "pet.h"
#include "ctrl.h"
#include "petsgui.h"

class choice : public QWidget
{
	Q_OBJECT

public:
	choice(QWidget *parent = 0)
	{
		ui.setupUi(this);
		QObject::connect(this->ui.InMemoryButton, SIGNAL(clicked()), this, SLOT(InMemory()));
		QObject::connect(this->ui.CSVButton, SIGNAL(clicked()), this, SLOT(CSV()));
		QObject::connect(this->ui.HTMLButton, SIGNAL(clicked()), this, SLOT(HTML()));
	}

	~choice() { delete this->w; }

	void run_app()
	{
		Pet::TestPets();
		PetRepo::TestPetRepo();
		PetController::TestPetContoller();

		PetRepo *database = nullptr;

		if (op == 0)
			database = new PetRepo{};
		else
		{
			if (op == 1)
				database = new CSVRepo{ "CSVPets.csv",0 }; // 0 for read from file, 1 for don't (1 only needed to export adoption list);
			else
				if (op == 2)
					database = new HTMLRepo{ "HTMLPets.html",0 };
		}

		PetController *controller = new PetController{ database ,op }; // 1 = flag pt CSV (0 pt mem si 2 pt HTML)

		if (op == 0)
			controller->AddSampleData(); // add all doggo's :D

		w = new PetsGUI(controller);
		w->show();
	}

public slots:
	void InMemory()
	{
		this->op = 0;
		run_app();
		this->close();
	}

	void CSV()
	{
		this->op = 1;
		run_app();
		this->close();
	}

	void HTML()
	{
		this->op = 2;
		run_app();
		this->close();
	}

private:
	Ui::choice ui;
	int op = 0;
	PetsGUI *w;
};

#endif // CHOICE_H
