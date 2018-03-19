#pragma once
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>

class Pet {
public:
	void setBreed(std::string breed)
	{
		this->breed = breed;
	}

	std::string getBreed()
	{
		return this->breed;
	}

	void setName(std::string name)
	{
		this->name = name;
	}

	std::string getName()
	{
		return this->name;
	}

	int getAge()
	{
		return this->age;
	}

	void setAge(int age)
	{
		this->age = age;
	}

	void setPhotograph(std::string photograph)
	{
		this->photograph = photograph;
	}

	std::string getPhotograph()
	{
		return this->photograph;
	}

	Pet() : breed(""), name(""), age(0), photograph(""), weight(0)
	{
		;
	}

	// DEFAULT Constructor: creates a new pet using the given data.
	Pet(std::string breed, std::string name, int age, std::string photograph, int weight) {
		this->breed = breed;
		this->name = name;
		this->age = age;
		this->photograph = photograph;
		this->weight = weight;
	}

	// IDENTITY OPERATOR OVERLOAD
	bool operator== (const Pet pet) const {
		if (this->photograph!=pet.photograph)
			return false;
		return true;
	}

	// LESS THAN OPERATOR OVERLOAD
	bool operator< (const int value) const {
		return this->weight < value;
	}

	bool operator< (const Pet other) const {
		return this->name < other.name;
	}

	// String converter
	std::string to_string()
	{
		std::stringstream yee;
		yee << "Pet of breed " << this->breed << " is named " << this->name<<", " << this->age << " year(s) old and has " << this->photograph << " photograph and weighs "<<this->weight<<" lbs.";
		return yee.str();
	}

	friend std::istream &operator >> (std::istream  &input, Pet &P)
	{
		std::cout << "Input the following: breed, name, age, weight and photograph and press enter after each of them!\n";
		input >> P.breed >> P.name >> P.age >> P.weight >> P.photograph;
		return input;
	}

	friend std::ostream &operator<<(std::ostream &output, const Pet &P)
	{
		Pet temp = P;
		output << temp.getBreed() << ',' << temp.getName() << ',' << temp.getAge() << ',' << temp.getWeight() << ',' << temp.getPhotograph() << '\n';
		return output;
	}

	int getWeight()
	{
		return this->weight;
	}

	void setWeight(int weight)
	{
		this->weight = weight;
	}

	// Tester
	static void TestPets()
	{
		assert(true);

		Pet dog = { "a", "b", 1, "c", 0 };

		assert(dog < 1);
		assert(!(dog < -1));
		assert(!(dog < 0));

		assert(dog.getBreed()=="a");
		assert(dog.getName() == "b");
		assert(dog.getAge() == 1);
		assert(dog.getPhotograph() == "c");

		assert(dog.to_string() == "Pet of breed a is named b, 1 year(s) old and has c photograph and weighs 0 lbs.");

		dog.setBreed("A");
		dog.setName("B");
		dog.setAge(2);
		dog.setPhotograph("C");
		dog.setWeight(5000);

		assert(dog.getBreed() == "A");
		assert(dog.getName()=="B");
		assert(dog.getAge() == 2);
		assert(dog.getPhotograph() == "C");

		assert(dog.to_string() == "Pet of breed A is named B, 2 year(s) old and has C photograph and weighs 5000 lbs.");

		Pet cat = dog;
		assert(cat == dog);
		assert(cat.to_string() == dog.to_string());

		Pet mouser = { "X", "Y", 1, "Z", 1 };
		assert(!(mouser == cat));
		mouser = cat;
		assert(cat == mouser);
		assert(cat.to_string() == dog.to_string());
	}

protected: // breed, name, age, photograph
	std::string breed;
	std::string name;
	int age;
	int weight;
	std::string photograph;
};
