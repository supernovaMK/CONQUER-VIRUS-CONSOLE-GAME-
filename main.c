#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
#include <mmsystem.h> // bgm
#include <Digitalv.h> // bgm
#include "data.h"
#include "worldmap.h"
#include "makeTap.h"

#pragma warning(disable:4996)
#pragma comment(lib, "winmm.lib") // bgm

#define WORLD_WIDTH 77      //���� ũ��
#define WORLD_HEIGHT 39      //���� ����

/*�Լ�*/
void SetCurrentCursorPos(int x, int y);      //���� Ŀ�� ��ġ ���� �Լ�
COORD GetCurrentCursorPos();   //Ŀ�� ��ġ �������� �Լ�
void MakeWorld();      //�� �����  
void RemoveCursor();      //Ŀ�� �����
void Colorset(int backColor, int textColor);      //��Ʈ�� ����(���ڹ���, ���ڻ�)
void SummonVaccine(int type);       //��� ��ȯ�Լ�
void ShowShip(SHIP* nowShip);      //�踦 �����ִ� �Լ�
void DeleteShip(SHIP* nowShip);      //�踦 ����� �Լ�
void ProcessKeyInput();         //Ű�Է�
void vMove(SHIP* nowShip);     //���̷����� �����̴� �Լ�
void VirusShipSpawn(SHIP* node);        //���̷��� �� ����
int detectCollision(int posX, int posY, SHIP* nowship);     //�浹 ���� Ȯ���Լ�
void AddShip();         //�� ���� �Լ�
void AddShipInMap(SHIP* nowShip);       //�� �ʿ� �����ϴ� �Լ�
void crashShip(SHIP* nowShip, SHIP* targetShip);        //�谡 �ε�����
SHIP* brokenShip(SHIP* nowShip);            //�谡 �ν�������
void dropItem();            //������ ����Ʈ���� �Լ�
void AddRoute(int push, SHIP* nowShip);     //�ε��� �� ��Ʈ �����ϴ� �Լ�
int goalCheck(SHIP* nowShip);           //���谡 ��ǥ������ �������� �Ҹ��� �Լ�
void updateTime();      //�ð� ������Ʈ �Լ�
void MakeShip();        //ȭ�鿡 �� �����ϴ� �Լ�
void TapBreakingShip();     //�ǿ��� �� ���� �μ��� �Լ�
void ReBuildShip();         //�ǿ��� �� ���� �츮�� �Լ�
void healing();             //�� ü�� ȸ���ϴ� �Լ�
void breakEffect(COORD pos);    //�ν����� ����Ʈ
void goalEffect(COORD pos);    //�ν����� ����Ʈ
void makeitem(int p);           //������ ����� �Լ�
void updatePeople();    // �α� �� ������Ʈ �Լ�
void showPeopleBar();     // �α� ���� �� ���� �Լ�
void showVaccineBar();    // ��� �ϼ��� �� ���� �Լ�
void printPer();          // �� �� �ۼ�Ƽ�� ��� �Լ�
void showBar();           // �� ���� ���
void updateBar();         // �� ������Ʈ �Լ�
void updateMap();           //�������� ���� �� ������Ʈ�ϴ� �Լ�
void virusUpCal(int i, int j, int maxPeople);       //������ ���
void virusUp();             //������ �ø�
void breakPlayer();         //�÷��̾� ü�� 0�϶� ���ִ� �Լ�
void showStartScreen();         // ���� ȭ�� ��� �Լ�
void removeStartScreen();       // ���� ȭ�� ���� �Լ�
void dieShip();             //�� ���̴� �Լ�
void itemUse();         //������ ����ϴ� �Լ�
void updateMapVirus();
void bloodRouteUpgrade();        // ������� ���� ���׷��̵� ��
void logIn(char ch[30]);
int endCheck();
void showEndingScreenWin();
void showEndingScreenLose();

/*bgm ���� �Լ�*/  // .wav �˻��ؼ� ���� �̸� Ȯ���� ��!!

void MainSound(int bgmFlag); // bgmFlag == 0�� ��(��� ��ȣ��) ���
void playSoundBreak(); // �� �μ����� ȿ����
void playSoundItem(); // ������ �Դ� ȿ����
void playSoundCollision(); // virus ship collision ��
void playSoundSinking(); // virus ship �ļ� (loading)
void playSoundHealing(); // healing ȿ����
void playSoundWarning(); // warning ȿ���� (�� ����, Ȥ�� �������� up)
void playSoundLog(); // Log ȿ����

/*��� �� ���� ��ũ�帮��Ʈ �Լ�*/
void InitList();                    //��ũ�� ����Ʈ ����
void InsertAfter(int PosSet, int _type, SHIP* node);        //��ũ�� ����Ʈ�� ������ �ֱ�
SHIP* DeleteNode(SHIP* node);               //��� ����
/*��*/

/*��������*/

currentTap = 0;                     //���� �� ����
COORD spawnSet[8] = { {94,22}, {12,11}, {32,21},{128,29}, {140,9}, {80, 32} , {62,3},{46,19} };      //������(������ 1 -> ������ 1)
COORD VirusSpawn = { 2,1 };             //���̷��� ���� ��ġ
int totalSpeed = 5;                 //��ü �ӵ�
int virusSpeed = 5;     //���̷��� ��ӵ�(vt.min % virusSpeed >= 0 && vt.min % virusSpeed < 2)

int deadPeople = 0;              // ���� �α�
int peoplePer = 0;               // �ٿ� �ݿ� + ����� �α� �ۼ�Ƽ��
int vaccinePer = 0;              // �ٿ� �ݿ� + ����� ��ſϼ��� �ۼ�Ƽ��
int die = 0;                    //�׾��°� Ȯ��(������� 0, �׾��� 1)
int dieTime = 0;                //���� �ð� Ȯ��(Ȯ���� ������ ����

/*�÷��̾� �� ���׷��̵� ǥ*/
int upVirusSpeed[5] = { 5,4,3,2,1 };     //�ӵ� ���� �� ����
int upVirusHealth[10] = { 3,5,7,10,13,15,17,20,20,20 };         //ü�� ������ ����
int upVirusPower[10] = { 1,1,1,1,1,2,2,2,2,2 };         //�� ������ ����
int upVirusHealing[5] = { 1,2,3,4,5 };              //�����忡�� ȸ���Ǵ� �ӵ� ������ ����
int goldUp = 50;                //���̷��� �� ����� �� ��� ���
int healcnt = 0;                //ȸ���Ҷ� ����ϴ� ī��Ʈ
int healingPoint = 1;               //�����忡�� ȸ���ϴ� ��

itemwork[5] = { 0,0,0,0,0 };            //������ ������ΰ� Ȯ��
itemstart0 = 0;             //������ ����� �ð�
itemstart1 = 0;             //������ ����� �ð�
itemstart2 = 0;             //������ ����� �ð�
itemstart3 = 0;             //������ ����� �ð�
itemstart4 = 0;             //������ ����� �ð�

/*���׷��̵� ���ǥ*/
goldSpeed[5] = { 50, 150, 250, 350, 450 };          //�ӵ� ���׷��̵忡 ����ϴ� ���
goldHealth[10] = { 25, 50, 100, 125, 150, 175, 200, 250, 250, 250 };       //ü�� ���׷��̵忡 ����ϴ� ���
goldPower[10] = { 25, 50, 100, 125, 150, 175, 200, 250, 250, 250 };     //�� ���׷��̵忡 ����ϴ� ���
goldWater[5] = { 100,300, 500, 700 };     //�� ���׷��̵忡 ����ϴ� ���
goldHealing[5] = { 50, 100, 200, 350, 500 };            //������ ���׷��̵忡 ����ϴ� ���
goldPropagation[2] = { 500, 1000 };
goldFatality[2] = { 1000,2000 };
goldAnimal = 500;
goldBlood = 500;
goldAir = 500;
goldGoldup[2] = { 300,800 };    // ��ȭȹ�淮 ���׷��̵� ���
//goldAir[] = 0;

/*���� ���� ����*/
int moveRoute[8][200] = { {0,1,1,1,1,1,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,3},//1��������
                        {0,1,1,1,-2,-2,-2,-2,1,1,1,1,1,2,-1,-1,2,2,2,1,1,1,2,1,1,1,1,2,2,2,2,-1,-1,1,1 - 1,-2,-1,-1,2,2,2,2,2,2,3},//2��������
                        {0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-2,-2,3},
                        {0,2,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,1,1,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,2,2,2,2,2,3},
                        {0,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,3},
                        {0,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3},
                        {0,-1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-2,-1,-1,-2,-1,-1,3},
                        {0,2,2,2,2,2,2,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-2,-2,3} };

int stageEnemy[][4] = { {40,0,0,0},      //�������� �� �� ���� ����
                        {60,7,0,0},
                        {80,15,2,0},
                        {110,20,4,0} };
int stageSpeed[][4] = { {30,15,30,0},  //�������� �� �� �ӵ�(1~100)������ ����
                        {45,20,45,0},
                        {50,25,50,0},
                        {65,30,65,0} };
int stageHealth[][4] = { {2,10,5,1 },  //�������� �� �� ü��(1~100)������ ����
                        {3,15,7,1},
                        {5,20,9,1},
                        {7,30,13,1} };
int stagePower[][4] = { {2, 1, 4, 20 },  //�������� �� �� �Ŀ�(1~100)������ ����
                        {3, 2, 6, 20},
                        {5, 2, 8, 20},
                        {7, 3, 10, 20} };

int sumVaccinecnt = 0;              //��Ź谡 �󸶳� �����Ǿ��°�(updatetime������ ���� ������ �����)
int sumTruckcnt = 0;                //ȭ������ �󸶳� �����Ǿ��°�
int sumMilicnt = 0;             //������ �󸶳� �����Ǿ��°�
int sumBombcnt = 0;         //��ڰ� �󸶳� �����Ǿ��°�
int milinext = 0;           //������ ��Ź�� �Բ� �����Ҷ� ī��Ʈ

int stage = 0;              //���� ��������

/*�� ���� ����*/
currentvt = 0;
currentst = 0;
currentrt = 0;
currentit = 0;
check = 0;
logcnt = 0;

/*���̷��� ���� ����*/
int mapVirus[2][5] = { {0,0,0,0,0},     //����� ���� �α� (�����ƴϾ� > ������ī > ���Ƹ޸�ī> �ϾƸ޸�ī > ����þ�)
                       {43,98,74,241,366} };    //����� �α� ����
int allMap = 827;
int totalPeople = 10000;            // ��ü �α�(�ӽ�)
int infectedPeople = 0;          // ������ �α�
int currentWaterLv = 0; // ������� �� ���׷��̵� ���� ����
int virusUpCheck = 0;
int virusUpCnt = 1;

currentBlood = -1; // ������� ���� ���� �� ������ ���� ����
nowShiphealth = 121;

int nowOse = 0;
int nowAfr = 0;
int nowSAme = 0;
int nowNAme = 0;
int nowEA = 0;

void main()
{
    COORD bufferSize = { 100, 10000 }; // ����, ����
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
    vt.gold = 1000;
    //vt.min += 14300;
    //vt.health = 0; // �ӽ�
    system("mode con cols=200 lines=60");      //�ܼ� ũ�� ����
    SetConsoleTitle(TEXT("Conqure Us"));        //�ܼ� �̸� ����
    while (_kbhit() == 0)
    {
        showStartScreen(); // �ӽ÷� press any key
        Colorset(black, white);
        SetCurrentCursorPos(82, 40);
        printf("///PRESS ANY KEY TO START///");
    }
    removeStartScreen();
    InitList();
    initTap();
    RemoveCursor();
    MakeWorld();
    MakeShip();
    printTime();
    printGold();
    drawFirstTap();
    makeExplane();
    VirusShipSpawn(head);       // ���̷����� ����Ʈ�� �߰�
    showBar();                  // �� ��� �Լ�
    printPer();                 // �ۼ�Ƽ�� ��� �Լ�
    srand((unsigned int)time(NULL));        //����
    int nowstage = 0;
    int cnt = 0;
    logIn("������ ���۵Ǿ����ϴ�.");
    logIn("---------------- STAGE: 1----------------");

    PlaySound(TEXT("Mainbgm.wav"), NULL, SND_ASYNC | SND_LOOP);

    while (nowstage < 4) {// ���� 4�ε� �ӽ�
        while (nowstage == stage) {
            printLogEx();
            ProcessKeyInput();
            healing();
            SHIP* tmp = head->next;
            for (int k = 1; k < size; k++) {
                tmp = tmp->next;
                if (tmp->type < 0 || tmp->type > 4) {
                    tmp = head->next;
                    continue;
                }
                if (100 < tmp->speedCnt) {       //�ӵ� ����(�踶�� �ӵ��� ���)
                    vMove(tmp);         //�ӵ��� ������ ������
                    tmp->move = tmp->moveRoute[tmp->movecnt++];    //������ȯ (��Ʈ��� ����)
                    if (!goalCheck(tmp)) {      //������ ������ 0
                        tmp = brokenShip(tmp);
                        continue;       //���� �Լ� �θ�
                    }
                    tmp->speedCnt = tmp->speedCnt % 100;
                }
                tmp->speedCnt += tmp->speed;            //�ӵ� ī��Ʈ ����
                if (tmp->health <= 0) {
                    tmp = brokenShip(tmp);
                    continue;
                }
                dieShip();
                itemUse();
            }
            SetCurrentCursorPos(0, 0);
            updateMap();
            if (endCheck() > 0) {
                system("cls");
                if (endCheck() == 1)
                    showEndingScreenWin();
                else
                    showEndingScreenLose();
                getchar();
                return 0;
            }
        }
        nowstage++;
        switch (nowstage) {
        case 1: logIn("---------------- STAGE: 2----------------"); break;
        case 2: logIn("---------------- STAGE: 3----------------"); break;
        case 3: logIn("---------------- STAGE: 4----------------"); break;
        }

    }
    system("cls");
    showEndingScreenLose();
    getchar();
    return 0;
}

void SetCurrentCursorPos(int x, int y) {         //Ŀ�� ��ġ ����
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos(void) {         //���� Ŀ�� ��ġ �θ���
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    return curInfo.dwCursorPosition;
}


void MakeWorld() {                        //�� ����� �Լ�
    COORD curPos = { 0,0 };
    for (int i = 2; i < WORLD_HEIGHT - 1; i++) {
        for (int j = 1; j < WORLD_WIDTH - 1; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            if (World[i][j] == 6) {
                Colorset(green, green);
                printf("��");
            }
            else if (World[i][j] == 5) {
                Colorset(lightBlue, lightBlue);
                printf("��");
            }
            else if (World[i][j] == 7) {
                Colorset(Orange, Orange);
                printf("��");
            }
            else if (World[i][j] == 8) {
                Colorset(green, brightRed);
                printf("��");
            }
            else if (World[i][j] == 9) {
                Colorset(brightYellow, red);
                printf("��");
            }
        }
    }
}

void RemoveCursor(void) {            //Ŀ�� �����
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void Colorset(int backColor, int textColor) {         //�� ��ȯ �Լ�
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Handle, (backColor << 4) + textColor);
}

void SummonVaccine(int type) {      //��Ź� ��ȯ
    switch (type) {
    case 0: logIn("������ ��Ź谡 �����˴ϴ�."); break;
    case 1: logIn("����� ȭ������ �����˴ϴ�."); break;
    case 2: logIn("ȸ�� ������ �����˴ϴ�."); break;
    case 3: logIn("��ڰ� �����˴ϴ�."); break;
    }
    SHIP* nowShip = head;       //���⼭ ó�� �� ���� �־�

    int PosSet = rand() % 8;     //��ȯ��� ����
    for (int i = 0; i < size; i++) {
        nowShip = nowShip->next;
    }
    InsertAfter(PosSet, type, nowShip);
}

void ShowShip(SHIP* nowShip) {
    COORD curPos = GetCurrentCursorPos();
    AddShipInMap(nowShip);      //�踦 �ٴٿ� ����
    switch (nowShip->type) {
    case 0: Colorset(lightBlue, red); break;
    case 1: Colorset(lightBlue, Orange); break;
    case 2: Colorset(lightBlue, gray); break;
    case 3: Colorset(lightBlue, red); break;
    case 4: Colorset(lightBlue, white); break;
    }
    switch (nowShip->move) {
    case 1:printf("��");  break;
    case -1:printf("��"); break;
    case 2:printf("��");  break;
    case -2:printf("��"); break;
    case 0: printf("��"); break;
    }
    SetCurrentCursorPos(curPos.X, curPos.Y);
}

void DeleteShip(SHIP* nowShip) {
    AddShipInMap(nowShip);      //�踦 �ٴٿ��� ����
    COORD curPos = GetCurrentCursorPos();
    Colorset(lightBlue, lightBlue);
    printf("��");
    SetCurrentCursorPos(curPos.X, curPos.Y);
}


void InitList() {       //��ũ�帮��Ʈ ����
    head = (SHIP*)malloc(sizeof(SHIP));      //�Ӹ� �޸� �Ҵ�
    tail = (SHIP*)malloc(sizeof(SHIP));      //���� �޸� �Ҵ�
    head->next = tail;            //�Ӹ� ������ ����
    tail->next = tail;            //���� ������ ����
    size = 0;               //������� 0
}

void InsertAfter(int PosSet, int _type, SHIP* node) {
    if (node == tail)            //���� �ڿ� ���� �Ҽ� ����.
        return;
    SHIP* newNode = (SHIP*)malloc(sizeof(SHIP));   //�޸� �Ҵ�
    newNode->startPos = spawnSet[PosSet];            //���� �־��ش�   
    newNode->type = _type;
    newNode->move = 0;
    newNode->next = node->next;         //Ư����尡 ����Ű�� ��带 ���� ��� ����Ű��
    newNode->movecnt = 0;
    newNode->speedCnt = 0;
    newNode->speed = stageSpeed[stage][_type];
    for (int i = 0; i < sizeof(moveRoute[PosSet]) / sizeof(int); i++) {
        newNode->moveRoute[i] = moveRoute[PosSet][i];
    }
    switch (_type) {
    case 0:     //�Ϲ� ��Ź�
        newNode->health = stageHealth[stage][0];
        newNode->power = stagePower[stage][0];
        break;
    case 1:     //ȭ����
        newNode->health = stageHealth[stage][1];
        newNode->power = stagePower[stage][1];
        break;
    case 2:     //����
        newNode->health = stageHealth[stage][2];
        newNode->power = stagePower[stage][2];
        break;
    case 3:     //���
        newNode->health = stageHealth[stage][3];
        newNode->power = stagePower[stage][3];
        break;
    }
    AddShipInMap(newNode);
    node->next = newNode;            //Ư����尡 ���� ����Ű�� �Ѵ�.
    ++size;                  //ũ�⸦ �ϳ� �÷��ش�.
}

void VirusShipSpawn(SHIP* node) {
    SHIP* newNode = (SHIP*)malloc(sizeof(SHIP));
    newNode->startPos.X = 2;
    newNode->startPos.Y = 2;
    newNode->endPos.X = -1;
    newNode->endPos.Y = -1;
    newNode->type = 4;
    newNode->move = node->move;
    newNode->next = node->next;
    newNode->maxhp = 10;
    newNode->health = 10;
    newNode->power = 1;
    AddShipInMap(newNode);
    node->next = newNode;
    ++size;
}


SHIP* DeleteNode(SHIP* node)
{
    if (node == head || tail == node)   //�Ӹ��� ������ ����� ����.
        return 0;
    SHIP* preNode = head;         //���� ������� ������ ������ ���� �ʾƼ� ���� ����ؾ��Ѵ�.
    SHIP* temp = head->next;      //ã�� ���
    while (temp != tail)               //����Ʈ�� ������ ����.
    {
        if (node == temp)            //���� ��带 ã�Ҵٸ�
        {
            SHIP* deleteNode = node;      //���� ��带 ����Ѵ�.
            preNode->next = deleteNode->next;   //���� ��尡 ���� ����� ������ ����Ű���Ѵ�.
            free(deleteNode);         //���� ��带 �޸� ����
            --size;               //ũ�⸦ �ٿ��ش�.
            temp = preNode->next;
            return temp;
        }
        preNode = temp;         //�� ã������
        temp = temp->next;      //���� ����
    }
    return temp;
}

void vMove(SHIP* nowShip) {//���̷��� �� �̵��Լ�
    SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
    DeleteShip(nowShip);
    SHIP* targetShip = head;
    int mNext;  //���� �����ӿ��� �浹�Ҷ� ������ �浹�ϴ��� Ȯ���ϴ� ����
    COORD now = nowShip->startPos;
    switch (nowShip->move) {
    case 1:
        mNext = detectCollision(nowShip->startPos.X, nowShip->startPos.Y + 1, nowShip);
        if (mNext == 0)     //�浹 ������ ���
            nowShip->startPos.Y++;
        else if (mNext == 1) {    //�÷��̾�谡 ��Ź�� �ε������
            while (nowShip->startPos.X != targetShip->startPos.X || nowShip->startPos.Y + 1 != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*�÷��̾�� �����̱�*/
            if (World[targetShip->startPos.Y + 1][targetShip->startPos.X / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.Y += 1;
                AddRoute(-1, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*����*/
            if (World[nowShip->startPos.Y - 1][nowShip->startPos.X / 2] == 5) {
                nowShip->startPos.Y--;
                AddRoute(1, nowShip);
            }
            nowShip->movecnt -= 1;
        }
        break;
    case -1:
        mNext = detectCollision(nowShip->startPos.X, nowShip->startPos.Y - 1, nowShip);
        if (mNext == 0)
            nowShip->startPos.Y--;
        else if (mNext == 1) {    //�÷��̾�谡 ��Ź�� �ε������
            while (nowShip->startPos.X != targetShip->startPos.X || nowShip->startPos.Y - 1 != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*�÷��̾�� �����̱�*/
            if (World[targetShip->startPos.Y - 1][targetShip->startPos.X / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.Y -= 1;
                AddRoute(1, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*����*/
            if (World[nowShip->startPos.Y + 1][nowShip->startPos.X / 2] == 5) {
                nowShip->startPos.Y++;
                AddRoute(-1, nowShip);
            }
            nowShip->movecnt -= 1;
        }
        break;
    case 2:
        mNext = detectCollision(nowShip->startPos.X + 2, nowShip->startPos.Y, nowShip);
        if (mNext == 0)
            nowShip->startPos.X += 2;
        else if (mNext == 1) {    //�÷��̾�谡 ��Ź�� �ε������
            while (nowShip->startPos.X + 2 != targetShip->startPos.X || nowShip->startPos.Y != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*�÷��̾�� �����̱�*/
            if (World[targetShip->startPos.Y][(targetShip->startPos.X + 2) / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.X += 2;
                AddRoute(-2, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*����*/
            if (World[nowShip->startPos.Y][(nowShip->startPos.X - 2) / 2] == 5) {
                nowShip->startPos.X -= 2;
                AddRoute(2, nowShip);
            }
            nowShip->movecnt -= 1;
        }
        break;
    case -2:
        mNext = detectCollision(nowShip->startPos.X - 2, nowShip->startPos.Y, nowShip);
        if (mNext == 0)
            nowShip->startPos.X -= 2;
        else if (mNext == 1) {    //�÷��̾�谡 ��Ź�� �ε������
            while (nowShip->startPos.X - 2 != targetShip->startPos.X || nowShip->startPos.Y != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*�÷��̾�� �����̱�*/
            if (World[targetShip->startPos.Y][(targetShip->startPos.X - 2) / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.X -= 2;
                AddRoute(2, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*����*/
            if (World[nowShip->startPos.Y][(nowShip->startPos.X + 2) / 2] == 5) {
                nowShip->startPos.X += 2;
                AddRoute(-2, nowShip);
            }
            nowShip->movecnt -= 1;

        }
        break;
    }
    if (now.X == nowShip->startPos.X && now.Y == nowShip->startPos.Y && nowShip->type < 4 && nowShip->move != 0) {
        AddRoute(nowShip->moveRoute[nowShip->movecnt], nowShip);
    }
    SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
    ShowShip(nowShip);
}
void ProcessKeyInput() {//Ű �Է¹޴� �Լ�
    int key;

    for (int i = 0; i < 5; i++) {
        updateTime();
        showBar();
        printPer();
        virusUp();
        /*SetCurrentCursorPos(0, 0);
        for (int i = 0; i < 5; i++) {
            printf("%d ", mapVirus[0][i]);
        }
        printf("\n%d %d", infectedPeople, deadPeople);
        */
        if (_kbhit() != 0) {
            key = _getch();
            switch (key) {
            case 75:
                if (vt.min % virusSpeed >= 0 && vt.min % virusSpeed <= 2 && die == 0) {
                    head->next->move = -2;
                    vMove(head->next);
                }
                break;
            case 77:
                if (vt.min % virusSpeed >= 0 && vt.min % virusSpeed <= 2 && die == 0) {
                    head->next->move = 2;
                    vMove(head->next);
                }
                break;
            case 72:
                if (vt.min % virusSpeed >= 0 && vt.min % virusSpeed <= 2 && die == 0) {
                    head->next->move = -1;
                    vMove(head->next);
                }
                break;
            case 80:
                if (vt.min % virusSpeed >= 0 && vt.min % virusSpeed <= 2 && die == 0) {
                    head->next->move = 1;
                    vMove(head->next);
                }
                break;
            case Q:
                check = 0;
                currentst = 0;
                currentrt = 0;
                currentvt = 0;
                currentit = 0;
                if (currentTap == 0) break;
                currentTap = (currentTap + 3) % 4;
                if (currentTap == 0) {
                    removeTap();
                    selectedTap(0);
                    break;
                }
                else if (currentTap == 1) {
                    selectedTap(1);
                    removeTap();
                    break;
                }
                else if (currentTap == 2) {
                    selectedTap(2);
                    removeTap();
                    break;
                }
                else if (currentTap == 3) {
                    selectedTap(3);
                    removeTap();
                    break;
                }
                break;
            case E:
                check = 0;
                currentst = 0;
                currentrt = 0;
                currentvt = 0;
                currentit = 0;
                if (currentTap == 3) break;
                currentTap = (currentTap + 1) % 4;
                if (currentTap == 0) {
                    removeTap();
                    selectedTap(0);
                    break;
                }
                else if (currentTap == 1) {
                    removeTap();
                    selectedTap(1);
                    break;
                }
                else if (currentTap == 2) {
                    removeTap();
                    selectedTap(2);
                    break;
                }
                else if (currentTap == 3) {
                    removeTap();
                    selectedTap(3);
                    break;
                }
                break;
                /*���׷��̵� ���ý� �Ͼ�� ��*/
            case F:
                if (currentTap == 0) {      //���̷��� ��ȭ ���ϰ��
                    if (check == 0) {
                        removeTap();
                        colorChangeVirusTap(0, currentWaterLv);
                        check = 1;
                        break;
                    }
                    if (currentvt == 0) {           //���̷��� ��ȭ �� �� ���ķ��� ���
                        check = 1;
                        if (vt.gold < goldPropagation[currentPropagation]) { break; }
                        for (int i = 0; i < 5; i++) {   // ���ķ� ���׷��̵��Ͽ� ���̷��� ���Ŀ� �ݿ�
                            if (vt.propagation[i] - rt.animal >= 2) { continue; }
                            vt.propagation[i] += 1;
                        }
                        if (currentPropagation > 2) { currentPropagation -= 1; }
                        if (currentPropagation >= 2) break;
                        logIn("���ķ��� ���׷��̵� �Ͽ����ϴ�.");
                        vt.gold -= goldPropagation[currentPropagation];
                        currentPropagation += 1;
                        printGold();
                        colorChangeVirusTap(0, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 1) {      //���̷��� ��ȭ �� �� ġ������ ���
                        check = 1;
                        if (vt.gold < goldFatality[currentFatality]) { break; }
                        for (int i = 0; i < 5; i++) {   // ġ���� ���׷��̵��Ͽ� ����ڼ��� �ݿ� / �����ڼ����� ����
                            if (currentBlood == 1 && vt.fatality[i] - rt.blood * 2 >= 2) { continue; }
                            if (currentBlood != 1 && vt.fatality[i] >= 2) { continue; }
                            vt.fatality[i] += 1;
                        }
                        if (currentFatality > 2) { currentFatality -= 1; }
                        if (currentFatality >= 2) break;
                        logIn("ġ������ ���׷��̵� �Ͽ����ϴ�.");
                        vt.gold -= goldFatality[currentFatality];
                        currentFatality += 1;
                        printGold();
                        colorChangeVirusTap(1, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 2) {
                        check = 1;
                        if (vt.gold < goldGoldup[vt.goldup]) { break; }
                        if (vt.goldup == 2) { break; }
                        vt.gold -= goldGoldup[vt.goldup];
                        vt.goldup += 1;
                        goldUp = goldUp * 2;
                        logIn("��ȭ ȹ�淮�� ���׷��̵� �Ͽ����ϴ�.");
                        printGold();
                        colorChangeVirusTap(2, currentWaterLv);
                        break;
                    }
                    break;
                }
                else if (currentTap == 1) {     //���� ��� ��ȭ���ϰ��
                    if (check == 0) {
                        removeTap();
                        colorChangeRouteTap(0);
                        check = 1;
                        break;
                    }
                    if (currentrt == 0) {       //������� ����
                        check = 1;
                        if (rt.animal > 1)  break;  // �� �� ���׷��̵� �������� MAX 1 / �� ���׷��̵� �� 0���� �ʱ�ȭ
                        if (goldAnimal > vt.gold) break;
                        rt.animal += 1;
                        vt.propagation[currentWaterLv]++; // ��� ���ķ� ��� / ���� ���׷��̵� ���� ������� ����
                        vt.gold -= goldAnimal;
                        printGold();
                        colorChangeRouteTap(0);
                        logIn("���� ���ĸ� ���׷��̵� �Ͽ����ϴ�.");
                        break;
                    }
                    else if (currentrt == 1) {  //������� ����
                        check = 1;
                        if (rt.blood > 1)  break;  // �� �� ���׷��̵� �������� MAX 1 / �� ���׷��̵� �� 0���� �ʱ�ȭ
                        if (goldBlood > vt.gold) break;
                        rt.blood += 1;
                        vt.gold -= goldBlood;
                        printGold();
                        bloodRouteUpgrade();                // �������� / ���� ���׷��̵� ���� ����� ġ���� �������� 0�ܰ�� �ϰ� �Ǵ� 2�ܰ� ���
                        colorChangeRouteTap(1);
                        logIn("���� ���ĸ� ���׷��̵� �Ͽ����ϴ�.");
                        if (currentBlood == 0) {
                            logIn("�������̷� ġ������ 0 ������ �Ǿ����ϴ�.");
                        }
                        else if (currentBlood == 1) {
                            logIn("�������̷� ġ������ 2 ���� ����մϴ�.");
                        }
                        break;
                    }
                    else if (currentrt == 2) {      //������� ����
                        check = 1;
                        if (rt.air == 1) break;
                        if (goldAir > vt.gold) break;
                        rt.air += 1;                // ���̷��� ���� �� �ݿ� ���� �߰�
                        vt.gold -= goldAir;
                        printGold();
                        colorChangeRouteTap(2);
                        logIn("��� ���ĸ� ���׷��̵� �Ͽ����ϴ�.");
                        break;
                    }
                    else if (currentrt == 3) {      //������� ��
                        check = 1;
                        if (rt.water == 4)  break;
                        if (vt.gold < goldWater[rt.water]) break;
                        vt.gold -= goldWater[rt.water];
                        rt.water += 1;
                        printGold();
                        vt.propagation[currentWaterLv] -= rt.animal;
                        rt.animal = 0;
                        if (currentBlood == 1) {
                            vt.fatality[currentWaterLv] -= rt.blood * 2;
                        }
                        currentBlood = -1;
                        rt.blood = 0;
                        if (currentTap == 1) {
                            colorChangeRouteTap(currentrt);
                        }
                        currentWaterLv = rt.water;
                        logIn("�� ��θ� ���׷��̵� �Ͽ����ϴ�.");
                        colorChangeRouteTap(3);
                        break;
                    }
                    else if (currentrt == 4) {      //������� ������
                        check = 1;
                        if (rt.port == 5)  break;
                        if (vt.gold < goldHealing[rt.port]) break;
                        healingPoint += upVirusHealing[rt.port];
                        vt.gold -= goldHealing[rt.port];
                        printGold();
                        rt.port += 1;
                        logIn("�������� ���׷��̵� �Ͽ����ϴ�.");
                        colorChangeRouteTap(4);
                        break;
                    }
                    break;
                }
                else if (currentTap == 2) { // �� ��ȭ
                    if (check == 0) {
                        colorChangeShipTap(0);
                        check = 1;
                        break;
                    }

                    if (currentst == 0) { // ���ݷ�
                        if (st.power == 10) break;
                        if (vt.gold < goldPower[st.power]) break;
                        check = 1;
                        SHIP* nowShip = head->next;
                        nowShip->power += upVirusPower[st.power];
                        vt.gold -= goldPower[st.power];
                        printGold();
                        st.power += 1;
                        logIn("�� ���ݷ��� ���׷��̵� �Ͽ����ϴ�.");
                        colorChangeShipTap(0);
                        break;
                    }

                    else if (currentst == 1) { // ������
                        if (st.health == 10) break;
                        if (vt.gold < goldHealth[st.health]) break;
                        check = 1;
                        SHIP* nowShip = head->next;
                        nowShip->health += upVirusHealth[st.health];
                        nowShip->maxhp += upVirusHealth[st.health];
                        vt.gold -= goldHealth[st.health];
                        printGold();
                        st.health += 1;
                        logIn("�� ü���� ���׷��̵� �Ͽ����ϴ�.");
                        colorChangeShipTap(1);
                        break;
                    }
                    else if (currentst == 2) { // �ӵ�
                        if (st.speed == 5) break;
                        if (vt.gold < goldSpeed[st.speed]) break;
                        check = 1;
                        virusSpeed = upVirusSpeed[st.speed];
                        vt.gold -= goldSpeed[st.speed];
                        printGold();
                        st.speed += 1;
                        logIn("�� �ӵ��� ���׷��̵� �Ͽ����ϴ�.");
                        colorChangeShipTap(2);
                        break;
                    }
                    break;
                }
                else if (currentTap == 3) { //�������� ���� ��
                    if (check == 0) {
                        removeTap();
                        colorChangeItemTap(0);
                        check = 1;
                        break;
                    }
                    if (currentit == 0) { //��¥�⸧����(��� �� �ӵ� ����)
                        check = 1;
                        if (itemList[0] > 0) {
                            logIn("��¥�⸧������ ����Ͽ����ϴ�.");
                            itemList[0] -= 1;
                            itemwork[0] = 1;
                            itemUse();
                        }
                        colorChangeItemTap(0);
                        break;
                    }
                    else if (currentit == 1) { //���ΰ� �� ü�� ȸ��
                        check = 1;
                        if (itemList[1] > 0) {
                            logIn("��� ȸ���� ����Ͽ����ϴ�.");
                            itemList[1] -= 1;
                            itemwork[1] = 1;
                            itemUse();
                        }
                        colorChangeItemTap(1);
                        break;
                    }
                    else if (currentit == 2) { // �Ͻ��� �ӵ� ���� �ִ��(1)
                        check = 1;
                        if (itemList[2] > 0) {
                            logIn("�ӵ� �ִ븦 ����Ͽ����ϴ�.");
                            itemList[2] -= 1;
                            itemwork[2] = 1;
                            itemstart2 = vt.min;
                            itemUse();
                        }
                        colorChangeItemTap(2);
                        break;
                    }
                    else if (currentit == 3) { // ��� ��� �� �νñ�
                        check = 1;
                        if (itemList[3] > 0) {
                            logIn("��ڸ� ����Ͽ����ϴ�.");
                            itemList[3] -= 1;
                            itemwork[3] = 1;

                            itemUse();
                        }
                        colorChangeItemTap(3);
                        break;
                    }
                    else if (currentit == 4) { // ��� ��� ȹ�淮 ������ ����
                        check = 1;
                        if (itemList[4] > 0) {
                            logIn("�������� ����Ͽ����ϴ�.");
                            itemList[4] -= 1;
                            itemwork[4] = 1;
                            itemstart4 = vt.min;
                            itemUse();
                        }
                        colorChangeItemTap(4);
                        break;
                    }
                    break;
                }
                break;

            case A:
                if (currentTap == 0) {
                    if (currentvt == 0) break;
                    currentvt = (currentvt + 2) % 3;
                    if (currentvt == 0) {
                        colorChangeVirusTap(0, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 1) {
                        colorChangeVirusTap(1, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 2) {
                        colorChangeVirusTap(2, currentWaterLv);
                    }
                }
                else if (currentTap == 1) {
                    if (currentrt == 0 || currentrt == 3) break;
                    currentrt = (currentrt + 4) % 5;
                    if (currentrt == 0) {
                        colorChangeRouteTap(0);
                        break;
                    }
                    else if (currentrt == 1) {
                        colorChangeRouteTap(1);
                        break;
                    }
                    else if (currentrt == 2) {
                        colorChangeRouteTap(2);
                        break;
                    }
                    else if (currentrt == 3) {
                        colorChangeRouteTap(3);
                        break;
                    }
                    else if (currentrt == 4) {
                        colorChangeRouteTap(4);
                        break;
                    }
                }
                else if (currentTap == 2) {
                    if (currentst == 0) break;
                    currentst = (currentst + 2) % 3;
                    if (currentst == 0) {
                        colorChangeShipTap(0);
                        break;
                    }
                    else if (currentst == 1) {
                        colorChangeShipTap(1);
                        break;
                    }
                    else if (currentst == 2) {
                        colorChangeShipTap(2);
                        break;
                    }
                }
                else if (currentTap == 3) {
                    if (currentit == 0 || currentrt == 3) break;
                    currentit = (currentit + 4) % 5;
                    if (currentit == 0) {
                        colorChangeItemTap(0);
                        break;
                    }
                    else if (currentit == 1) {
                        colorChangeItemTap(1);
                        break;
                    }
                    else if (currentit == 2) {
                        colorChangeItemTap(2);
                        break;
                    }
                    else if (currentit == 3) {
                        colorChangeItemTap(3);
                        break;
                    }
                    else if (currentit == 4) {
                        colorChangeItemTap(4);
                        break;
                    }
                }
                break;
            case D:
                if (currentTap == 0) {
                    if (currentvt == 2) break;
                    currentvt = (currentvt + 1) % 3;
                    if (currentvt == 0) {
                        colorChangeVirusTap(0, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 1) {
                        colorChangeVirusTap(1, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 2) {
                        colorChangeVirusTap(2, currentWaterLv);
                        break;
                    }
                }
                if (currentTap == 1) {
                    if (currentrt == 4 || currentrt == 2) break;
                    currentrt = (currentrt + 1) % 5;
                    if (currentrt == 0) {
                        colorChangeRouteTap(0);
                        break;
                    }
                    else if (currentrt == 1) {
                        colorChangeRouteTap(1);
                        break;
                    }
                    else if (currentrt == 2) {
                        colorChangeRouteTap(2);
                        break;
                    }
                    else if (currentrt == 3) {
                        colorChangeRouteTap(3);
                        break;
                    }
                    else if (currentrt == 4) {
                        colorChangeRouteTap(4);
                        break;
                    }
                }
                else if (currentTap == 2) {
                    if (currentst == 2) break;
                    currentst = (currentst + 1) % 3;
                    if (currentst == 0) {
                        colorChangeShipTap(0);
                        break;
                    }
                    else if (currentst == 1) {
                        colorChangeShipTap(1);
                        break;
                    }
                    else if (currentst == 2) {
                        colorChangeShipTap(2);
                        break;
                    }
                }
                else if (currentTap == 3) {
                    if (currentit == 4 || currentit == 2) break;
                    currentit = (currentit + 1) % 5;
                    if (currentit == 0) {
                        colorChangeItemTap(0);
                        break;
                    }
                    else if (currentit == 1) {
                        colorChangeItemTap(1);
                        break;
                    }
                    else if (currentit == 2) {
                        colorChangeItemTap(2);
                        break;
                    }
                    else if (currentit == 3) {
                        colorChangeItemTap(3);
                        break;
                    }
                    else if (currentit == 4) {
                        colorChangeItemTap(4);
                        break;
                    }
                }
                break;
            case W:
                if (currentTap == 1) {
                    if (currentrt == 0 || currentrt == 1 || currentrt == 2) {
                        break;
                    }
                    if (currentrt == 3) {
                        currentrt = 0;
                        colorChangeRouteTap(0);
                    }
                    else if (currentrt == 4) {
                        currentrt = 2;
                        colorChangeRouteTap(2);
                    }
                    break;
                }
                else if (currentTap == 3) {
                    if (currentit == 0 || currentit == 1 || currentit == 2) {
                        break;
                    }
                    if (currentit == 3) {
                        currentit = 0;
                        colorChangeItemTap(0);
                    }
                    else if (currentit == 4) {
                        currentit = 2;
                        colorChangeItemTap(2);
                    }
                    break;
                }
                break;
            case S:
                if (currentTap == 1) {
                    if (currentrt == 3 || currentrt == 4) {
                        break;
                    }
                    if (currentrt == 0) {
                        currentrt = 3;
                        colorChangeRouteTap(3);
                        break;
                    }
                    else if (currentrt == 1) {
                        currentrt = 3;
                        colorChangeRouteTap(3);
                        break;
                    }
                    else if (currentrt == 2) {
                        currentrt = 4;
                        colorChangeRouteTap(4);
                        break;
                    }
                }
                else if (currentTap == 3) {
                    if (currentit == 3 || currentit == 4) {
                        break;
                    }
                    if (currentit == 0) {
                        currentit = 3;
                        colorChangeItemTap(3);
                        break;
                    }
                    else if (currentit == 1) {
                        currentit = 3;
                        colorChangeItemTap(3);
                        break;
                    }
                    else if (currentit == 2) {
                        currentit = 4;
                        colorChangeItemTap(4);
                        break;
                    }
                }
                break;
            case '0':
                SummonVaccine(0); break;
            case '1':
                SummonVaccine(1); break;
            }
        }
        printUpEx();
        Sleep(totalSpeed);
    }
}

int detectCollision(int posX, int posY, SHIP* nowship) {
    int arrX = (posX) / 2; // arrX = posX / w
    int arrY = posY; // arrY = posY
    if (nowship->type <= 3) // ��� ���� ���(0,1,2,3) (��� �� �߰��Ǹ� ���� �ٲ������)
    {
        if (World[arrY][arrX] == 5) // ��� �� �浹x
            return 0;
        else if (World[arrY][arrX] == 4) // ��Ź谡 �÷��̾��� �ε����� ���
            return 1;

        else // �ٸ� ��� �浹
            return 3;
    }
    else // ���̷��� ���� ���
    {
        if (World[arrY][arrX] == 5 || World[arrY][arrX] == 10 || World[arrY][arrX] == 11 || World[arrY][arrX] == 12 || World[arrY][arrX] == 13 || World[arrY][arrX] == 14) { // �÷��̾� �� �浹x
            if (World[arrY][arrX] == 10) {
                logIn("��¥ �⸧ ���� ������ ȹ��");
                itemList[0] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("��");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 11) {
                logIn("��� ȸ�� ������ ȹ��");
                itemList[1] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("��");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 12) {
                logIn("�ӵ� �ִ� ������ ȹ��");
                itemList[2] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("��");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 13) {//�� ���� ��� ��Ź� ���δ� ���ֱ�
                logIn("��� ������ ȹ��");
                itemList[3] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("��");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 14) {
                logIn("������ ������ ȹ��");
                itemList[4] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("��");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }


            return 0;
        }

        else if (World[arrY][arrX] <= 3) // �÷��̾�谡 ��Ź�� �ε����� ���
            return 1;
        else if (World[arrY][arrX] == 9)// �ٸ� ��� �浹
            return 2;
        else
            return 3;
    }
}
void itemUse() {
    if (itemwork[0] == 1) {
        SHIP* tmq = head->next;
        for (int k = 1; k < size; k++) {
            tmq = tmq->next;
            tmq->speed -= 1;            //�ӵ� ���� 1���� (�ӽ�)
        }
        itemwork[0] = 0;
    }
    if (itemwork[1] == 1) {//������ �� �� ȸ��
        SHIP* nowShip = head->next;
        nowShip->health = nowShip->maxhp;
        ReBuildShip();
        itemwork[1] = 0;
    }
    if (itemwork[2] == 1) {//���ӽð����� ���ΰ� �� �ְ�ӵ�
        if (itemwork[2] == 1 && vt.min >= itemstart2 + 120) {
            itemwork[2] = 0;
            virusSpeed = upVirusSpeed[st.speed];
        }
        else if (itemwork[2] == 1) {
            virusSpeed = 1;
        }
    }
    if (itemwork[3] == 1) {//�� ���� �� ���� ����
        SHIP* tmq = head->next;
        for (int k = 1; k < size; k++) {
            tmq = tmq->next;
            tmq->health = 0;
            tmq = brokenShip(tmq);
        }

        itemwork[3] = 0;

    }
    if (itemwork[4] == 1) {//���ȹ�淮 �Ͻ������� 3�� ����
        if (itemwork[4] == 1 && vt.min >= itemstart4 + 500) {
            itemwork[4] = 0;
            if (goldUp == 30) {
                goldUp = 10;
            }
            else if (goldUp == 60) {
                goldUp = 20;
            }
            else if (goldUp == 120) {
                goldUp = 30;
            }
        }
        else if (itemwork[4] == 1) {
            if (goldUp == 10) {
                goldUp = 30;
            }
            else if (goldUp == 20) {
                goldUp = 60;
            }
            else if (goldUp == 40) {
                goldUp = 120;
            }
        }
    }

}
void AddShip() {
    SHIP* nowShip = head;
    for (int i = 0; i < size; i++) {
        nowShip = nowShip->next;
        SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
        ShowShip(nowShip);
    }

}

void AddShipInMap(SHIP* nowShip) {
    if (World[nowShip->startPos.Y][nowShip->startPos.X / 2] == 5) {     //�ٴٸ�
        World[nowShip->startPos.Y][nowShip->startPos.X / 2] = nowShip->type;     //type���� �ٲ�
    }
    else if (World[nowShip->startPos.Y][nowShip->startPos.X / 2] == nowShip->type) {
        World[nowShip->startPos.Y][nowShip->startPos.X / 2] = 5;
    }
}

void crashShip(SHIP* nowShip, SHIP* targetShip) // �浹 �� �ļյ� ����, ��Ź� ü�� ����
{
    nowShip->health -= targetShip->power;
    targetShip->health -= nowShip->power;
    TapBreakingShip();
    SetCurrentCursorPos(0, 0);
}

SHIP* brokenShip(SHIP* nowShip) // ü�� ���� �� ����, ħ���� dropItem �Լ� ȣ��
{
    SHIP* nextShip;
    // ������ ��� goalCheck���� �����ϸ� �� �� ����
    SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
    DeleteShip(nowShip);
    if (nowShip->health <= 0) {
        vt.gold += goldUp;//�̰ԺҸ��� ������
        printGold();
        breakEffect(nowShip->startPos);
        int percent = rand() % 100;
        if (percent < 30) {
            SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
            dropItem();
        } // ħ���� 10% Ȯ���� ����
    }
    else {      //�׳ɰ�
        goalEffect(nowShip->startPos);
        if (nowShip->type != 2) {
            vaccinePer += 5;
        }
    }
    //nowShip->isBroken = 1; 
    nextShip = DeleteNode(nowShip); // ��� ����
    return nextShip;
}

void dropItem() // �� �μ����� �� Ȯ�������� ������ drop
{
    int prob[5] = { 22, 22, 22, 12, 22 }; // �ӽ� Ȯ��(Ȯ�� ���������� �÷��ٱ�?)
    int cumulativeProb[5] = { 22, 22, 22, 12, 22 };
    int chooseItem;

    for (int i = 0; i < 4; i++)
        cumulativeProb[i + 1] += cumulativeProb[i]; // ���� Ȯ�� ���� // 70 90 95 98 100

    for (int i = 0; i < 5; i++)
        prob[i] = cumulativeProb[i] - prob[i] + 1; // ������ Ȯ�� // 1 71 91 96 99
    int pb; // random��

    pb = rand() % 100 + 1;
    for (int p = 4; p >= 0; p--)
    {
        if (pb >= prob[p])
        {
            chooseItem = p;
            makeitem(p);

            if (currentTap == 3) {
                colorChangeItemTap(currentit);
            }
            break;
        }
    }
}
void AddRoute(int push, SHIP* nowShip) {
    if (nowShip->type != 4) {
        for (int i = sizeof(nowShip->moveRoute) / sizeof(int) - 1; i > nowShip->movecnt; i--) {
            nowShip->moveRoute[i] = nowShip->moveRoute[i - 1];
        }
        nowShip->moveRoute[nowShip->movecnt] = push;
    }
}
int goalCheck(SHIP* nowShip) {
    if (nowShip->move == 3) {
        if (nowShip->type == 2) return 0;
        logIn("���� ��ǥ�� ������ ����� �����մϴ�.");
        return 0;
    }
    return 1;
}

void updateTime()
{
    vt.min += 1;
    stage = vt.min / 5760;
    int summonEnemy = 5760 / stageEnemy[stage][0] + stageEnemy[stage][1];
    if (vt.min != 0 && vt.min % summonEnemy == 0) {
        if (sumVaccinecnt != 0 && sumVaccinecnt % 5 == 0 && sumTruckcnt < stageEnemy[stage][1]) {
            SummonVaccine(1);
            sumTruckcnt++;
        }
        else if (sumVaccinecnt < stageEnemy[stage][0]) {
            SummonVaccine(0);

        }
        sumVaccinecnt++;
    }
    if (sumMilicnt < stageEnemy[stage][3] && sumVaccinecnt % 5 == 0 && milinext++ % 50 == 0)
        SummonVaccine(2);
    if ((vt.min % 120) == 0) {
        virusUpCnt++;
        virusUpCheck = 1;
    }
    printTime();
}

void MakeShip() {                        //����â �� ����� �Լ�
    int n = 0;
    COORD curPos = { 90,42 };
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 20; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            if (MyShip[i][j] == 0) {
                Colorset(black, black);
                printf(" ");
            }
            else if (MyShip[i][j] == 1) {
                Colorset(white, white);
                printf("��");
                n++;
            }
            else if (MyShip[i][j] == 2) {
                Colorset(gray, gray);
                printf("��");
                n++;
            }
            else if (MyShip[i][j] == 3) {
                Colorset(black, black);
                printf("��");
            }
            else if (MyShip[i][j] == 4) {
                Colorset(black, black);
                printf("��");
            }
        }
    }
}

void TapBreakingShip() {
    SHIP* nowShip = head->next;
    double brokenPer;
    int broken;
    if (nowShip->health > 0) {
        brokenPer = nowShip->health / (double)nowShip->maxhp * 100;
        broken = 121 * (brokenPer / 100);
    }
    else {
        brokenPer = 0;
        broken = 0;
        breakPlayer();
    }
    while (broken <= nowShiphealth) {
        int x = rand() % 20;
        int y = rand() % 15;
        if (MyShip[y][x] == 1) {
            MyShip[y][x] = 3;
            nowShiphealth--;
        }
        else if (MyShip[y][x] == 2) {
            MyShip[y][x] = 4;
            nowShiphealth--;
        }
    }
    MakeShip();
}

void ReBuildShip() {
    SHIP* nowShip = head->next;
    double rebuildPer = nowShip->health / (double)nowShip->maxhp * 100;
    int rebuilt = 121 * (rebuildPer / 100);
    while (rebuilt > nowShiphealth) {
        int x = rand() % 20;
        int y = rand() % 15;
        if (MyShip[y][x] == 3) {
            MyShip[y][x] = 1;
            nowShiphealth++;
        }
        else if (MyShip[y][x] == 4) {
            MyShip[y][x] = 2;
            nowShiphealth++;
        }
    }
    MakeShip();
}
void healing() {
    SHIP* nowShip = head->next;
    int incnt = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (World[nowShip->startPos.Y + j][(nowShip->startPos.X + i * 2) / 2] == 9) {
                incnt = 1;
            }
        }
    }
    if (incnt == 1) {
        healcnt++;
        if (healcnt % 3 == 0) {
            nowShip->health += healingPoint;
            if (nowShip->health > nowShip->maxhp) {
                nowShip->health = nowShip->maxhp;
            }
            ReBuildShip();
        }
    }
}

void breakEffect(COORD pos) {
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, brightYellow);
    printf("*");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, brightYellow);
    printf("��");
    Sleep(100);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, brightYellow);
    printf("��");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, lightBlue);
    printf("��");
}

void goalEffect(COORD pos) {
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("��");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("��");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("��");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("��");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, lightBlue);
    printf("��");
}

void makeitem(int p) {

    COORD curPos = GetCurrentCursorPos();
    Colorset(white, red);

    switch (p) {
    case 0:
        World[curPos.Y][curPos.X / 2] = 10;//��� �� �ӵ� ���� ������
        printf("��");
        break;
    case 1:
        World[curPos.Y][curPos.X / 2] = 11;//�ٸ� �������� �����̰� �ϴ� ������
        printf("��");        break;
    case 2:
        World[curPos.Y][curPos.X / 2] = 12;// ����̵� �����ϰ� �� �Ŀ� ���� ����� �ٴٷ� �̵��ϴ� ������

        printf("��");
        break;
    case 3:
        World[curPos.Y][curPos.X / 2] = 13;// ���̷��� �� ���� ħ����Ų�Ŀ� ��ȭ��� ������
        printf("��");
        break;
    case 4:
        World[curPos.Y][curPos.X / 2] = 14;//ȹ���� �� �ִ� ��ȭ�� 3�� �����ϴ� ������
        printf("��");
        break;
    }
}
void updatePeople()
{
    // ���Ŀ� ���̷��� ���� �Ϸ� �� �ݿ� ����

}

void showBar()            // Bar ����
{
    showPeopleBar();
    showVaccineBar();
}

void showPeopleBar()
{
    updateBar();
    COORD curPos = { 0,0 };
    for (int i = 0; i < BAR_WIDTH - 1; i++)
    {
        SetCurrentCursorPos(curPos.X + (i + 1) * 2, curPos.Y);
        if (population[0][i] == 0) {
            Colorset(black, white);
            printf("  ");
        }
        else if (population[0][i] == 1)
        {
            Colorset(brightRed, brightRed);
            printf("��");
        }
    }
}

void showVaccineBar()
{
    updateBar();
    COORD curPos = { 0,1 };
    for (int i = 0; i < BAR_WIDTH - 1; i++)
    {
        SetCurrentCursorPos(curPos.X + (i + 1) * 2, curPos.Y);
        if (vaccineMaturity[0][i] == 0) {
            Colorset(black, white);
            printf("  ");
        }
        else if (vaccineMaturity[0][i] == 1)
        {
            Colorset(white, white);
            printf("��");
        }
    }
}

void showVaccineBar2()
{
    updateBar();
    COORD curPos = { 0,1 };
    for (int i = 0; i < BAR_WIDTH - 1; i++)
    {
        SetCurrentCursorPos(curPos.X + (i + 1) * 2, curPos.Y);
        if (vaccineMaturity[0][i] == 0) {
            Colorset(black, white);
            printf("  ");
        }
        else if (vaccineMaturity[0][i] == 1)
        {
            Colorset(white, white);
            printf("��");
        }
    }
}

void printPer()
{
    Colorset(black, green);
    SetCurrentCursorPos(104, 0);
    printf("%d%% �������", peoplePer);
    Colorset(black, red);
    SetCurrentCursorPos(104, 1);
    printf("%d%% ��ſϼ���", vaccinePer);
}

void updateBar()
{
    for (int i = 0; i < vaccinePer / 2 - 1; i++)
        vaccineMaturity[0][i] = 1;

    peoplePer = ((double)deadPeople / totalPeople) * 100;

    for (int i = 0; i < peoplePer / 2 - 1; i++)
        population[0][i] = 1;

}

void updateMap() {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 16; j++) {
            if (NorthAmerica[j][i] == 3) {
                SetCurrentCursorPos((6 + i) * 2, 4 + j);
                Colorset(green, red);
                printf("��");

            }
            else if (NorthAmerica[j][i] == 4) {
                SetCurrentCursorPos((6 + i) * 2, 4 + j);
                Colorset(Orange, red);
                printf("��");

            }
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 16; j++) {
            if (SouthAmerica[j][i] == 3) {
                SetCurrentCursorPos((16 + i) * 2, 20 + j);
                Colorset(green, red);
                printf("��");

            }
            else if (SouthAmerica[j][i] == 4) {
                SetCurrentCursorPos((16 + i) * 2, 20 + j);
                Colorset(Orange, red);
                printf("��");

            }
        }
    }

    for (int i = 0; i < 39; i++) {
        for (int j = 0; j < 21; j++) {
            if (EuAsia[j][i] == 3) {
                SetCurrentCursorPos((32 + i) * 2, 6 + j);
                Colorset(green, red);
                printf("��");

            }
            else if (EuAsia[j][i] == 4) {
                SetCurrentCursorPos((32 + i) * 2, 6 + j);
                Colorset(Orange, red);
                printf("��");

            }
        }
    }
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 15; j++) {
            if (Africa[j][i] == 3) {
                SetCurrentCursorPos((34 + i) * 2, 17 + j);
                Colorset(green, red);
                printf("��");

            }
            else if (Africa[j][i] == 4) {
                SetCurrentCursorPos((34 + i) * 2, 17 + j);
                Colorset(Orange, red);
                printf("��");

            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 12; j++) {
            if (Oceania[j][i] == 3) {
                SetCurrentCursorPos((57 + i) * 2, 21 + j);
                Colorset(green, red);
                printf("��");

            }
            else if (Oceania[j][i] == 4) {
                SetCurrentCursorPos((57 + i) * 2, 21 + j);
                Colorset(Orange, red);
                printf("��");

            }
        }
    }

}

void virusUpCal(int i, int j, int maxPeople) {
    int plusDiff; //mapVirus�� �߰��� ��
    int k = mapVirus[0][i] / 5;
    int total = mapVirus[0][0] + mapVirus[0][1] + mapVirus[0][2] + mapVirus[0][3] + mapVirus[0][4];
    switch (i) {
    case 0:
        if (j > 1) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (4 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)) / (j); //����
        }
        else {
            if (mapVirus[0][i] < 6) {
                plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (4 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)); //����
            }
            else if (mapVirus[0][i] < 19) {
                plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (2 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)); //����
            }
            else {
                plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (1 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)); //����
            }
        }
        break;
    case 1:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (11 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //11
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (17 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //15
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (20 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //20
        }
        break;
    case 2:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (9 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //9
        }
        else if (total < 60) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (10 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //10
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (12 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //12
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (17 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //17
        }
        break;
    case 3:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (22 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //50
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (24 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //50
        }
        else if (total < 300) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (40 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //50
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (45 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //50
        }
        break;
    case 4:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (23 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //25
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (28 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //30
        }
        else if (total < 300) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (42 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //45
        }
        else if (total < allMap - mapVirus[1][i]) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (58 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //60
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (63 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //���� //65
        }
        break;
    }
    // �� ��������� ������(�� ����) / ���� �����ڼ� / ���̷����ǿ����� ���ķ� ��ȭ / ����ڼ�(�ݺ��) // �ݿ�
    mapVirus[0][i] += (int)plusDiff; // �迭�� �߰�
    if (mapVirus[0][i] >= maxPeople) { // �迭 �ִ� �� ������ ���
        mapVirus[0][i] = maxPeople;
        plusDiff = (maxPeople - (mapVirus[0][i] - plusDiff));
    }
    infectedPeople = ((double)(mapVirus[0][0] + mapVirus[0][1] + mapVirus[0][2] + mapVirus[0][3] + mapVirus[0][4]) / allMap) * totalPeople; // �����ڼ� ���
    if (virusUpCnt > 2) { // �� ��° ������Ʈ���� ����ڼ� ����
        deadPeople = ((double)(((double)(vt.fatality[i] + 1) / 6 * total)) / allMap) * totalPeople; // ����ڼ� ���
        //deadPeople += ((double)(((double)(vt.fatality[i] + 1) / 5 * plusDiff)) / allMap) * totalPeople; // ����ڼ� ���
        //infectedPeople -= ((double)(((double)(vt.fatality[i] + 1) / 6 * total)) / allMap) * totalPeople; // �����ڼ����� ����ڼ� ����
    }

}


void virusUp() {
    // 30�ʸ��� ������Ʈ�� �س��� ��������
    if (virusUpCheck == 1) {
        if (currentWaterLv == 0) {
            if (infectedPeople == 0) {
                infectedPeople = 12; // (1/allmap)*totalPeople = 12
                mapVirus[0][0] = 1;
            }
            if (mapVirus[0][0] == 0) { mapVirus[0][0] = 1; }
            if (mapVirus[0][0] == 43) { virusUpCheck = 0; return; }
            virusUpCal(0, 1, 43);
            virusUpCheck = 0;
        }
        else if (currentWaterLv == 1) {
            if (infectedPeople == 0) {
                infectedPeople = 12;
                mapVirus[0][0] = 1;
                mapVirus[0][1] = 1;
            }
            if (mapVirus[0][0] == 0) { mapVirus[0][0] = 1; }
            if (mapVirus[0][1] == 0) { mapVirus[0][1] = 1; }
            if (mapVirus[0][0] != 43) { virusUpCal(0, 2, 43); }
            if (mapVirus[0][1] != 98) { virusUpCal(1, 2, 98); }
            virusUpCheck = 0;
        }
        else if (currentWaterLv == 2) {
            if (infectedPeople == 0) {
                infectedPeople = 12;
                mapVirus[0][0] = 1;
                mapVirus[0][1] = 1;
                mapVirus[0][2] = 1;
            }
            if (mapVirus[0][0] == 0) { mapVirus[0][0] = 1; }
            if (mapVirus[0][1] == 0) { mapVirus[0][1] = 1; }
            if (mapVirus[0][2] == 0) { mapVirus[0][2] = 1; }
            if (mapVirus[0][0] != 43) { virusUpCal(0, 2, 43); }
            if (mapVirus[0][1] != 98) { virusUpCal(1, 2, 98); }
            if (mapVirus[0][2] != 74) { virusUpCal(2, 3, 74); }
            virusUpCheck = 0;
        }
        else if (currentWaterLv == 3) {
            if (infectedPeople == 0) {
                infectedPeople = 12;
                mapVirus[0][0] = 1;
                mapVirus[0][1] = 1;
                mapVirus[0][2] = 1;
                mapVirus[0][3] = 1;
            }
            if (mapVirus[0][0] == 0) { mapVirus[0][0] = 1; }
            if (mapVirus[0][1] == 0) { mapVirus[0][1] = 1; }
            if (mapVirus[0][2] == 0) { mapVirus[0][2] = 1; }
            if (mapVirus[0][3] == 0) { mapVirus[0][3] = 1; }
            if (mapVirus[0][0] != 43) { virusUpCal(0, 4, 43); }
            if (mapVirus[0][1] != 98) { virusUpCal(1, 2, 98); }
            if (mapVirus[0][2] != 74) { virusUpCal(2, 3, 74); }
            if (mapVirus[0][3] != 241) { virusUpCal(3, 4, 241); }
            virusUpCheck = 0;
        }
        else if (currentWaterLv == 4) {
            if (infectedPeople == 0) {
                infectedPeople = 12;
                mapVirus[0][0] = 1;
                mapVirus[0][1] = 1;
                mapVirus[0][2] = 1;
                mapVirus[0][3] = 1;
                mapVirus[0][4] = 1;
            }
            if (mapVirus[0][0] == 0) { mapVirus[0][0] = 1; }
            if (mapVirus[0][1] == 0) { mapVirus[0][1] = 1; }
            if (mapVirus[0][2] == 0) { mapVirus[0][2] = 1; }
            if (mapVirus[0][3] == 0) { mapVirus[0][3] = 1; }
            if (mapVirus[0][4] == 0) { mapVirus[0][4] = 1; }
            if (mapVirus[0][0] != 43) { virusUpCal(0, 5, 43); }
            if (mapVirus[0][1] != 98) { virusUpCal(1, 2, 98); }
            if (mapVirus[0][2] != 74) { virusUpCal(2, 3, 74); }
            if (mapVirus[0][3] != 241) { virusUpCal(3, 4, 241); }
            if (mapVirus[0][4] != 366) { virusUpCal(4, 5, 366); }
            virusUpCheck = 0;
        }
        updateMapVirus();
        return;
    }
    else { return; }
}


void breakPlayer() {
    logIn("�÷��̾� �谡 �ı��Ǿ����ϴ�.");
    logIn("20�� �� ������˴ϴ�.");
    die = 1;
    dieTime = vt.min;
}
void showStartScreen()
{
    COORD curPos = { 5,10 };

    for (int i = 1; i < LOGO_HEIGHT - 1; i++) {
        for (int j = 1; j < LOGO_WIDTH - 1; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            if (Logo[i][j] == 0) {
                Colorset(black, white);
                printf("#");
            }
            else if (Logo[i][j] == 1) {
                Colorset(black, white);
                printf(" ");
            }
            else if (Logo[i][j] == 2) {
                Colorset(white, white);
                printf("X");
            }
        }
    }
}
void removeStartScreen() {
    COORD curPos = { 5,10 };
    Colorset(black, white);
    for (int i = 1; i < LOGO_HEIGHT - 1; i++) {
        for (int j = 1; j < LOGO_WIDTH - 1; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            printf(" ");
        }
    }
    SetCurrentCursorPos(82, 40);
    printf("                            ");
}

void dieShip() {
    if (die == 1 && vt.min >= dieTime + 100) {
        SetCurrentCursorPos(96, 47);
        Colorset(black, white);
        printf("                                              ");
        head->next->startPos.X = 2;
        head->next->startPos.Y = 2;
        head->next->health = (int)(head->next->maxhp / 3);
        SetCurrentCursorPos(head->next->startPos.X, head->next->startPos.Y);
        ShowShip(head->next);
        die = 0;
        ReBuildShip();
        logIn("�谡 ����� �Ǿ����ϴ�.");
    }
    else if (die == 1) {
        SetCurrentCursorPos(head->next->startPos.X, head->next->startPos.Y);
        DeleteShip(head->next);
        if (World[head->next->startPos.Y][head->next->startPos.X / 2] == head->next->type) {     //�ٴٸ�
            World[head->next->startPos.Y][head->next->startPos.X / 2] = 5;     //type���� �ٲ�
        }
        SetCurrentCursorPos(96, 47);
        Colorset(black, red);
        printf("                                              ");
        SetCurrentCursorPos(96, 47);
        printf("�� �ı��� /// �������� �����ð� : %d��", (int)((dieTime + 100 - vt.min) / 4));
    }
}

void updateMapVirus() {
    for (int i = 0; i < rt.water + 1; i++) {
        int infec = mapVirus[0][i];
        switch (i) {
        case 0:
            while (infec > nowOse) {
                int x = rand() % 8;
                int y = rand() % 12;
                if (Oceania[y][x] == 1) {
                    Oceania[y][x] = 3;
                    nowOse++;
                }
                else if (Oceania[y][x] == 2) {
                    Oceania[y][x] = 4;
                    nowOse++;
                }
            }
            break;
        case 1:
            while (infec > nowAfr) {
                int x = rand() % 14;
                int y = rand() % 15;
                if (Africa[y][x] == 1) {
                    Africa[y][x] = 3;
                    nowAfr++;
                }
                else if (Africa[y][x] == 2) {
                    Africa[y][x] = 4;
                    nowAfr++;
                }
            }
            break;
        case 2:
            while (infec > nowSAme) {
                int x = rand() % 15;
                int y = rand() % 16;
                if (SouthAmerica[y][x] == 1) {
                    SouthAmerica[y][x] = 3;
                    nowSAme++;
                }
                else if (SouthAmerica[y][x] == 2) {
                    SouthAmerica[y][x] = 4;
                    nowSAme++;
                }
            }
            break;
        case 3:
            while (infec > nowNAme) {
                int x = rand() % 30;
                int y = rand() % 16;
                if (NorthAmerica[y][x] == 1) {
                    NorthAmerica[y][x] = 3;
                    nowNAme++;
                }
                else if (NorthAmerica[y][x] == 2) {
                    NorthAmerica[y][x] = 4;
                    nowNAme++;
                }
            }
            break;
        case 4:
            while (infec > nowEA) {
                int x = rand() % 39;
                int y = rand() % 21;
                if (EuAsia[y][x] == 1) {
                    EuAsia[y][x] = 3;
                    nowEA++;
                }
                else if (EuAsia[y][x] == 2) {
                    EuAsia[y][x] = 4;
                    nowEA++;
                }
            }
            break;
        }
    }
    updateMap();
}

void bloodRouteUpgrade() {
    int temp = rand() % 2;
    if (temp == 0) {
        vt.fatality[currentWaterLv] = 0; // ���� ���׷��̵� ���� ����鿡 ���� ġ���� 0�ܰ��
        currentBlood = 0;

    }
    if (temp == 1) {
        vt.fatality[currentWaterLv] += 2; // ���� ���׷��̵� ���� ����鿡 ���� ġ���� 2�ܰ� ���׷��̵� / �� ���׷��̵� �� ����
        currentBlood = 1;
    }
}

void logIn(char ch[30]) {
    if (logcnt < 20) {
        strcpy(log[logcnt], ch);
        logcnt++;
    }
    else {
        for (int i = 1; i <= logcnt; i++) {
            strcpy(log[i - 1], log[i]);
        }
        strcpy(log[logcnt - 1], ch);
        logcnt = 20;
    }
}

int endCheck() {
    if (deadPeople >= totalPeople / 2) {
        return 1;
    }
    else if (vaccinePer >= 100) {
        return 2;
    }
    return 0;
}

void showEndingScreenWin()
{
    COORD curPos = { 5,12 };
    Colorset(black , white);
    for (int i = 1; i < 25 - 1; i++) {
        for (int j = 1; j < 95 - 1; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            if (viruswin[i][j] == 0) {
                Colorset(black, white);
                printf("@");
            }
            else if (viruswin[i][j] == 1) {
                Colorset(black, white);
                printf(" ");
            }
            else if (viruswin[i][j] == 2) {
                Colorset(white, white);
                printf("X");
            }
        }
    }
    Colorset(black, white);
}

void showEndingScreenLose()
{
    COORD curPos = { 5,12 };
    Colorset(black, white);
    for (int i = 1; i < 25 - 1; i++) {
        for (int j = 1; j < 95 - 1; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            if (virusLose[i][j] == 0) {
                Colorset(black, white);
                printf("@");
            }
            else if (virusLose[i][j] == 1) {
                Colorset(black, white);
                printf(" ");
            }
            else if (virusLose[i][j] == 2) {
                Colorset(white, white);
                printf("X");
            }
        }
    }
    Colorset(black, white);
}