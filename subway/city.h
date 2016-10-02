#pragma once
#include "line.h"
#include <iostream>
#define Max 10000
#define Max_Sta 500
static void Dijkstra(int n, int v, int *dist, int *prev, int c[Max_Sta][Max_Sta]);



class City
{
public:
	City(char *pathfile);
	void setvlink(int a, int b);
	void setvlink_c(int a, int b);
	void addline(Line *line);
	int str2id(char *s);
	char *id2str(int i);
	char *id2line(int i);
	int linked(char *s1, char *s2);
    int connected(int id1,int id2);
	int *findway(char *s1, char *s2, int type);
	int *shortestway(char *s1,char *s2);
	int *least_transfer(char *s1, char *s2);
	void printline(char *s);
	int getlinenum();
	int getstanum();
	int *getline(int i);
	Point getloc(int i);
private:
	int useful;
	char name[30];
	int stanum;
	int linenum;
	int vstanum;
	int v2pandl[Max_Sta][2];
	int vlink[Max_Sta][Max_Sta];
	int vlink_c[Max_Sta][Max_Sta];
	Line *lines[50];
	Station *stas[Max_Sta];
};
