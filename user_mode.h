#ifndef USER_MODE_H
#define USER_MODE_H

#include <QWidget>
#include "ui_user_mode.h"
#include "ctrl.h"

class user_mode : public QWidget
{
	Q_OBJECT

public:
	user_mode(PetController*c,QWidget *parent = 0)
	{
		this->setAttribute(Qt::WA_DeleteOnClose);
		ui.setupUi(this);
		QObject::connect(this->ui.adoptButton, SIGNAL(clicked()), this, SLOT(YesHandler()));
		QObject::connect(this->ui.SkipButton, SIGNAL(clicked()), this, SLOT(SkipHandler()));
		QObject::connect(this->ui.ExportButton, SIGNAL(clicked()), this, SLOT(ExportHandler()));
		QObject::connect(this->ui.radioButton, SIGNAL(clicked()), this, SLOT(Change_filtering()));
		this->c = c;
		g = this->c->AdoptionMode(1);
		this->ui.breed_info->setText(QString::fromStdString(g.getBreed()));
		this->ui.name_info->setText(QString::fromStdString(g.getName()));
		this->ui.age_info->setText(QString::fromStdString(std::to_string(g.getAge())));
		this->ui.weight_info->setText(QString::fromStdString(std::to_string(g.getWeight())));
		//ShellExecuteA(NULL, NULL, "firefox.exe", g.getPhotograph().c_str(), NULL, SW_SHOWMAXIMIZED);
	}
	~user_mode() {}

	// When an item in the list is clicked, the text boxes get filled with the item's information
	void populatePetsList()
	{
		// clear the list, if there are elements in it
		if (this->ui.listWidget->count() > 0)
			this->ui.listWidget->clear();

		for (int i = 0; i<this->c->getAdoptionLength(); i++)
		{
			Pet g = this->c->getAdoptedPetOnPos(i);

			QString itemInList = QString::fromStdString(g.getBreed() + " - " + g.getName()); // edit here for additional information :)

			QListWidgetItem* item = new QListWidgetItem{ itemInList };
			QFont f{ "Verdana", 15 };
			item->setFont(f);

			this->ui.listWidget->addItem(item);
		}

		// set the selection on the first item in the list
		if (this->c->getLength() > 0)
			this->ui.listWidget->setCurrentRow(0);
	}

public slots:
	void YesHandler()
	{
		if (flag == 0)
			g = this->c->AdoptionMode(2);
		else
			g = this->c->CustomAdoptionMode(2, breed, age);
		this->ui.breed_info->setText(QString::fromStdString(g.getBreed()));
		this->ui.name_info->setText(QString::fromStdString(g.getName()));
		this->ui.age_info->setText(QString::fromStdString(std::to_string(g.getAge())));
		this->ui.weight_info->setText(QString::fromStdString(std::to_string(g.getWeight())));
		//ShellExecuteA(NULL, NULL, "firefox.exe", g.getPhotograph().c_str(), NULL, SW_SHOWMAXIMIZED);
		this->populatePetsList();
	}

	void SkipHandler()
	{
		if (flag == 0)
			g = this->c->AdoptionMode(3);
		else
			g = this->c->CustomAdoptionMode(3,breed,age);
		this->ui.breed_info->setText(QString::fromStdString(g.getBreed()));
		this->ui.name_info->setText(QString::fromStdString(g.getName()));
		this->ui.age_info->setText(QString::fromStdString(std::to_string(g.getAge())));
		this->ui.weight_info->setText(QString::fromStdString(std::to_string(g.getWeight())));
		//ShellExecuteA(NULL, NULL, "firefox.exe", g.getPhotograph().c_str(), NULL, SW_SHOWMAXIMIZED);
		this->populatePetsList();
	}

	void ExportHandler()
	{
		this->c->save_adopted();
		this->c->display_adopted();
	}

	void Change_filtering()
	{
		std::string tmp1 = this->ui.filter_breed->text().toStdString();
		std::string tmp2 = this->ui.filter_weight->text().toStdString();
		if (tmp1.length() == 0 || tmp2.length() == 0)
		{
			this->ui.radioButton->setChecked(false);
			return;
		}
		try
		{
			breed = this->ui.filter_breed->text().toStdString();
			age = std::stoi(this->ui.filter_weight->text().toStdString());
		}
		catch (std::exception &e)
		{
			this->ui.radioButton->setChecked(false);
			return;
		}

		flag = 1;
		breed = this->ui.filter_breed->text().toStdString();
		age = std::stoi(this->ui.filter_weight->text().toStdString());
		g=this->c->CustomAdoptionMode(1, breed, age);
		this->ui.breed_info->setText(QString::fromStdString(g.getBreed()));
		this->ui.name_info->setText(QString::fromStdString(g.getName()));
		this->ui.age_info->setText(QString::fromStdString(std::to_string(g.getAge())));
		this->ui.weight_info->setText(QString::fromStdString(std::to_string(g.getWeight())));
	}

private:
	Ui::user_mode ui;
	PetController *c;
	int flag = 0;
	Pet g;
	std::string breed;
	int age;
};

#endif // USER_MODE_H
