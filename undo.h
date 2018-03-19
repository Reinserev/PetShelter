#pragma once
#include "Pet.h"
#include "new_repo.h"

class UndoAction
{
public:
	UndoAction() {}

	virtual ~UndoAction() {}

	virtual void executeUndo() = 0; //pure virtual function

	char getType()
	{
		return this->type;
	}
	
	virtual Pet getPet() = 0; // pure virtual

protected:
	char type = 'e';
};

class UndoAdd : public UndoAction {
public:
	UndoAdd(PetRepo &r, Pet s) : repo{ r }, addedPet{ s } { this->type = 'a'; } // initializer list is a MUST, because we have a reference to a repositoy

	~UndoAdd() {}

	void executeUndo() override { this->repo.del(this->addedPet.getPhotograph()); }

	Pet getPet() override { return this->addedPet; }

private:
	Pet addedPet;
	PetRepo& repo;
};

class UndoRemove : public UndoAction {
public:
	UndoRemove(PetRepo &r, Pet s) : repo{ r }, deletedPet{ s } { this->type = 'r'; }

	~UndoRemove() {}

	void executeUndo() override { this->repo.add(deletedPet); }

	Pet getPet() override { return this->deletedPet; }

private:
	Pet deletedPet;
	PetRepo& repo;
};

class UndoUpdate : public UndoAction {
public:
	UndoUpdate(PetRepo &r, Pet s) : repo{ r }, updatedPet{ s } { this->type = 'u'; }

	~UndoUpdate() {}

	void executeUndo() override { this->repo.update(updatedPet); }

	Pet getPet() override { return this->updatedPet; }

private:
	Pet updatedPet;
	PetRepo& repo;
};