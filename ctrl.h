#pragma once
#include "new_repo.h"
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <vector>
#include "undo.h"

class PetController {
public:
	PetController()
	{
		this->repo = nullptr;
		this->AdoptionRepo = nullptr;
	}

	// Constructor.
	PetController(PetRepo *r, int repo_type)
	{
		this->repo = r;
		this->repo_type = repo_type;
		if(this->repo_type ==0)
			this->AdoptionRepo = new PetRepo;
		else
		{
			if (this->repo_type == 1)
				this->AdoptionRepo = new CSVRepo("CSVAdoptedPets.csv",1);
			else
			{
				if (this->repo_type == 2)
					this->AdoptionRepo = new HTMLRepo("HTMLAdoptedPets.html", 1);
			}
		}
	}

	// Destructor.
	~PetController()
	{
		delete this->repo;
		delete this->AdoptionRepo;
	}

	PetRepo* getRepo()
	{
		return this->repo;
	}

	// Adds a pet given by its fields to the controller.
	int add(std::string breed, std::string name, int age, std::string photograph, int weight)
	{
		Pet p = { breed, name, age, photograph, weight };

		this->undoActions.push_back(std::make_unique<UndoAdd>(*this->repo, p));
		this->clearRedo();

		int res = this->repo->add(p);
		return res;
	}

	// Deletes a pet given by its ID from the controller.
	int del(std::string photograph)
	{
		this->undoActions.push_back(std::make_unique<UndoRemove>(*this->repo, this->repo->find_pet(photograph)));
		this->clearRedo();

		int res = this->repo->del(photograph);
		return res;
	}

	void delPetOnPos(int pos)
	{
		if (pos > 0 || pos >= this->getLength())
			return;
		this->repo->delPetOnPos(pos);
	}

	// Updates a pet given by its fields.
	int update(std::string breed, std::string name, int age, std::string photograph, int weight)
	{
		Pet p = { breed, name, age, photograph, weight };

		this->undoActions.push_back(std::make_unique<UndoUpdate>(*this->repo, this->repo->find_pet(photograph)));
		this->clearRedo();

		int res = this->repo->update(p);
		return res;
	}

	// Returns the position where a pet given by its photograph is found.
	int find(std::string photograph)
	{
		return this->repo->find(photograph);
	}

	// Returns the length of the pet list.
	int getLength()
	{
		return this->repo->getLength();
	}

	int getAdoptionLength()
	{
		return this->AdoptionRepo->getLength();
	}

	// Returns the pet at the given position.
	Pet getPetOnPos(int pos)
	{
		return this->repo->getPetOnPos(pos);
	}

	// Returns the pet at the given position.
	Pet getAdoptedPetOnPos(int pos)
	{
		return this->AdoptionRepo->getPetOnPos(pos);
	}

	void AddSampleData()
	{
		this->add("Labrador Retriever", "Cam", 1, "http://cdn2-www.dogtime.com/assets/uploads/gallery/labrador-retriever-dog-breed-pictures/labrador-retriever-dog-pictures-6.jpg", 1);
		this->add("German Shepherd", "Sam", 2, "http://cdn2-www.dogtime.com/assets/uploads/gallery/german-shepherd-dog-breed-pictures/standing-7.jpg", 2);
		this->add("Golden Retriever", "Hilary", 3, "http://static.animalzoo.ro/uploads/2015/08/golden2.jpg", 3);
		this->add("Bulldog", "Kendl", 4, "http://ichef-1.bbci.co.uk/news/660/cpsprodpb/B22F/production/_90551654_gettyimages-511711526.jpg", 4);
		this->add("Beagle", "Tony", 5, "https://i.ytimg.com/vi/bx7BjjqHf2U/maxresdefault.jpg", 5);
		this->add("French Bulldog", "Carl", 6, "https://s-media-cache-ak0.pinimg.com/736x/87/f9/90/87f99031a6ebd013f14758e61f3b0512.jpg", 6);
		this->add("French Bulldog", "Carlos", 10, "https://s-media-cache-ak0.pinimg.com/736x/87/f9/90/87f99031a6ebd013f14758e61f3b0512.jpeg", 7);
		this->add("Poodle", "Tommy", 7, "http://cdn2-www.dogtime.com/assets/uploads/gallery/poodle-dog-breed-pictures/threequarters-4.jpg", 8);
		this->add("Poodle", "Ken", 70, "http://cdn2-www.dogtime.com/assets/uploads/gallery/poodle-dog-breed-pictures/threequarters-4.jpeg", 9);
		this->add("Rottweiler", "Mercedes", 8, "https://www.pets4homes.co.uk/images/breeds/14/large/fdd5bf3bbf8b6e6c42e44d5bdb44d05c.jpg", 10);
		this->add("Yorkshire Terrier", "Catalina", 9, "https://www.pets4homes.co.uk/images/breeds/20/large/917d77fa7e0070d0e219ba59ff770ade.jpg", 11);
		this->add("Boxer", "Maria", 10, "http://pbs.twimg.com/media/CYH9v1BWQAA8Irg.jpg:larg", 12);
		this->add("Boxer", "Mary", 2, "http://pbs.twimg.com/media/CYH9v1BWQAA8Irg.jpg:lar", 13);
		this->add("Boxer", "Marean", 5, "http://pbs.twimg.com/media/CYH9v1BWQAA8Irg.jpg:la", 14);
	}

	// Adds dogs to the controller's adoption repository while communicating with the UI.
	// This is done by the use of flags: 1 - initialize and retreive first dog; 2 - accept dog and retreive next; 3 - discard dog and retreive next
	Pet AdoptionMode(int flag)
	{
		if (flag == 1) {
			delete this->AdoptionRepo;
			
			if(this->repo_type ==0)
				this->AdoptionRepo = new PetRepo;
			else
			{
				if (this->repo_type == 1)
					this->AdoptionRepo = new CSVRepo("CSVAdoptedPets.csv",1);
				else
				{
					if (this->repo_type == 2)
						this->AdoptionRepo = new HTMLRepo("HTMLAdoptedPets.html", 1);
				}
			}

			this->index = 0;
			return this->repo->getPetOnPos(this->index);
		}
		else { if (flag == 2) {
			if (this->repo->getPetOnPos(this->index) == Pet{})
				return Pet{};

				this->AdoptionRepo->add(this->repo->getPetOnPos(this->index));
				this->repo->del(this->repo->getPetOnPos(this->index).getPhotograph());
				if (this->index == this->getLength())
					this->index = 0;
				return this->repo->getPetOnPos(this->index);
			}
			else { if (flag == 3) {
					this->index++;
					if (this->index == this->getLength())
						this->index = 0;
					return this->repo->getPetOnPos(this->index);
				}
			}
		}
	}

	// Only allows dog matching the given breed and younger than the given age.
	Pet CustomAdoptionMode(int flag, std::string breed, int age)
	{
		if (flag == 1) {
			delete this->AdoptionRepo;

			if (this->repo_type == 0)
				this->AdoptionRepo = new PetRepo;
			else
			{
				if (this->repo_type == 1)
					this->AdoptionRepo = new CSVRepo("CSVAdoptedPets.csv",1);
				else
				{
					if (this->repo_type == 2)
						this->AdoptionRepo = new HTMLRepo("HTMLAdoptedPets.html",1);
				}
			}

			PetRepo custom_dogs = this->repo->filter_2nd(breed,age);
			this->temp = custom_dogs;

			this->index = 0;
			return this->temp.getPetOnPos(this->index);
		}
		else {
			if (flag == 2) {
				if (this->temp.getPetOnPos(this->index) == Pet{})
					return Pet{};

				this->AdoptionRepo->add(this->temp.getPetOnPos(this->index));
				this->repo->del(this->temp.getPetOnPos(this->index).getPhotograph());
				this->temp.del(this->temp.getPetOnPos(this->index).getPhotograph());
				if (this->index == this->temp.getLength())
					this->index = 0;
				return this->temp.getPetOnPos(this->index);
			}
			else {
				if (flag == 3) {
					this->index++;
					if (this->index == this->temp.getLength())
						this->index = 0;
					return this->temp.getPetOnPos(this->index);
				}
			}
		}
	}

	PetRepo filterByWeight(int weight)
	{
		return this->repo->filter(weight);
	}

	void display()
	{
		this->repo->display();
	}

	void save_adopted()
	{
		this->AdoptionRepo->save_data();
	}

	void display_adopted()
	{
		this->AdoptionRepo->display();
	}

	void doTheUndo()
	{
		if (!this->undoActions.empty())
		{
			try
			{
				char type = this->undoActions.back()->getType();
				Pet p = this->undoActions.back()->getPet();
				
				switch (type)
				{
				case 'a':
				{
					this->redoActions.push_back(std::make_unique<UndoRemove>(*this->repo, p));
					break;
				}
				case 'r':
				{
					this->redoActions.push_back(std::make_unique<UndoAdd>(*this->repo, p));
					break;
				}
				case 'u':
				{
					this->redoActions.push_back(std::make_unique<UndoUpdate>(*this->repo, p));
					break;
				}
				default:
					break;
				}

				undoActions.back()->executeUndo();
				undoActions.pop_back();
			}
			catch (std::exception & e) { ; }
		}
	}

	void doTheRedo()
	{
		if (!this->redoActions.empty())
		{
			try
			{
				char type = this->redoActions.back()->getType();
				Pet p = this->redoActions.back()->getPet();

				switch (type)
				{
				case 'a':
				{
					this->undoActions.push_back(std::make_unique<UndoRemove>(*this->repo, p));
					break;
				}
				case 'r':
				{
					this->undoActions.push_back(std::make_unique<UndoAdd>(*this->repo, p));
					break;
				}
				case 'u':
				{
					this->undoActions.push_back(std::make_unique<UndoUpdate>(*this->repo, p));
					break;
				}
				default:
					break;
				}

				redoActions.back()->executeUndo();
				redoActions.pop_back();
			}
			catch (std::exception & e) { ; }
		}
	}

	void clearRedo()
	{
		this->redoActions.clear();
	}

	// Tests.
	static void TestPetContoller()
	{
		PetRepo *repo = new PetRepo;
		PetController default_test;
		PetController ctrl = { repo ,0};

		Pet p1 = { "a", "b", 1, "c", 0 };
		Pet p2 = { "d", "e", 2, "f", 1 };
		Pet p3 = { "g", "h", 3, "i", 2 };
		Pet p4 = { "T", "S", 4, "i", 3 };

		assert(ctrl.add("a", "b", 1, "c", 0) == 1);
		assert(ctrl.add("d", "e", 2, "f", 1) == 1);
		assert(ctrl.add("g", "h", 3, "i", 2) == 1);

		assert(ctrl.add("a", "b", 1, "c", 0) == 0);

		assert(ctrl.find("f") == 1);

		assert(ctrl.getPetOnPos(3) == Pet{});

		assert(ctrl.getPetOnPos(0) == p1);
		assert(ctrl.getPetOnPos(1) == p2);
		assert(ctrl.getPetOnPos(2) == p3);

		assert(ctrl.del("i") == 1);

		assert(ctrl.del("i") == 0);

		assert(ctrl.getLength() == 2);

		assert(ctrl.getPetOnPos(0) == p1);
		assert(ctrl.getPetOnPos(1) == p2);

		assert(ctrl.getPetOnPos(2) == Pet{});

		assert(ctrl.update("T", "S", 4, "i", 3) == 0);

		ctrl.add("g", "h", 3, "i", 2);
		assert(ctrl.update("T", "S", 4, "i", 3) == 1);

		assert(ctrl.getPetOnPos(0) == p1);
		assert(ctrl.getPetOnPos(1) == p2);
		assert(ctrl.getPetOnPos(2) == p4);

		PetRepo *r=new PetRepo;
		PetController c = { r ,0};
		c.AddSampleData();
		Pet doggo = c.AdoptionMode(1);
		assert(doggo.getAge() == 1);
		doggo=c.AdoptionMode(2);
		assert(doggo.getAge() == 2);
		doggo = c.AdoptionMode(3);
		assert(doggo.getAge() == 3);
		doggo = c.AdoptionMode(2);
		doggo = c.getAdoptedPetOnPos(0);
		assert(doggo.getAge() == 1);
		doggo = c.getAdoptedPetOnPos(1);
		assert(doggo.getAge() == 3);
		doggo = c.getAdoptedPetOnPos(2);
		assert(c.getAdoptionLength() == 2);

		PetRepo *R=new PetRepo;
		PetController C = { R ,0};
		C.AddSampleData();
		doggo = C.CustomAdoptionMode(1,"Boxer",10);
		assert(doggo.getAge() == 2);
		doggo = C.CustomAdoptionMode(2, "Boxer", 10);
		assert(doggo.getAge() == 5);
		doggo = C.CustomAdoptionMode(3, "Boxer", 10);
		doggo = C.CustomAdoptionMode(2, "Boxer", 10);
		assert(C.getAdoptionLength() == 2);
		doggo = C.CustomAdoptionMode(2, "Boxer", 10);
		assert(doggo == Pet{});

		doggo = C.CustomAdoptionMode(1, "YEE", 5);
		assert(doggo == Pet{});

		PetRepo ya = ctrl.filterByWeight(1);
		assert(ya.getLength() == 1);

		PetRepo *RR=new PetRepo;
		PetController CC = { RR ,0};
		CC.add("YEE", "DAP", 1, "mup", 5);
		doggo = CC.AdoptionMode(1);
		assert(doggo.getPhotograph() == "mup");
		doggo = CC.AdoptionMode(3);
		assert(doggo.getPhotograph() == "mup");
		doggo = CC.AdoptionMode(2);
		assert(doggo == Pet{});
		assert(CC.getAdoptionLength() == 1);
	}

protected:
	PetRepo *repo;
	PetRepo *AdoptionRepo;
	PetRepo temp;
	int index;
	int repo_type;
	std::vector<std::unique_ptr<UndoAction>> undoActions;
	std::vector<std::unique_ptr<UndoAction>> redoActions;
};