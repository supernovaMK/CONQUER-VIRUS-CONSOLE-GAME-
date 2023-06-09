#pragma once
typedef enum ColorKinds {      //색깔지정
    black, blue, green, skyBlue, red, pink, Orange, white, gray, lightBlue, brightGreen, sky, brightRed, brightPink, brightYellow, brightWhite
}ColorKinds;

typedef struct SHIP {
    COORD startPos;   //배 시작점
    COORD endPos;      //배 끝점
    int speed;      //배 속도(1~100)까지의 정수
    int speedCnt;    //속도 조절 카운트에  속도를 더해 100이상 달성하면 한칸 움직임
    int health;      //배 체력
    int maxhp;      //최대 체력 (플레이어용)
    int move;      //배가 움직이는 방향 
    struct SHIP* next;      //연결리스트 배 포인터
    int type;      //배의 타입
    int movecnt;        //몇칸째 움직였는지 확인
    int power;  //플레이어 배와 부딪힐때 줄이는 체력 / 플레이어배가 백신배를 부딪혔을때 닳게하는 체력
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
int goldGoldup[2];      // 재화획득량 관련 골드
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

SHIP* head;   //더미 머리
SHIP* tail;   //더미 꼬리 
int size;   //링크드 리스트 크기