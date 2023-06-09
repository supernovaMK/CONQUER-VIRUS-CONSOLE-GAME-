#pragma once
typedef enum ColorKinds {      //��������
    black, blue, green, skyBlue, red, pink, Orange, white, gray, lightBlue, brightGreen, sky, brightRed, brightPink, brightYellow, brightWhite
}ColorKinds;

typedef struct SHIP {
    COORD startPos;   //�� ������
    COORD endPos;      //�� ����
    int speed;      //�� �ӵ�(1~100)������ ����
    int speedCnt;    //�ӵ� ���� ī��Ʈ��  �ӵ��� ���� 100�̻� �޼��ϸ� ��ĭ ������
    int health;      //�� ü��
    int maxhp;      //�ִ� ü�� (�÷��̾��)
    int move;      //�谡 �����̴� ���� 
    struct SHIP* next;      //���Ḯ��Ʈ �� ������
    int type;      //���� Ÿ��
    int movecnt;        //��ĭ° ���������� Ȯ��
    int power;  //�÷��̾� ��� �ε����� ���̴� ü�� / �÷��̾�谡 ��Ź踦 �ε������� ����ϴ� ü��
    int moveRoute[200];
}SHIP;

int goldSpeed[5];
int goldHealth[10];
int goldPower[10];
int goldWater[5];
int goldHealing[5];
int goldAnimal;
int goldBlood;
int goldAir;
int goldPropagation[2];
int goldFatality[2];
int goldGoldup[2];      // ��ȭȹ�淮 ���� ���
int currentTap;
int currentvt;
int currentst;
int currentrt;
int currentit;
int check;
int itemwork[5];
int itemstart0;
int itemstart1;
int itemstart2;
int itemstart3;
int itemstart4;
int nowShiphealth;
int currentWaterLv;
int currentBlood;
int currentPropagation;
int currentFatality;

int logcnt;
char log[20][50];

SHIP* head;   //���� �Ӹ�
SHIP* tail;   //���� ���� 
int size;   //��ũ�� ����Ʈ ũ��