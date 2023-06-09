#pragma once;
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define TAP_WIDTH 75
#define TAP_HEIGHT 20
#define Q 113
#define E 101
#define F 102
#define W 119
#define A 97
#define D 100
#define S 115

struct virusTap {
	int propagation[5];
	int fatality[5];
	int day; // time 관련
	int hour; // time 관련
	int min; // time 관련
	int gold;
	int goldup;
};
struct shipTap {
	int power;
	int health;
	int speed;
};
struct routeTap {
	int animal;
	int blood;
	int air;
	int water;
	int port;
};
struct itemTap {
	int oil;
	int jamming;
	int toground;
	int engine;
	int hostage;
};
struct virusTap vt;
struct shipTap st;
struct routeTap rt;
struct itemTap it;
int itemList[5];    // 아이템 개수 저장 
int currentBlood;

void removeTap();
void initTap();
void drawBox(int fstPosX, int fstPosY, int width, int height);
void colorChangeVirusTap(int i, int j);
void colorChangeRouteTap(int i);
void colorChangeShipTap(int i);
void colorChangeItemTap(int i);
void selectedTap(int i);
int drawFirstTap();
void printTime(); // time 관련
void printGold();
void makeExplane();
void printUpEx();
void printLogEx();
void printuserInformation();