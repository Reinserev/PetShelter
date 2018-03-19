#pragma once
#include "pet.h"
#include <vector>
#include <cassert>
#include <string>
#include <algorithm>
#include <fstream>
#include <Windows.h>
#include <shellapi.h>

class PetRepo {
public:
	PetRepo()
	{
	}

	// Destructor.
	virtual ~PetRepo()
	{
	}

	// Adds a copy of the given pet to the Repository.
	int add(Pet p)
	{
		int pos = find(p.getPhotograph());
		if (pos != -1)
			return 0;
		this->Pets.push_back(p); // else-like
		return 1;
	}

	int del(std::string photo) // Deletes the given pet from the repository.
	{
		int pos = find(photo);
		if (pos == -1)
			return 0;
		this->Pets.erase(this->Pets.begin() + pos); // this is how you delete for good from a STL Vector, kids !!
		return 1;
	}

	void delPetOnPos(int pos)
	{
		this->Pets.erase(this->Pets.begin() + pos);
	}

	// Updates the given pet.
	int update(Pet p)
	{
		int pos = find(p.getPhotograph());
		if (pos == -1)
			return 0;
		this->Pets[pos] = p;
		return 1;
	}

	// Finds the position of the pet with the given photograph (considered an ID).
	int find(std::string photograph)
	{
		int ct = 0;
		for (Pet p : this->Pets)
		{
			if (p.getPhotograph() == photograph)
				return ct;
			++ct;
		}
		return -1;
	}

	Pet find_pet(std::string photograph)
	{
		for (Pet p : this->Pets)
		{
			if (p.getPhotograph() == photograph)
				return p;
		}
		return Pet{};
	}

	// Returns the Repo's Length.
	long long getLength()
	{
		return this->Pets.size();
	}

	// Returns the pet in the given position.
	Pet getPetOnPos(int pos)
	{
		if (pos >= 0 && pos < this->Pets.size())
			return this->Pets[pos];
		return Pet{}; //else-like
	}

	std::vector<Pet> getAllPets()
	{
		return this->Pets;
	}

	PetRepo filter(int weight)
	{
		// http://www.cplusplus.com/reference/algorithm/copy_if/

		std::vector<Pet>source = this->Pets;
		std::vector<Pet>destination(source.size());
		auto it = std::copy_if(source.begin(), source.end(), destination.begin(), [&](Pet doggo) {return (doggo < weight); });
		destination.resize(std::distance(destination.begin(), it));

		PetRepo res;
		res.Pets = destination;
		return res;
	}

	PetRepo filter_2nd(std::string breed, int age)
	{
		std::vector<Pet>source = this->Pets;
		std::vector<Pet>destination(source.size());
		auto it = std::copy_if(source.begin(), source.end(), destination.begin(), [&](Pet doggo) {return (doggo.getBreed()==breed && doggo.getAge()<age); });
		destination.resize(std::distance(destination.begin(), it));
		PetRepo res;
		res.Pets = destination;
		return res;
	}

	virtual void display() const  // polymorphic
	{
		;
	}

	virtual void save_data() // polymorphic
	{
		;
	}

	static void TestPetRepo() // Tests the PetRepo.
	{
		assert(true);

		PetRepo repo;
		assert(repo.getLength() == 0);

		Pet p1 = { "a", "b", 1, "c", 0 };
		Pet p2 = { "d", "e", 2, "f", 1 };
		Pet p3 = { "g", "h", 3, "i", 2 };
		Pet p4 = { "T", "S", 4, "i", 3 };

		assert(repo.add(p1) == 1);
		assert(repo.add(p2) == 1);
		assert(repo.add(p3) == 1);

		assert(repo.getLength() == 3);

		assert(repo.add(p1) == 0);

		assert(repo.find("f") == 1);
		assert(repo.find("YEE") == -1);

		assert(repo.getPetOnPos(3) == Pet{});

		assert(repo.getPetOnPos(0) == p1);
		assert(repo.getPetOnPos(1) == p2);
		assert(repo.getPetOnPos(2) == p3);

		assert(repo.del(p3.getPhotograph()) == 1);

		assert(repo.del(p3.getPhotograph()) == 0);

		assert(repo.getLength() == 2);

		assert(repo.getPetOnPos(0) == p1);
		assert(repo.getPetOnPos(1) == p2);

		assert(repo.getPetOnPos(2) == Pet{});

		assert(repo.update(p4) == 0);

		repo.add(p3);
		assert(repo.update(p4) == 1);

		assert(repo.getPetOnPos(0) == p1);
		assert(repo.getPetOnPos(1) == p2);
		assert(repo.getPetOnPos(2) == p4);

		PetRepo yee;
		assert(yee.add(p1) == 1);
		assert(yee.add(p2) == 1);
		assert(yee.add(p3) == 1);
		PetRepo filtered = yee.filter(2);
		assert(filtered.getLength() == 2);
		assert(filtered.getPetOnPos(0) == p1);
		assert(filtered.getPetOnPos(1) == p2);
	}

protected:
	std::vector<Pet> Pets;
};

class CSVRepo : public PetRepo // inheritance
{
private:
	std::string file;
public:
	CSVRepo(std::string file, int flag)
	{
		this->file = file;
		// add CSV-Specific Stuff Here
		if (flag == 0)
			loadDataFromCSV();
	}

	~CSVRepo()
	{
		// destroy CSV-Specific Stuff Here
		saveDataToCSV();
	}

	virtual void loadDataFromCSV()
	{
		std::ifstream fin(this->file);
		std::string linie;
		std::string workbench="";
		std::string breed;
		std::string name;
		std::string age;
		std::string weight;
		std::string photograph;
		int ct;

		while (getline(fin, linie))
		{

			char sir[513];
			strcpy(sir, linie.c_str());
			ct = 0;
			
			char * p = std::strtok(sir, ",");
			while (p != NULL)
			{
				++ct;
				workbench = p;
				switch (ct)
				{
				case 1:
				{
					breed = workbench;
					break;
				}
				case 2:
				{
					name = workbench;
					break;
				}
				case 3:
				{
					age = workbench;
					break;
				}
				case 4:
				{
					weight = workbench;
					break;
				}
				case 5:
				{
					photograph = workbench;
					break;
				}
				}
				p = std::strtok(NULL, ",");
			}
			int varsta = atoi(age.c_str());
			int greutate = atoi(weight.c_str());
			Pet doggo = { breed,name,varsta,photograph,greutate};
			this->add(doggo);
		}
		fin.close();
	}

	void saveDataToCSV()
	{
		std::ofstream fout(this->file);
		for (int i = 0; i < this->Pets.size(); i++)
			fout << this->Pets[i];
		fout.close();
	}

	void display() const override // polymorphic
	{
		std::string path = "./" + this->file;
		ShellExecuteA(NULL, NULL, "excel.exe", path.c_str(), NULL, SW_SHOWMAXIMIZED);
	}

	void save_data() override // polymorphic
	{
		this->saveDataToCSV();
	}
};

class HTMLRepo : public PetRepo // inheritance
{
private:
	std::string file;
public:
	HTMLRepo(std::string file, int flag)
	{
		this->file = file;
		// add CSV-Specific Stuff Here
		if (flag == 0)
			loadDataFromHTML();
	}

	~HTMLRepo()
	{
		// destroy CSV-Specific Stuff Here
		saveDataToHTML();
	}

	virtual void loadDataFromHTML()
	{
		std::ifstream fin(this->file);

		std::string dummy;
		std::string breed = "yee";
		std::string name;
		std::string age;
		std::string weight;
		std::string photograph;

		for (int i = 1; i <= 14; i++)
			getline(fin, dummy);

		while (true)
		{
			getline(fin, dummy);
			if (dummy == "</table>")
				break;

			getline(fin, breed);
			getline(fin, name);
			getline(fin, age);
			getline(fin, weight);
			getline(fin, photograph);
			getline(fin, dummy);

			breed.erase(0, 4);
			name.erase(0, 4);
			age.erase(0, 4);
			weight.erase(0, 4);
			photograph.erase(0, 13);

			breed = breed.substr(0, breed.size() - 5);
			name = name.substr(0, name.size() - 5);
			age = age.substr(0, age.size() - 5);
			weight = weight.substr(0, weight.size() - 5);
			photograph = photograph.substr(0, photograph.size() - 14);


			int varsta = atoi(age.c_str());
			int greutate = atoi(weight.c_str());
			Pet doggo = { breed,name,varsta,photograph,greutate };
			this->add(doggo);
		}

		fin.close();
	}

	void saveDataToHTML()
	{
		std::ofstream fout(this->file);

		fout << "<!DOCTYPE html>\n";
		fout << "<html>\n";
		fout << "<head>\n";
		fout << "<title>Dogs</title>\n";
		fout << "</head>\n";
		fout << "<body>\n";
		fout << "<table border=\"1\">\n";
		fout << "<tr>\n";
		fout << "<td>Breed</td>\n";
		fout << "<td>Name</td>\n";
		fout << "<td>Age</td>\n";
		fout << "<td>Weight</td>\n";
		fout << "<td>Photograph</td>\n";
		fout << "</tr>\n";

		for (int i = 0; i < this->Pets.size(); i++)
		{
			fout << "<tr>\n";
			fout << "<td>" << this->Pets[i].getBreed() << "</td>\n";
			fout << "<td>" << this->Pets[i].getName() << "</td>\n";
			fout << "<td>" << this->Pets[i].getAge() << "</td>\n";
			fout << "<td>" << this->Pets[i].getWeight() << "</td>\n";
			fout << "<td><a href =" << this->Pets[i].getPhotograph() << ">Link</a></td>\n";
			fout << "</tr>\n";
		}

		fout << "</table>\n";
		fout << "</body>\n";
		fout << "</html>\n";

		fout.close();
	}

	void display() const override // polymorphic
	{
		std::string path = this->file;
		ShellExecuteA(NULL, NULL, "firefox.exe", path.c_str(), NULL, SW_SHOWMAXIMIZED);
	}

	void save_data() override // polymorphic
	{
		this->saveDataToHTML();
	}
};