#pragma once

struct Point {
	int x;
	int y;
};

class Station
{
public:
	Station(char *n, int i,int x,int y);
	void addvid(int vi);
	char *getname();
	int getid();
	int getvid(int i);
	int getvnum();
	Point getloc();
private:
	char name[30];
	int id;
	int vid[10];
	int vnum=0;
	Point loc;
};