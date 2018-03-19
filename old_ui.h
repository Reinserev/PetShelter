#pragma once
#include <cstring>
#include <cstdio>
#include <iostream>
#include "ctrl.h"
#include <Windows.h>
#include <shellapi.h>
#include <string>
#include <exception>

class InvalidNumber : public std::exception
{
	virtual const char* what() const throw()
	{
		return "An InvalidNumberException has occured.";
	}
}inv_nr;

class InvalidLink : public std::exception
{
	virtual const char* what() const throw()
	{
		return "An InvalidLinkException has occured.";
	}
}inv_lnk;

class Validator {
public:
	bool ValidName(std::string param)
	{
		if (param.length() > 0)
			return true;
		return false;
	}

	bool ValidNumber(int nr)
	{
		if(nr>0)
			return true;
		return false;
	}

	bool ValidLink(std::string zelda)
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

class PetUI {
public:
	PetUI::PetUI(PetController *ctrl)
	{
		this->ctrl = ctrl;
		this->valid = Validator();
	}

	PetUI::~PetUI()
	{
		delete this->ctrl;
	}

	void PetUI::start()
	{
		printf("Welcome!!\n");
		while (1)
		{
			std::cout << "\nPlease type 1 for admin and 2 for user!\n";
			int command = readIntegerNumber("\nInput command: ");
			while (!(command >= 0 && command <= 2))
			{
				printf("Please input a valid command!\n");
				command = readIntegerNumber("Input command: ");
			}
			if (command == 0)
				return;
			switch (command)
			{
			case 1:
			{
				this->adminUI();
				break;
			}
			case 2:
			{
				this->userUI();
				break;
			}
			}
		}
	}

protected:
	PetController* ctrl;
	Validator valid;

	void PetUI::adminUI()
	{
		while (1)
		{
			printAdminMenu();
			int command = readIntegerNumber("\nInput command: ");
			while (validCommand(command) == 0)
			{
				printf("Please input a valid command!\n");
				command = readIntegerNumber("Input command: ");
			}
			if (command == 0)
				return;
			try
			{
				switch (command)
				{
				case 1:
				{
					int res = this->addUI();
					if (res == 1)
						printf("Dog successfully added.\n");
					else
					{
						if (res == 0)
							printf("Dog already exists. Nothing changed.\n");
						else
							printf("Error adding dog.");
					}
					break;
				}
				case 2:
				{
					this->listAllUI(this->ctrl);
					break;
				}
				case 3:
				{
					int res = this->updateUI();
					if (res == 1)
						printf("Dog successfully updated.\n");
					else
					{
						if (res == 0)
							printf("Dog does not exist. Nothing changed.\n");
						else
							printf("Error updating dog.");
					}
					break;
				}
				case 4:
				{
					int res = this->deleteUI();
					if (res == 1)
						printf("Dog successfully deleted.\n");
					else
						printf("Dog does not exist.\n");
					break;
				}
				case 5:
				{
					this->listLightUI(this->ctrl);
					break;
				}
				case 6:
				{
					this->ctrl->display();
					break;
				}
				}
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	}

	void PetUI::userUI()
	{
		while (1)
		{
			printUserMenu();
			int command = readIntegerNumber("\nInput command: ");
			while (validCommand(command) == 0)
			{
				printf("Please input a valid command!\n");
				command = readIntegerNumber("Input command: ");
			}
			if (command == 0)
			{
				return;
			}
			try
			{
				switch (command)
				{
				case 1:
				{
					Pet doggo = this->ctrl->AdoptionMode(1);
					char command;
					std::string litere;
					while (true)
					{
						if (doggo == Pet{})
						{
							this->ctrl->save_adopted();
							break;
						}
						ShellExecuteA(NULL, NULL, "iexplore.exe", doggo.getPhotograph().c_str(), NULL, SW_SHOWMAXIMIZED);
						std::cout << doggo << '\n';
						std::cout << "Adopt this dog?\n y for YES, n for NO, e for EXIT\n";
						std::cin >> command;
						if (command == 'e')
						{
							this->ctrl->save_adopted();
							break;
						}
						else
						{
							if (command == 'y')
								doggo = this->ctrl->AdoptionMode(2);
							else
							{
								if (command == 'n')
									doggo = this->ctrl->AdoptionMode(3);
							}
						}
					}
					break;
				}
				case 2:
				{
					char breed[33];
					int age;
					std::cout << "Desired breed?\n";
					std::cin >> breed;
					std::cout << "Age less than?\n";
					std::cin >> age;
					if (valid.ValidNumber(age) == false)
					{
						printf("Invalid age!");
						throw inv_nr;
					}

					Pet doggo = this->ctrl->CustomAdoptionMode(1, breed, age);
					char command;
					std::string litere;
					while (true)
					{
						if (doggo == Pet{})
						{
							this->ctrl->save_adopted();
							break;
						}
						ShellExecuteA(NULL, NULL, "iexplore.exe", doggo.getPhotograph().c_str(), NULL, SW_SHOWMAXIMIZED);
						std::cout << doggo << '\n';
						std::cout << "Adopt this dog?\n y for YES, n for NO, e for EXIT\n";
						std::cin >> command;
						if (command == 'e')
						{
							this->ctrl->save_adopted();
							break;
						}
						else
						{
							if (command == 'y')
								doggo = this->ctrl->CustomAdoptionMode(2, breed, age);
							else
							{
								if (command == 'n')
									doggo = this->ctrl->CustomAdoptionMode(3, breed, age);
							}
						}
					}
					break;
				}
				case 3:
				{
					this->listAllAdoptedUI(this->ctrl);
					this->ctrl->display_adopted();
					break;
				}
				}
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	}

	void PetUI::printAdminMenu()
	{
		printf("\n**********************************************************\n");
		printf("1 - add a dog.\n");
		printf("2 - list all dogs.\n");
		printf("3 - update a dog.\n");
		printf("4 - delete a dog.\n");
		printf("5 - show dogs lighter than a given weight.\n");
		printf("6 - to display the current repo.\n");
		printf("0 - to exit.\n");
		printf("\n**********************************************************\n");
	}

	void PetUI::printUserMenu()
	{
		printf("\n**********************************************************\n");
		printf("1 - choose dogs to adopt.\n");
		printf("2 - choose dogs to adopt (only dogs of a given breed younger than a given age).\n");
		printf("3 - display current adoption list.\n");
		printf("0 - to exit.\n");
		printf("\n**********************************************************\n");
	}

	int PetUI::validCommand(int command)
	{
		if (command >= 0 && command <= 6)
			return 1;
		return 0;
	}

	int PetUI::readIntegerNumber(const char* message)
	{
		char s[21];
		int res = 0;
		int flag = 0;
		int r;

		while (flag == 0)
		{
			printf(message);
			scanf("%20s", s);

			r = sscanf(s, "%d", &res);	// reads data from s and stores them as integer, if possible; returns 1 if successful
			flag = (r == 1);
			if (flag == 0)
				printf("Error reading number!\n");
		}
		return res;
	}

	int PetUI::addUI()
	{
		std::string breed, name, photograph;
		int age, weight;
		std::cout << "Input the following: breed, name, age, photograph and weight and press enter after each of them!\n";
		std::cin >> breed >> name >> age >> photograph>> weight;
		if (valid.ValidNumber(age) == false || valid.ValidNumber(weight)== false)
		{
			printf("Age AND weight must be positive! ");
			throw inv_nr;
		}
		if (valid.ValidLink(photograph) == false)
		{
			throw inv_lnk;
		}
		return this->ctrl->add(breed, name, age, photograph, weight);
	}

	int PetUI::updateUI()
	{
		std::string breed, name, photograph;
		int age, weight;
		std::cout << "Input the following: breed, name, age, weight and photograph and press enter after each of them!\n";
		std::cin >> breed >> name >> age >> weight >> photograph;
		if (valid.ValidNumber(age)== false)
		{
			printf("Age must be positive! ");
			throw inv_nr;
		}
		return this->ctrl->update(breed, name, age, photograph, weight);
	}

	int PetUI::deleteUI()
	{
		std::string photograph;
		std::cout << "Please input the photograph: ";
		std::cin >> photograph;
		return this->ctrl->del(photograph);
	}

	void PetUI::listAllUI(PetController *ctrl)
	{

		int length = ctrl->getLength();

		if (length == 0)
		{
			char* str = "There are no stored dogs.";
			printf("%s \n", str);
		}

		for (int i = 0; i < length; i++)
			std::cout << this->ctrl->getPetOnPos(i) << '\n';
	}

	void PetUI::listAllAdoptedUI(PetController *ctrl)
	{

		int length = ctrl->getAdoptionLength();

		if (length == 0)
		{
			char* str = "There are no stored dogs.";
			printf("%s \n", str);
		}

		for (int i = 0; i < length; i++)
			std::cout << this->ctrl->getAdoptedPetOnPos(i) << '\n';
	}

	void PetUI::listLightUI(PetController *ctrl)
	{
		int weight;
		printf("Please input the weight: ");
		scanf("%d", &weight);
		if (valid.ValidNumber(weight) == false)
		{
			printf("Invalid weight!");
			throw inv_nr;
		}
		PetRepo res = this->ctrl->filterByWeight(weight);
		int i;
		for (i = 0; i < res.getLength(); i++)
			std::cout << res.getPetOnPos(i) << '\n';
	}

};