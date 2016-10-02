#pragma once
#include "line.h"
#define Max 10000
#define Max_Sta 500
#define Max_Line 100
static void Dijkstra(int n, int v, int *dist, int *prev, int c[Max_Sta][Max_Sta]);



class City
{
public:
	City(char *pathfile);
	City();
	int readmap(char *path);
	int str2id(char *s);
	char *id2str(int i);
	char *id2line(int i);
	int linked(char *s1, char *s2);
	int linked(int id1, int id2);
	int *findway(char *s1, char *s2, int type);
	int *shortestway(char *s1, char *s2);
	int *least_transfer(char *s1, char *s2);
	int getlinenum();
	int getstanum();
	int *getline(int i);
	Point getloc(int i);
	int sta2line(int i);
private:
	int useful=0;
	char name[30];
	int stanum=0;
	int linenum=0;
	int vstanum=0;
	int v2pandl[Max_Sta][2];
	int vlink[Max_Sta][Max_Sta];
	int vlink_c[Max_Sta][Max_Sta];
	Line *lines[Max_Line];
	Station *stas[Max_Sta];
};


class Errortype
{
public:
	Errortype::Errortype(int i)
	{
		this->type = i;
	}
	int get() {
		return type;
	}
private:
	int type;
};
