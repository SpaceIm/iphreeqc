#include <iostream>
#include <IPhreeqc.hpp>

int main(int argc, char* argv[])
{
	IPhreeqc obj;
	obj.SetOutputOn(true);
	if (obj.LoadDatabase("phreeqc.dat"))
	{
		goto error;
	}
	if (obj.RunFile("ex1"))
	{
		goto error;
	}
	std::cout << "Ok\n";
	return 0;
error:
	std::cout << obj.GetLastErrorString();
	return 1;
}

