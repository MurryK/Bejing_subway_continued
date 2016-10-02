#pragma once
#include "station.h"
class Line
{
public:
	Line(char *n);
	void addsta(Station *sta);
	int getstanum();
	int idexist(int id);
	char *getname();
	int *getstas();
private:
	char name[30];
	Station *stas[50];
	int stanum;
};