#include <QtWidgets/QApplication>
#include "choice.h"
#include <Windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	choice c{};
	c.show();

	return a.exec();
}
