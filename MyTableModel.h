#pragma once
#include <qabstractitemmodel.h>
#include "new_repo.h"

class MyTableModel: public QAbstractTableModel
{
private:
	PetRepo& repo;

public:
	MyTableModel::MyTableModel(PetRepo& r) : repo{ r } {}

	MyTableModel::~MyTableModel() {}

	int MyTableModel::rowCount(const QModelIndex & index) const
	{
		return this->repo.getLength();
	}

	int MyTableModel::columnCount(const QModelIndex & index) const
	{
		return 5;
	}

	QVariant MyTableModel::data(const QModelIndex & index, int role) const
	{
		int row = index.row();
		int col = index.column();

		std::vector<Pet> pets = this->repo.getAllPets();

		if (row < 0 || row >= this->repo.getLength())
			return QVariant{};

		Pet currentPet = pets[row];

		if (role == Qt::DisplayRole)
		{

			switch (col)
			{
			case 0:
				return QString::fromStdString(currentPet.getBreed());
			case 1:
				return QString::fromStdString(currentPet.getName());
			case 2:
				return QString::fromStdString(std::to_string(currentPet.getAge()));
			case 3:
				return QString::fromStdString(currentPet.getPhotograph());
			case 4:
				return QString::fromStdString(std::to_string(currentPet.getWeight()));
			}
		}

		if (role == Qt::FontRole)
		{
			QFont f { "Times", 15, 10 };
			return f;
		}

		return QVariant();
	}

};

