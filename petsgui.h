#include <qwidget.h>
#include <QListWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include "ctrl.h"
#include "user_mode.h"
#include "graphic.h"
#include <vector>
#include <algorithm>
#include "tableviewexample.h"

class Validator {
public:
	static bool ValidName(std::string param)
	{
		if (param.length() > 0)
			return true;
		return false;
	}

	static bool ValidNumber(int nr)
	{
		if (nr>0)
			return true;
		return false;
	}

	static bool ValidLink(std::string zelda)
	{
		char photograph[91];
		strcpy(photograph, zelda.c_str());
		char head[8], secure_head[9];
		strncpy(head, photograph, 7);
		head[7] = '\0';
		strncpy(secure_head, photograph, 8);
		secure_head[8] = '\0';
		bool not_secure_ok = !strcmp(head, "http://");
		bool secure_ok = !strcmp(secure_head, "https://");
		if (!(secure_ok || not_secure_ok))
			return false;
		if (secure_ok)
		{
			if (!isalnum(photograph[8]))
				return false;
		}
		else
		{
			if (!isalnum(photograph[7]))
				return false;
		}
		char* pointer = strchr(photograph, '.');
		if (pointer == NULL)
			return false;
		long long pos = pointer - photograph;
		if (!(isalnum(photograph[pos + 1]) && isalnum(photograph[pos + 2])))
			return  false;
		return true;
	}
};

class PetsGUI : public QWidget
{
	Q_OBJECT

public:
	PetsGUI(PetController *c,QWidget *parent = 0)
	{
		this->controller = c;
		this->initGUI(); // draw stuff
		this->connectSignalsAndSlots(); // define actions
		this->populatePetsList(); // show list
		this->showMaximized();
	}
	~PetsGUI()
	{
		delete this->controller;
	}

private:
	int order_flag;

	QListWidget* petsList;

	QLineEdit* breedEdit;
	QLineEdit* petNameEdit;
	QLineEdit* ageEdit;
	QLineEdit* photoEdit;
	QLineEdit* weightEdit;

	QPushButton* addPetButton;
	QPushButton* deletePetButton;
	QPushButton* updatePetButton;
	QPushButton* ShowPetButton;
	QPushButton* adopt_opener;
	QPushButton* graph_opener;
	QPushButton* undo_button;
	QPushButton* redo_button;
	QPushButton* table_view;

	QRadioButton* normal_order;
	QRadioButton* sorted_order;
	QRadioButton* shuffled_order;

	PetController* controller;

	void initGUI()
	{
		//Petral layout of the window
		QHBoxLayout* layout = new QHBoxLayout{ this };

		// left side - just the list
		this->petsList = new QListWidget{};
		// set the selection model
		this->petsList->setSelectionMode(QAbstractItemView::SingleSelection);
		layout->addWidget(this->petsList);

		// right side - pet information + buttons
		QWidget* rightSide = new QWidget{};
		QVBoxLayout* vLayout = new QVBoxLayout{ rightSide };

		// pet information
		QWidget* petDataWidget = new QWidget{};
		QFormLayout* formLayout = new QFormLayout{ petDataWidget };

		this->petNameEdit = new QLineEdit{};
		this->breedEdit = new QLineEdit{};
		this->ageEdit = new QLineEdit{};
		this->photoEdit = new QLineEdit{};
		this->weightEdit = new QLineEdit{};

		QFont f{ "Verdana", 15 };

		QLabel* petLabel = new QLabel{ "&Name : " };
		petLabel->setBuddy(this->petNameEdit);
		petLabel->setFont(f);

		QLabel* breedLabel = new QLabel{ "&Breed:" };
		breedLabel->setBuddy(this->breedEdit);
		breedLabel->setFont(f);

		QLabel* ageLabel = new QLabel{ "&Age:" };
		ageLabel->setBuddy(this->ageEdit);
		ageLabel->setFont(f);

		QLabel* photoLabel = new QLabel{ "&Photo:" };
		photoLabel->setBuddy(this->photoEdit);
		photoLabel->setFont(f);

		QLabel* weightLabel = new QLabel{ "&Weight:" };
		weightLabel->setBuddy(this->weightEdit);
		weightLabel->setFont(f);

		this->breedEdit->setFont(f);
		this->petNameEdit->setFont(f);
		this->ageEdit->setFont(f);
		this->photoEdit->setFont(f);
		this->weightEdit->setFont(f);

		formLayout->addRow(breedLabel, this->breedEdit);
		formLayout->addRow(petLabel, this->petNameEdit);
		formLayout->addRow(ageLabel, this->ageEdit);
		formLayout->addRow(photoLabel, this->photoEdit);
		formLayout->addRow(weightLabel, this->weightEdit);

		vLayout->addWidget(petDataWidget);

		// buttons
		QWidget* buttonsWidget = new QWidget{};
		QVBoxLayout* hLayout = new QVBoxLayout{ buttonsWidget };

		this->addPetButton = new QPushButton("Add");
		this->addPetButton->setFont(f);
		this->deletePetButton = new QPushButton("Delete");
		this->deletePetButton->setFont(f);
		this->updatePetButton = new QPushButton("Update");
		this->updatePetButton->setFont(f);
		this->ShowPetButton = new QPushButton("Show");
		this->ShowPetButton->setFont(f);
		this->adopt_opener = new QPushButton("Adoption mode");
		this->adopt_opener->setFont(f);
		this->graph_opener = new QPushButton("Graphic statistics");
		this->graph_opener->setFont(f);
		this->undo_button = new QPushButton("Undo");
		this->undo_button->setFont(f);
		this->redo_button = new QPushButton("Redo");
		this->redo_button->setFont(f);
		this->table_view = new QPushButton("Table view");
		this->table_view->setFont(f);

		this->normal_order = new QRadioButton("Normal order");
		this->normal_order->setFont(f);
		this->sorted_order = new QRadioButton("Sorted order");
		this->sorted_order->setFont(f);
		this->shuffled_order = new QRadioButton("Shuffled order");
		this->shuffled_order->setFont(f);

		hLayout->addWidget(this->addPetButton);
		hLayout->addWidget(this->deletePetButton);
		hLayout->addWidget(this->updatePetButton);
		hLayout->addWidget(this->ShowPetButton);
		hLayout->addWidget(this->adopt_opener);
		hLayout->addWidget(this->graph_opener);
		hLayout->addWidget(this->undo_button);
		hLayout->addWidget(this->redo_button);
		hLayout->addWidget(this->table_view);

		hLayout->addWidget(this->normal_order);
		hLayout->addWidget(this->sorted_order);
		hLayout->addWidget(this->shuffled_order);

		this->normal_order->setChecked(true);
		this->order_flag = 0;

		vLayout->addWidget(buttonsWidget);

		// add everything to the big layout
		layout->addWidget(this->petsList);
		layout->addWidget(rightSide);
	}

	int getSelectedIndex()
	{
		if (this->petsList->count() == 0)
			return -1;

		// get selected index
		QModelIndexList index = this->petsList->selectionModel()->selectedIndexes();
		if (index.size() == 0)
		{
			this->breedEdit->clear();
			this->petNameEdit->clear();
			this->ageEdit->clear();
			this->weightEdit->clear();
			this->photoEdit->clear();
			return -1;
		}

		int idx = index.at(0).row();
		return idx;
	}

	void connectSignalsAndSlots()
	{
		// when the vector of pets is updated - re-populate the list
		//QObject::connect(this, SIGNAL(petsUpdatedSignal()), this, SLOT(populatePetsList()));
		QObject::connect(this, &PetsGUI::petsUpdatedSignal, this, &PetsGUI::populatePetsList);

		// add a connection: function listItemChanged() will be called when an item in the list is selected
		QObject::connect(this->petsList, &QListWidget::itemSelectionChanged, this, [this]() {this->listItemChanged(); });

		// add button connections
		QObject::connect(this->addPetButton, SIGNAL(clicked()), this, SLOT(addPetButtonHandler()));
		QObject::connect(this->deletePetButton, SIGNAL(clicked()), this, SLOT(deletePetButtonHandler()));
		QObject::connect(this->updatePetButton, SIGNAL(clicked()), this, SLOT(UpdatePetButtonHandler()));
		QObject::connect(this->ShowPetButton, SIGNAL(clicked()), this, SLOT(ShowPetButtonHandler()));
		QObject::connect(this->adopt_opener, SIGNAL(clicked()), this, SLOT(AdoptOpenerHandler()));
		QObject::connect(this->graph_opener, SIGNAL(clicked()), this, SLOT(GraphOpenerHandler()));
		QObject::connect(this->undo_button, SIGNAL(clicked()), this, SLOT(UndoHandler()));
		QObject::connect(this->redo_button, SIGNAL(clicked()), this, SLOT(RedoHandler()));
		QObject::connect(this->table_view, SIGNAL(clicked()), this, SLOT(TableHandler()));

		QObject::connect(this->normal_order, SIGNAL(clicked()), this, SLOT(NormalOrderHandler()));
		QObject::connect(this->sorted_order, SIGNAL(clicked()), this, SLOT(SortedOrderHandler()));
		QObject::connect(this->shuffled_order, SIGNAL(clicked()), this, SLOT(ShuffledOrderHandler()));
	}

	void listItemChanged()
	{
		int idx = this->getSelectedIndex();
		if (idx == -1)
			return;

		// get item at the selected index
		if (idx >= this->controller->getLength())
			return;
		Pet g = this->controller->getPetOnPos(idx);

		this->breedEdit->setText(QString::fromStdString(g.getBreed()));
		this->petNameEdit->setText(QString::fromStdString(g.getName()));
		this->ageEdit->setText(QString::fromStdString(std::to_string(g.getAge())));
		this->photoEdit->setText(QString::fromStdString(g.getPhotograph()));
		this->weightEdit->setText(QString::fromStdString(std::to_string(g.getWeight())));
	}

	bool isOkNumber(std::string s)
	{
		int i;
		for (i = 0; i < s.length(); i++)
			if (!(s[i] >= '0' && s[i] <= '9'))
				return false;
		return true;
	}

signals:
	void petsUpdatedSignal();

	public slots:

	// When an item in the list is clicked, the text boxes get filled with the item's information
	void populatePetsList()
	{
		// clear the list, if there are elements in it
		if (this->petsList->count() > 0)
			this->petsList->clear();

		if (this->order_flag == 0)
		{
			for (int i = 0; i<this->controller->getLength(); i++)
			{
				Pet g = this->controller->getPetOnPos(i);

				QString itemInList = QString::fromStdString(g.getName() + " - " + g.getBreed()); // edit here for additional information :)

				QListWidgetItem* item = new QListWidgetItem{ itemInList };
				QFont f{ "Verdana", 15 };
				item->setFont(f);

				this->petsList->addItem(item);
			}
		}
		else if (this->order_flag == 1)
		{
			std::vector<Pet> dogs;
			int i;
			for (i = 0; i < this->controller->getLength(); i++)
				dogs.push_back(this->controller->getPetOnPos(i));

			std::sort(dogs.begin(), dogs.end());

			for (int i = 0; i<this->controller->getLength(); i++)
			{
				Pet g = dogs[i];

				QString itemInList = QString::fromStdString(g.getName() + " - " + g.getBreed()); // edit here for additional information :)

				QListWidgetItem* item = new QListWidgetItem{ itemInList };
				QFont f{ "Verdana", 15 };
				item->setFont(f);

				this->petsList->addItem(item);
			}
		}
		else if (this->order_flag == 2)
		{
			std::vector<int> shuffle;
			int i;
			for (i = 0; i < this->controller->getLength(); i++)
				shuffle.push_back(i);
			std::random_shuffle(shuffle.begin(), shuffle.end());

			for (int i = 0; i<this->controller->getLength(); i++)
			{
				Pet g = this->controller->getPetOnPos(shuffle[i]);

				QString itemInList = QString::fromStdString(g.getName() + " - " + g.getBreed()); // edit here for additional information :)

				QListWidgetItem* item = new QListWidgetItem{ itemInList };
				QFont f{ "Verdana", 15 };
				item->setFont(f);

				this->petsList->addItem(item);
			}
		}

		// set the selection on the first item in the list
		if (this->controller->getLength() > 0)
			this->petsList->setCurrentRow(0);
	}

	void addPetButtonHandler()
	{
		// read data from the textboxes and add the new pet
		QString breedName = this->breedEdit->text();
		QString petName = this->petNameEdit->text();
		QString Qage = this->ageEdit->text();
		QString Qphoto = this->photoEdit->text();
		QString Qweight = this->weightEdit->text();

		if (isOkNumber(Qage.toStdString()) == true && isOkNumber(Qweight.toStdString()) == true)
		{
			std::string breed = breedName.toStdString();
			std::string name = petName.toStdString();
			int age = std::stoi(Qage.toStdString());
			std::string photograph = Qphoto.toStdString();
			int weight = std::stoi(Qweight.toStdString());

			if (breed.length() > 0 && name.length() > 0 && age > 0 && photograph.length() > 0 && weight > 0)
			{
				this->controller->add(breed, name, age, photograph, weight);
				emit petsUpdatedSignal();
			}
		}
	}

	void deletePetButtonHandler()
	{
		QString Qphoto = this->photoEdit->text();
		std::string photograph = Qphoto.toStdString();

		if (photograph.length() > 0)
		{
			this->controller->del(photograph);
			// emit the signal: the pets were updated
			emit petsUpdatedSignal();
		}
	}


	void UpdatePetButtonHandler()
	{
		QString breedName = this->breedEdit->text();
		QString petName = this->petNameEdit->text();
		QString Qage = this->ageEdit->text();
		QString Qphoto = this->photoEdit->text();
		QString Qweight = this->weightEdit->text();

		if (isOkNumber(Qage.toStdString()) == true && isOkNumber(Qweight.toStdString()) == true)
		{
			std::string breed = breedName.toStdString();
			std::string name = petName.toStdString();
			int age = std::stoi(Qage.toStdString());
			std::string photograph = Qphoto.toStdString();
			int weight = std::stoi(Qweight.toStdString());

			if (breed.length() > 0 && name.length() > 0 && age > 0 && photograph.length() > 0 && weight > 0)
			{
				this->controller->update(breed, name, age, photograph, weight);
				// emit the signal: the pets were updated
				emit petsUpdatedSignal();
			}
		}
	}

	void ShowPetButtonHandler()
	{
		QString Qphoto = this->photoEdit->text();
		std::string photograph = Qphoto.toStdString();
		ShellExecuteA(NULL, NULL, "firefox.exe", photograph.c_str(), NULL, SW_SHOWMAXIMIZED);
	}

	void AdoptOpenerHandler()
	{
		user_mode *UM=new user_mode(this->controller);
		UM->show();
		connect(UM, SIGNAL(destroyed(QObject*)), this, SLOT(populatePetsList()));
	}

	void GraphOpenerHandler()
	{
		graphic *gr = new graphic(this->controller);
		gr->show();
	}

	void NormalOrderHandler()
	{
		this->order_flag = 0;
		emit petsUpdatedSignal();
	}

	void SortedOrderHandler()
	{
		this->order_flag = 1;
		emit petsUpdatedSignal();
	}

	void ShuffledOrderHandler()
	{
		this->order_flag = 2;
		emit petsUpdatedSignal();
	}

	void UndoHandler()
	{
		this->controller->doTheUndo();
		emit petsUpdatedSignal();
	}

	void RedoHandler()
	{
		this->controller->doTheRedo();
		emit petsUpdatedSignal();
	}

	void TableHandler()
	{
		TableViewExample *w = new TableViewExample(*this->controller->getRepo());
		w->show();
	}
};
