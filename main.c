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

#define WORLD_WIDTH 77      //맵의 크기
#define WORLD_HEIGHT 39      //맵의 높이

/*함수*/
void SetCurrentCursorPos(int x, int y);      //현재 커서 위치 변경 함수
COORD GetCurrentCursorPos();   //커서 위치 가져오는 함수
void MakeWorld();      //맵 만들기  
void RemoveCursor();      //커서 지우기
void Colorset(int backColor, int textColor);      //폰트색 변경(글자배경색, 글자색)
void SummonVaccine(int type);       //백신 소환함수
void ShowShip(SHIP* nowShip);      //배를 보여주는 함수
void DeleteShip(SHIP* nowShip);      //배를 지우는 함수
void ProcessKeyInput();         //키입력
void vMove(SHIP* nowShip);     //바이러스가 움직이는 함수
void VirusShipSpawn(SHIP* node);        //바이러스 배 스폰
int detectCollision(int posX, int posY, SHIP* nowship);     //충돌 판정 확인함수
void AddShip();         //배 생성 함수
void AddShipInMap(SHIP* nowShip);       //배 맵에 생성하는 함수
void crashShip(SHIP* nowShip, SHIP* targetShip);        //배가 부딪힐때
SHIP* brokenShip(SHIP* nowShip);            //배가 부숴졌을때
void dropItem();            //아이템 떨어트리는 함수
void AddRoute(int push, SHIP* nowShip);     //부딪힌 후 루트 생성하는 함수
int goalCheck(SHIP* nowShip);           //적배가 목표지점에 들어왔을때 불리는 함수
void updateTime();      //시간 업데이트 함수
void MakeShip();        //화면에 배 생성하는 함수
void TapBreakingShip();     //탭에서 배 모형 부수는 함수
void ReBuildShip();         //탭에서 배 모형 살리는 함수
void healing();             //배 체력 회복하는 함수
void breakEffect(COORD pos);    //부숴지는 이펙트
void goalEffect(COORD pos);    //부숴지는 이펙트
void makeitem(int p);           //아이템 만드는 함수
void updatePeople();    // 인구 수 업데이트 함수
void showPeopleBar();     // 인구 비율 바 생성 함수
void showVaccineBar();    // 백신 완성도 바 생성 함수
void printPer();          // 바 옆 퍼센티지 출력 함수
void showBar();           // 바 통합 출력
void updateBar();         // 바 업데이트 함수
void updateMap();           //감염도에 따라 맵 업데이트하는 함수
void virusUpCal(int i, int j, int maxPeople);       //감염도 계산
void virusUp();             //감염도 올림
void breakPlayer();         //플레이어 체력 0일때 없애는 함수
void showStartScreen();         // 시작 화면 출력 함수
void removeStartScreen();       // 시작 화면 삭제 함수
void dieShip();             //배 죽이는 함수
void itemUse();         //아이템 사용하는 함수
void updateMapVirus();
void bloodRouteUpgrade();        // 감염경로 혈액 업그레이드 시
void logIn(char ch[30]);
int endCheck();
void showEndingScreenWin();
void showEndingScreenLose();

/*bgm 관련 함수*/  // .wav 검색해서 파일 이름 확인할 것!!

void MainSound(int bgmFlag); // bgmFlag == 0일 때(재생 신호시) 재생
void playSoundBreak(); // 배 부서지는 효과음
void playSoundItem(); // 아이템 먹는 효과음
void playSoundCollision(); // virus ship collision 시
void playSoundSinking(); // virus ship 파손 (loading)
void playSoundHealing(); // healing 효과음
void playSoundWarning(); // warning 효과음 (배 등장, 혹은 스테이지 up)
void playSoundLog(); // Log 효과음

/*백신 배 생성 링크드리스트 함수*/
void InitList();                    //링크드 리스트 생성
void InsertAfter(int PosSet, int _type, SHIP* node);        //링크드 리스트에 다음꺼 넣기
SHIP* DeleteNode(SHIP* node);               //노드 삭제
/*끝*/

/*전역변수*/

currentTap = 0;                     //현제 탭 상태
COORD spawnSet[8] = { {94,22}, {12,11}, {32,21},{128,29}, {140,9}, {80, 32} , {62,3},{46,19} };      //시작점(시작점 1 -> 도착점 1)
COORD VirusSpawn = { 2,1 };             //바이러스 스폰 위치
int totalSpeed = 5;                 //전체 속도
int virusSpeed = 5;     //바이러스 배속도(vt.min % virusSpeed >= 0 && vt.min % virusSpeed < 2)

int deadPeople = 0;              // 죽은 인구
int peoplePer = 0;               // 바에 반영 + 출력할 인구 퍼센티지
int vaccinePer = 0;              // 바에 반영 + 출력할 백신완성도 퍼센티지
int die = 0;                    //죽었는가 확인(살아있음 0, 죽었음 1)
int dieTime = 0;                //죽은 시간 확인(확인후 몇초후 생성

/*플레이어 배 업그레이드 표*/
int upVirusSpeed[5] = { 5,4,3,2,1 };     //속도 레벨 별 차이
int upVirusHealth[10] = { 3,5,7,10,13,15,17,20,20,20 };         //체력 레벨별 차이
int upVirusPower[10] = { 1,1,1,1,1,2,2,2,2,2 };         //힘 레벨별 차이
int upVirusHealing[5] = { 1,2,3,4,5 };              //선착장에서 회복되는 속도 레벨별 차이
int goldUp = 50;                //바이러스 배 잡았을 때 얻는 골드
int healcnt = 0;                //회복할때 사용하는 카운트
int healingPoint = 1;               //선착장에서 회복하는 양

itemwork[5] = { 0,0,0,0,0 };            //아이템 사용중인가 확인
itemstart0 = 0;             //아이템 사용한 시간
itemstart1 = 0;             //아이템 사용한 시간
itemstart2 = 0;             //아이템 사용한 시간
itemstart3 = 0;             //아이템 사용한 시간
itemstart4 = 0;             //아이템 사용한 시간

/*업그레이드 골드표*/
goldSpeed[5] = { 50, 150, 250, 350, 450 };          //속도 업그레이드에 사용하는 골드
goldHealth[10] = { 25, 50, 100, 125, 150, 175, 200, 250, 250, 250 };       //체력 업그레이드에 사용하는 골드
goldPower[10] = { 25, 50, 100, 125, 150, 175, 200, 250, 250, 250 };     //힘 업그레이드에 사용하는 골드
goldWater[5] = { 100,300, 500, 700 };     //물 업그레이드에 사용하는 골드
goldHealing[5] = { 50, 100, 200, 350, 500 };            //선착장 업그레이드에 사용하는 골드
goldPropagation[2] = { 500, 1000 };
goldFatality[2] = { 1000,2000 };
goldAnimal = 500;
goldBlood = 500;
goldAir = 500;
goldGoldup[2] = { 300,800 };    // 재화획득량 업그레이드 골드
//goldAir[] = 0;

/*적배 관련 변수*/
int moveRoute[8][200] = { {0,1,1,1,1,1,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,3},//1번움직임
                        {0,1,1,1,-2,-2,-2,-2,1,1,1,1,1,2,-1,-1,2,2,2,1,1,1,2,1,1,1,1,2,2,2,2,-1,-1,1,1 - 1,-2,-1,-1,2,2,2,2,2,2,3},//2번움직임
                        {0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-2,-2,3},
                        {0,2,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,1,1,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,2,2,2,2,2,3},
                        {0,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,3},
                        {0,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3},
                        {0,-1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-2,-1,-1,-2,-1,-1,3},
                        {0,2,2,2,2,2,2,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-2,-2,3} };

int stageEnemy[][4] = { {40,0,0,0},      //스테이지 별 배 생성 개수
                        {60,7,0,0},
                        {80,15,2,0},
                        {110,20,4,0} };
int stageSpeed[][4] = { {30,15,30,0},  //스테이지 별 배 속도(1~100)까지의 정수
                        {45,20,45,0},
                        {50,25,50,0},
                        {65,30,65,0} };
int stageHealth[][4] = { {2,10,5,1 },  //스테이지 별 배 체력(1~100)까지의 정수
                        {3,15,7,1},
                        {5,20,9,1},
                        {7,30,13,1} };
int stagePower[][4] = { {2, 1, 4, 20 },  //스테이지 별 배 파워(1~100)까지의 정수
                        {3, 2, 6, 20},
                        {5, 2, 8, 20},
                        {7, 3, 10, 20} };

int sumVaccinecnt = 0;              //백신배가 얼마나 생성되었는가(updatetime내부의 몬스터 생성에 사용함)
int sumTruckcnt = 0;                //화물선이 얼마나 생성되었는가
int sumMilicnt = 0;             //군함이 얼마나 생성되었는가
int sumBombcnt = 0;         //어뢰가 얼마나 생성되었는가
int milinext = 0;           //군함이 백신배와 함께 등장할때 카운트

int stage = 0;              //현재 스테이지

/*탭 관련 변수*/
currentvt = 0;
currentst = 0;
currentrt = 0;
currentit = 0;
check = 0;
logcnt = 0;

/*바이러스 관련 변수*/
int mapVirus[2][5] = { {0,0,0,0,0},     //대륙별 감염 인구 (오세아니아 > 아프리카 > 남아메리카> 북아메리카 > 유라시아)
                       {43,98,74,241,366} };    //대륙별 인구 비율
int allMap = 827;
int totalPeople = 10000;            // 전체 인구(임시)
int infectedPeople = 0;          // 감염된 인구
int currentWaterLv = 0; // 감염경로 물 업그레이드 현재 레벨
int virusUpCheck = 0;
int virusUpCnt = 1;

currentBlood = -1; // 감염경로 혈액 선택 시 현재의 랜덤 종류
nowShiphealth = 121;

int nowOse = 0;
int nowAfr = 0;
int nowSAme = 0;
int nowNAme = 0;
int nowEA = 0;

void main()
{
    COORD bufferSize = { 100, 10000 }; // 가로, 세로
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
    vt.gold = 1000;
    //vt.min += 14300;
    //vt.health = 0; // 임시
    system("mode con cols=200 lines=60");      //콘솔 크기 조정
    SetConsoleTitle(TEXT("Conqure Us"));        //콘솔 이름 설정
    while (_kbhit() == 0)
    {
        showStartScreen(); // 임시로 press any key
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
    VirusShipSpawn(head);       // 바이러스배 리스트에 추가
    showBar();                  // 바 출력 함수
    printPer();                 // 퍼센티지 출력 함수
    srand((unsigned int)time(NULL));        //랜덤
    int nowstage = 0;
    int cnt = 0;
    logIn("게임이 시작되었습니다.");
    logIn("---------------- STAGE: 1----------------");

    PlaySound(TEXT("Mainbgm.wav"), NULL, SND_ASYNC | SND_LOOP);

    while (nowstage < 4) {// 원래 4인데 임시
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
                if (100 < tmp->speedCnt) {       //속도 계산식(배마다 속도를 계산)
                    vMove(tmp);         //속도가 맞으면 움직임
                    tmp->move = tmp->moveRoute[tmp->movecnt++];    //방향전환 (루트대로 따라감)
                    if (!goalCheck(tmp)) {      //골지점 들어오면 0
                        tmp = brokenShip(tmp);
                        continue;       //다음 함수 부름
                    }
                    tmp->speedCnt = tmp->speedCnt % 100;
                }
                tmp->speedCnt += tmp->speed;            //속도 카운트 증가
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

void SetCurrentCursorPos(int x, int y) {         //커서 위치 설정
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos(void) {         //현재 커서 위치 부르기
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    return curInfo.dwCursorPosition;
}


void MakeWorld() {                        //맵 만들기 함수
    COORD curPos = { 0,0 };
    for (int i = 2; i < WORLD_HEIGHT - 1; i++) {
        for (int j = 1; j < WORLD_WIDTH - 1; j++) {
            SetCurrentCursorPos(curPos.X + j * 2, curPos.Y + i);
            if (World[i][j] == 6) {
                Colorset(green, green);
                printf("■");
            }
            else if (World[i][j] == 5) {
                Colorset(lightBlue, lightBlue);
                printf("■");
            }
            else if (World[i][j] == 7) {
                Colorset(Orange, Orange);
                printf("■");
            }
            else if (World[i][j] == 8) {
                Colorset(green, brightRed);
                printf("■");
            }
            else if (World[i][j] == 9) {
                Colorset(brightYellow, red);
                printf("♥");
            }
        }
    }
}

void RemoveCursor(void) {            //커서 지우기
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void Colorset(int backColor, int textColor) {         //색 변환 함수
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Handle, (backColor << 4) + textColor);
}

void SummonVaccine(int type) {      //백신배 소환
    switch (type) {
    case 0: logIn("빨간색 백신배가 생성됩니다."); break;
    case 1: logIn("노란색 화물선이 생성됩니다."); break;
    case 2: logIn("회색 군함이 생성됩니다."); break;
    case 3: logIn("어뢰가 생성됩니다."); break;
    }
    SHIP* nowShip = head;       //여기서 처음 배 값을 넣어

    int PosSet = rand() % 8;     //소환장소 랜덤
    for (int i = 0; i < size; i++) {
        nowShip = nowShip->next;
    }
    InsertAfter(PosSet, type, nowShip);
}

void ShowShip(SHIP* nowShip) {
    COORD curPos = GetCurrentCursorPos();
    AddShipInMap(nowShip);      //배를 바다에 생성
    switch (nowShip->type) {
    case 0: Colorset(lightBlue, red); break;
    case 1: Colorset(lightBlue, Orange); break;
    case 2: Colorset(lightBlue, gray); break;
    case 3: Colorset(lightBlue, red); break;
    case 4: Colorset(lightBlue, white); break;
    }
    switch (nowShip->move) {
    case 1:printf("▼");  break;
    case -1:printf("▲"); break;
    case 2:printf("▶");  break;
    case -2:printf("◀"); break;
    case 0: printf("◆"); break;
    }
    SetCurrentCursorPos(curPos.X, curPos.Y);
}

void DeleteShip(SHIP* nowShip) {
    AddShipInMap(nowShip);      //배를 바다에서 없앰
    COORD curPos = GetCurrentCursorPos();
    Colorset(lightBlue, lightBlue);
    printf("■");
    SetCurrentCursorPos(curPos.X, curPos.Y);
}


void InitList() {       //링크드리스트 생성
    head = (SHIP*)malloc(sizeof(SHIP));      //머리 메모리 할당
    tail = (SHIP*)malloc(sizeof(SHIP));      //꼬리 메모리 할당
    head->next = tail;            //머리 다음은 꼬리
    tail->next = tail;            //꼬리 다음도 꼬리
    size = 0;               //사이즈는 0
}

void InsertAfter(int PosSet, int _type, SHIP* node) {
    if (node == tail)            //꼬리 뒤에 삽입 할수 없다.
        return;
    SHIP* newNode = (SHIP*)malloc(sizeof(SHIP));   //메모리 할당
    newNode->startPos = spawnSet[PosSet];            //값을 넣어준다   
    newNode->type = _type;
    newNode->move = 0;
    newNode->next = node->next;         //특정노드가 가르키던 노드를 내가 대신 가르키고
    newNode->movecnt = 0;
    newNode->speedCnt = 0;
    newNode->speed = stageSpeed[stage][_type];
    for (int i = 0; i < sizeof(moveRoute[PosSet]) / sizeof(int); i++) {
        newNode->moveRoute[i] = moveRoute[PosSet][i];
    }
    switch (_type) {
    case 0:     //일반 백신배
        newNode->health = stageHealth[stage][0];
        newNode->power = stagePower[stage][0];
        break;
    case 1:     //화물선
        newNode->health = stageHealth[stage][1];
        newNode->power = stagePower[stage][1];
        break;
    case 2:     //군함
        newNode->health = stageHealth[stage][2];
        newNode->power = stagePower[stage][2];
        break;
    case 3:     //어뢰
        newNode->health = stageHealth[stage][3];
        newNode->power = stagePower[stage][3];
        break;
    }
    AddShipInMap(newNode);
    node->next = newNode;            //특정노드가 나를 가리키게 한다.
    ++size;                  //크기를 하나 늘려준다.
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
    if (node == head || tail == node)   //머리나 꼬리는 지울수 없다.
        return 0;
    SHIP* preNode = head;         //노드는 이전노드 정보를 가지고 있지 않아서 따로 기억해야한다.
    SHIP* temp = head->next;      //찾는 노드
    while (temp != tail)               //리스트를 끝까지 돈다.
    {
        if (node == temp)            //지울 노드를 찾았다면
        {
            SHIP* deleteNode = node;      //지울 노드를 기억한다.
            preNode->next = deleteNode->next;   //이전 노드가 지울 노드의 다음을 가르키게한다.
            free(deleteNode);         //지울 노드를 메모리 해제
            --size;               //크기를 줄여준다.
            temp = preNode->next;
            return temp;
        }
        preNode = temp;         //못 찾았으면
        temp = temp->next;      //다음 노드로
    }
    return temp;
}

void vMove(SHIP* nowShip) {//바이러스 배 이동함수
    SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
    DeleteShip(nowShip);
    SHIP* targetShip = head;
    int mNext;  //다음 움직임에서 충돌할때 무엇과 충돌하는지 확인하는 변수
    COORD now = nowShip->startPos;
    switch (nowShip->move) {
    case 1:
        mNext = detectCollision(nowShip->startPos.X, nowShip->startPos.Y + 1, nowShip);
        if (mNext == 0)     //충돌 안했을 경우
            nowShip->startPos.Y++;
        else if (mNext == 1) {    //플레이어배가 백신배와 부딪힐경우
            while (nowShip->startPos.X != targetShip->startPos.X || nowShip->startPos.Y + 1 != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*플레이어배 움직이기*/
            if (World[targetShip->startPos.Y + 1][targetShip->startPos.X / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.Y += 1;
                AddRoute(-1, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*종료*/
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
        else if (mNext == 1) {    //플레이어배가 백신배와 부딪힐경우
            while (nowShip->startPos.X != targetShip->startPos.X || nowShip->startPos.Y - 1 != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*플레이어배 움직이기*/
            if (World[targetShip->startPos.Y - 1][targetShip->startPos.X / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.Y -= 1;
                AddRoute(1, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*종료*/
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
        else if (mNext == 1) {    //플레이어배가 백신배와 부딪힐경우
            while (nowShip->startPos.X + 2 != targetShip->startPos.X || nowShip->startPos.Y != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*플레이어배 움직이기*/
            if (World[targetShip->startPos.Y][(targetShip->startPos.X + 2) / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.X += 2;
                AddRoute(-2, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*종료*/
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
        else if (mNext == 1) {    //플레이어배가 백신배와 부딪힐경우
            while (nowShip->startPos.X - 2 != targetShip->startPos.X || nowShip->startPos.Y != targetShip->startPos.Y) {
                targetShip = targetShip->next;
            }
            crashShip(nowShip, targetShip);
            /*플레이어배 움직이기*/
            if (World[targetShip->startPos.Y][(targetShip->startPos.X - 2) / 2] == 5) {
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                DeleteShip(targetShip);
                targetShip->startPos.X -= 2;
                AddRoute(2, targetShip);
                SetCurrentCursorPos(targetShip->startPos.X, targetShip->startPos.Y);
                ShowShip(targetShip);
            }
            /*종료*/
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
void ProcessKeyInput() {//키 입력받는 함수
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
                /*업그레이드 선택시 일어나는 일*/
            case F:
                if (currentTap == 0) {      //바이러스 강화 탭일경우
                    if (check == 0) {
                        removeTap();
                        colorChangeVirusTap(0, currentWaterLv);
                        check = 1;
                        break;
                    }
                    if (currentvt == 0) {           //바이러스 강화 탭 중 전파력일 경우
                        check = 1;
                        if (vt.gold < goldPropagation[currentPropagation]) { break; }
                        for (int i = 0; i < 5; i++) {   // 전파력 업그레이드하여 바이러스 전파에 반영
                            if (vt.propagation[i] - rt.animal >= 2) { continue; }
                            vt.propagation[i] += 1;
                        }
                        if (currentPropagation > 2) { currentPropagation -= 1; }
                        if (currentPropagation >= 2) break;
                        logIn("전파력을 업그레이드 하였습니다.");
                        vt.gold -= goldPropagation[currentPropagation];
                        currentPropagation += 1;
                        printGold();
                        colorChangeVirusTap(0, currentWaterLv);
                        break;
                    }
                    else if (currentvt == 1) {      //바이러스 강화 탭 중 치사율일 경우
                        check = 1;
                        if (vt.gold < goldFatality[currentFatality]) { break; }
                        for (int i = 0; i < 5; i++) {   // 치사율 업그레이드하여 사망자수에 반영 / 감염자수에도 영향
                            if (currentBlood == 1 && vt.fatality[i] - rt.blood * 2 >= 2) { continue; }
                            if (currentBlood != 1 && vt.fatality[i] >= 2) { continue; }
                            vt.fatality[i] += 1;
                        }
                        if (currentFatality > 2) { currentFatality -= 1; }
                        if (currentFatality >= 2) break;
                        logIn("치사율을 업그레이드 하였습니다.");
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
                        logIn("재화 획득량을 업그레이드 하였습니다.");
                        printGold();
                        colorChangeVirusTap(2, currentWaterLv);
                        break;
                    }
                    break;
                }
                else if (currentTap == 1) {     //감염 경로 강화탭일경우
                    if (check == 0) {
                        removeTap();
                        colorChangeRouteTap(0);
                        check = 1;
                        break;
                    }
                    if (currentrt == 0) {       //감염경로 동물
                        check = 1;
                        if (rt.animal > 1)  break;  // 각 물 업그레이드 레벨마다 MAX 1 / 물 업그레이드 시 0으로 초기화
                        if (goldAnimal > vt.gold) break;
                        rt.animal += 1;
                        vt.propagation[currentWaterLv]++; // 대륙 전파력 상승 / 현재 업그레이드 중인 대륙에만 영향
                        vt.gold -= goldAnimal;
                        printGold();
                        colorChangeRouteTap(0);
                        logIn("동물 전파를 업그레이드 하였습니다.");
                        break;
                    }
                    else if (currentrt == 1) {  //감염경로 혈액
                        check = 1;
                        if (rt.blood > 1)  break;  // 각 물 업그레이드 레벨마다 MAX 1 / 물 업그레이드 시 0으로 초기화
                        if (goldBlood > vt.gold) break;
                        rt.blood += 1;
                        vt.gold -= goldBlood;
                        printGold();
                        bloodRouteUpgrade();                // 돌연변이 / 현재 업그레이드 중인 대륙의 치사율 랜덤으로 0단계로 하강 또는 2단계 상승
                        colorChangeRouteTap(1);
                        logIn("혈액 전파를 업그레이드 하였습니다.");
                        if (currentBlood == 0) {
                            logIn("돌연변이로 치사율이 0 레벨이 되었습니다.");
                        }
                        else if (currentBlood == 1) {
                            logIn("돌연변이로 치사율이 2 레벨 상승합니다.");
                        }
                        break;
                    }
                    else if (currentrt == 2) {      //감염경로 공기
                        check = 1;
                        if (rt.air == 1) break;
                        if (goldAir > vt.gold) break;
                        rt.air += 1;                // 바이러스 전파 맵 반영 이후 추가
                        vt.gold -= goldAir;
                        printGold();
                        colorChangeRouteTap(2);
                        logIn("대기 전파를 업그레이드 하였습니다.");
                        break;
                    }
                    else if (currentrt == 3) {      //감염경로 물
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
                        logIn("물 경로를 업그레이드 하였습니다.");
                        colorChangeRouteTap(3);
                        break;
                    }
                    else if (currentrt == 4) {      //감염경로 선착장
                        check = 1;
                        if (rt.port == 5)  break;
                        if (vt.gold < goldHealing[rt.port]) break;
                        healingPoint += upVirusHealing[rt.port];
                        vt.gold -= goldHealing[rt.port];
                        printGold();
                        rt.port += 1;
                        logIn("선착장을 업그레이드 하였습니다.");
                        colorChangeRouteTap(4);
                        break;
                    }
                    break;
                }
                else if (currentTap == 2) { // 배 강화
                    if (check == 0) {
                        colorChangeShipTap(0);
                        check = 1;
                        break;
                    }

                    if (currentst == 0) { // 공격력
                        if (st.power == 10) break;
                        if (vt.gold < goldPower[st.power]) break;
                        check = 1;
                        SHIP* nowShip = head->next;
                        nowShip->power += upVirusPower[st.power];
                        vt.gold -= goldPower[st.power];
                        printGold();
                        st.power += 1;
                        logIn("배 공격력을 업그레이드 하였습니다.");
                        colorChangeShipTap(0);
                        break;
                    }

                    else if (currentst == 1) { // 내구력
                        if (st.health == 10) break;
                        if (vt.gold < goldHealth[st.health]) break;
                        check = 1;
                        SHIP* nowShip = head->next;
                        nowShip->health += upVirusHealth[st.health];
                        nowShip->maxhp += upVirusHealth[st.health];
                        vt.gold -= goldHealth[st.health];
                        printGold();
                        st.health += 1;
                        logIn("배 체력을 업그레이드 하였습니다.");
                        colorChangeShipTap(1);
                        break;
                    }
                    else if (currentst == 2) { // 속도
                        if (st.speed == 5) break;
                        if (vt.gold < goldSpeed[st.speed]) break;
                        check = 1;
                        virusSpeed = upVirusSpeed[st.speed];
                        vt.gold -= goldSpeed[st.speed];
                        printGold();
                        st.speed += 1;
                        logIn("배 속도를 업그레이드 하였습니다.");
                        colorChangeShipTap(2);
                        break;
                    }
                    break;
                }
                else if (currentTap == 3) { //아이템탭 선택 시
                    if (check == 0) {
                        removeTap();
                        colorChangeItemTap(0);
                        check = 1;
                        break;
                    }
                    if (currentit == 0) { //가짜기름유포(백신 배 속도 감소)
                        check = 1;
                        if (itemList[0] > 0) {
                            logIn("가짜기름유포를 사용하였습니다.");
                            itemList[0] -= 1;
                            itemwork[0] = 1;
                            itemUse();
                        }
                        colorChangeItemTap(0);
                        break;
                    }
                    else if (currentit == 1) { //주인공 배 체력 회복
                        check = 1;
                        if (itemList[1] > 0) {
                            logIn("긴급 회복을 사용하였습니다.");
                            itemList[1] -= 1;
                            itemwork[1] = 1;
                            itemUse();
                        }
                        colorChangeItemTap(1);
                        break;
                    }
                    else if (currentit == 2) { // 일시적 속도 증가 최대로(1)
                        check = 1;
                        if (itemList[2] > 0) {
                            logIn("속도 최대를 사용하였습니다.");
                            itemList[2] -= 1;
                            itemwork[2] = 1;
                            itemstart2 = vt.min;
                            itemUse();
                        }
                        colorChangeItemTap(2);
                        break;
                    }
                    else if (currentit == 3) { // 모든 상대 배 부시기
                        check = 1;
                        if (itemList[3] > 0) {
                            logIn("어뢰를 사용하였습니다.");
                            itemList[3] -= 1;
                            itemwork[3] = 1;

                            itemUse();
                        }
                        colorChangeItemTap(3);
                        break;
                    }
                    else if (currentit == 4) { // 얻는 골드 획득량 영구적 증가
                        check = 1;
                        if (itemList[4] > 0) {
                            logIn("인질극을 사용하였습니다.");
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
    if (nowship->type <= 3) // 백신 배인 경우(0,1,2,3) (백신 배 추가되면 조건 바꿔줘야함)
    {
        if (World[arrY][arrX] == 5) // 백신 배 충돌x
            return 0;
        else if (World[arrY][arrX] == 4) // 백신배가 플레이어배와 부딪히는 경우
            return 1;

        else // 다른 모든 충돌
            return 3;
    }
    else // 바이러스 배인 경우
    {
        if (World[arrY][arrX] == 5 || World[arrY][arrX] == 10 || World[arrY][arrX] == 11 || World[arrY][arrX] == 12 || World[arrY][arrX] == 13 || World[arrY][arrX] == 14) { // 플레이어 배 충돌x
            if (World[arrY][arrX] == 10) {
                logIn("가짜 기름 유포 아이템 획득");
                itemList[0] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("■");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 11) {
                logIn("긴급 회복 아이템 획득");
                itemList[1] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("■");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 12) {
                logIn("속도 최대 아이템 획득");
                itemList[2] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("■");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 13) {//맵 위의 모든 백신배 전부다 없애기
                logIn("어뢰 아이템 획득");
                itemList[3] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("■");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }
            else if (World[arrY][arrX] == 14) {
                logIn("인질극 아이템 획득");
                itemList[4] += 1;
                World[arrY][arrX] = 5;
                Colorset(lightBlue, lightBlue);
                printf("■");
                if (currentTap == 3)
                    colorChangeItemTap(currentit);
            }


            return 0;
        }

        else if (World[arrY][arrX] <= 3) // 플레이어배가 백신배와 부딪히는 경우
            return 1;
        else if (World[arrY][arrX] == 9)// 다른 모든 충돌
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
            tmq->speed -= 1;            //속도 감소 1감소 (임시)
        }
        itemwork[0] = 0;
    }
    if (itemwork[1] == 1) {//아이템 배 피 회복
        SHIP* nowShip = head->next;
        nowShip->health = nowShip->maxhp;
        ReBuildShip();
        itemwork[1] = 0;
    }
    if (itemwork[2] == 1) {//지속시간동안 주인공 배 최고속도
        if (itemwork[2] == 1 && vt.min >= itemstart2 + 120) {
            itemwork[2] = 0;
            virusSpeed = upVirusSpeed[st.speed];
        }
        else if (itemwork[2] == 1) {
            virusSpeed = 1;
        }
    }
    if (itemwork[3] == 1) {//맵 위의 배 전부 삭제
        SHIP* tmq = head->next;
        for (int k = 1; k < size; k++) {
            tmq = tmq->next;
            tmq->health = 0;
            tmq = brokenShip(tmq);
        }

        itemwork[3] = 0;

    }
    if (itemwork[4] == 1) {//골드획득량 일시적으로 3배 증가
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
    if (World[nowShip->startPos.Y][nowShip->startPos.X / 2] == 5) {     //바다면
        World[nowShip->startPos.Y][nowShip->startPos.X / 2] = nowShip->type;     //type으로 바꿈
    }
    else if (World[nowShip->startPos.Y][nowShip->startPos.X / 2] == nowShip->type) {
        World[nowShip->startPos.Y][nowShip->startPos.X / 2] = 5;
    }
}

void crashShip(SHIP* nowShip, SHIP* targetShip) // 충돌 시 파손도 감소, 백신배 체력 감소
{
    nowShip->health -= targetShip->power;
    targetShip->health -= nowShip->power;
    TapBreakingShip();
    SetCurrentCursorPos(0, 0);
}

SHIP* brokenShip(SHIP* nowShip) // 체력 없는 배 삭제, 침몰시 dropItem 함수 호출
{
    SHIP* nextShip;
    // 도착한 배는 goalCheck에서 삭제하면 될 것 같음
    SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
    DeleteShip(nowShip);
    if (nowShip->health <= 0) {
        vt.gold += goldUp;//이게불리면 터진거
        printGold();
        breakEffect(nowShip->startPos);
        int percent = rand() % 100;
        if (percent < 30) {
            SetCurrentCursorPos(nowShip->startPos.X, nowShip->startPos.Y);
            dropItem();
        } // 침몰시 10% 확률로 수행
    }
    else {      //그냥골
        goalEffect(nowShip->startPos);
        if (nowShip->type != 2) {
            vaccinePer += 5;
        }
    }
    //nowShip->isBroken = 1; 
    nextShip = DeleteNode(nowShip); // 노드 삭제
    return nextShip;
}

void dropItem() // 배 부서졌을 시 확률적으로 아이템 drop
{
    int prob[5] = { 22, 22, 22, 12, 22 }; // 임시 확률(확률 전역변수로 올려줄까?)
    int cumulativeProb[5] = { 22, 22, 22, 12, 22 };
    int chooseItem;

    for (int i = 0; i < 4; i++)
        cumulativeProb[i + 1] += cumulativeProb[i]; // 누적 확률 구함 // 70 90 95 98 100

    for (int i = 0; i < 5; i++)
        prob[i] = cumulativeProb[i] - prob[i] + 1; // 시작점 확인 // 1 71 91 96 99
    int pb; // random값

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
        logIn("적이 목표에 도달해 백신을 제작합니다.");
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

void MakeShip() {                        //수리창 배 만들기 함수
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
                printf("■");
                n++;
            }
            else if (MyShip[i][j] == 2) {
                Colorset(gray, gray);
                printf("■");
                n++;
            }
            else if (MyShip[i][j] == 3) {
                Colorset(black, black);
                printf("■");
            }
            else if (MyShip[i][j] == 4) {
                Colorset(black, black);
                printf("■");
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
    printf("★");
    Sleep(100);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, brightYellow);
    printf("☆");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, lightBlue);
    printf("■");
}

void goalEffect(COORD pos) {
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("ο");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("◎");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("●");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, red);
    printf("○");
    Sleep(50);
    SetCurrentCursorPos(pos.X, pos.Y);
    Colorset(lightBlue, lightBlue);
    printf("■");
}

void makeitem(int p) {

    COORD curPos = GetCurrentCursorPos();
    Colorset(white, red);

    switch (p) {
    case 0:
        World[curPos.Y][curPos.X / 2] = 10;//백신 배 속도 감소 아이템
        printf("♣");
        break;
    case 1:
        World[curPos.Y][curPos.X / 2] = 11;//다른 방향으로 움직이게 하는 아이템
        printf("★");        break;
    case 2:
        World[curPos.Y][curPos.X / 2] = 12;// 대륙이동 가능하게 한 후에 제일 가까운 바다로 이동하는 아이템

        printf("▣");
        break;
    case 3:
        World[curPos.Y][curPos.X / 2] = 13;// 바이러스 배 전부 침몰시킨후에 재화얻는 아이템
        printf("♨");
        break;
    case 4:
        World[curPos.Y][curPos.X / 2] = 14;//획득할 수 있는 재화량 3배 증가하는 아이템
        printf("♬");
        break;
    }
}
void updatePeople()
{
    // 추후에 바이러스 전파 완료 후 반영 예정

}

void showBar()            // Bar 통합
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
            printf("■");
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
            printf("■");
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
            printf("■");
        }
    }
}

void printPer()
{
    Colorset(black, green);
    SetCurrentCursorPos(104, 0);
    printf("%d%% 사망비율", peoplePer);
    Colorset(black, red);
    SetCurrentCursorPos(104, 1);
    printf("%d%% 백신완성도", vaccinePer);
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
                printf("■");

            }
            else if (NorthAmerica[j][i] == 4) {
                SetCurrentCursorPos((6 + i) * 2, 4 + j);
                Colorset(Orange, red);
                printf("■");

            }
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 16; j++) {
            if (SouthAmerica[j][i] == 3) {
                SetCurrentCursorPos((16 + i) * 2, 20 + j);
                Colorset(green, red);
                printf("■");

            }
            else if (SouthAmerica[j][i] == 4) {
                SetCurrentCursorPos((16 + i) * 2, 20 + j);
                Colorset(Orange, red);
                printf("■");

            }
        }
    }

    for (int i = 0; i < 39; i++) {
        for (int j = 0; j < 21; j++) {
            if (EuAsia[j][i] == 3) {
                SetCurrentCursorPos((32 + i) * 2, 6 + j);
                Colorset(green, red);
                printf("■");

            }
            else if (EuAsia[j][i] == 4) {
                SetCurrentCursorPos((32 + i) * 2, 6 + j);
                Colorset(Orange, red);
                printf("■");

            }
        }
    }
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 15; j++) {
            if (Africa[j][i] == 3) {
                SetCurrentCursorPos((34 + i) * 2, 17 + j);
                Colorset(green, red);
                printf("■");

            }
            else if (Africa[j][i] == 4) {
                SetCurrentCursorPos((34 + i) * 2, 17 + j);
                Colorset(Orange, red);
                printf("■");

            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 12; j++) {
            if (Oceania[j][i] == 3) {
                SetCurrentCursorPos((57 + i) * 2, 21 + j);
                Colorset(green, red);
                printf("■");

            }
            else if (Oceania[j][i] == 4) {
                SetCurrentCursorPos((57 + i) * 2, 21 + j);
                Colorset(Orange, red);
                printf("■");

            }
        }
    }

}

void virusUpCal(int i, int j, int maxPeople) {
    int plusDiff; //mapVirus에 추가할 양
    int k = mapVirus[0][i] / 5;
    int total = mapVirus[0][0] + mapVirus[0][1] + mapVirus[0][2] + mapVirus[0][3] + mapVirus[0][4];
    switch (i) {
    case 0:
        if (j > 1) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (4 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)) / (j); //수정
        }
        else {
            if (mapVirus[0][i] < 6) {
                plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (4 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)); //수정
            }
            else if (mapVirus[0][i] < 19) {
                plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (2 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)); //수정
            }
            else {
                plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (1 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1)); //수정
            }
        }
        break;
    case 1:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (11 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //11
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (17 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //15
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (20 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //20
        }
        break;
    case 2:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (9 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //9
        }
        else if (total < 60) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (10 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //10
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (12 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //12
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (17 + (vt.propagation[i]) * 2) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //17
        }
        break;
    case 3:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (22 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //50
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (24 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //50
        }
        else if (total < 300) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (40 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //50
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (45 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //50
        }
        break;
    case 4:
        if (total < 10) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (23 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //25
        }
        else if (total < 100) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (28 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //30
        }
        else if (total < 300) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (42 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //45
        }
        else if (total < allMap - mapVirus[1][i]) {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (58 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //60
        }
        else {
            plusDiff = ((double)mapVirus[0][i] / maxPeople) * infectedPeople * (63 + vt.propagation[i]) * ((double)1 / (deadPeople + 1) / (k + 1)); //수정 //65
        }
        break;
    }
    // 각 대륙에서의 전염률(■ 비율) / 현재 감염자수 / 바이러스탭에서의 전파력 강화 / 사망자수(반비례) // 반영
    mapVirus[0][i] += (int)plusDiff; // 배열에 추가
    if (mapVirus[0][i] >= maxPeople) { // 배열 최대 수 도달한 경우
        mapVirus[0][i] = maxPeople;
        plusDiff = (maxPeople - (mapVirus[0][i] - plusDiff));
    }
    infectedPeople = ((double)(mapVirus[0][0] + mapVirus[0][1] + mapVirus[0][2] + mapVirus[0][3] + mapVirus[0][4]) / allMap) * totalPeople; // 감염자수 계산
    if (virusUpCnt > 2) { // 두 번째 업데이트부터 사망자수 생김
        deadPeople = ((double)(((double)(vt.fatality[i] + 1) / 6 * total)) / allMap) * totalPeople; // 사망자수 계산
        //deadPeople += ((double)(((double)(vt.fatality[i] + 1) / 5 * plusDiff)) / allMap) * totalPeople; // 사망자수 계산
        //infectedPeople -= ((double)(((double)(vt.fatality[i] + 1) / 6 * total)) / allMap) * totalPeople; // 감염자수에서 사망자수 제외
    }

}


void virusUp() {
    // 30초마다 업데이트로 해놓음 수정하쟈
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
    logIn("플레이어 배가 파괴되었습니다.");
    logIn("20분 후 재생성됩니다.");
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
        logIn("배가 재생성 되었습니다.");
    }
    else if (die == 1) {
        SetCurrentCursorPos(head->next->startPos.X, head->next->startPos.Y);
        DeleteShip(head->next);
        if (World[head->next->startPos.Y][head->next->startPos.X / 2] == head->next->type) {     //바다면
            World[head->next->startPos.Y][head->next->startPos.X / 2] = 5;     //type으로 바꿈
        }
        SetCurrentCursorPos(96, 47);
        Colorset(black, red);
        printf("                                              ");
        SetCurrentCursorPos(96, 47);
        printf("배 파괴됨 /// 복구까지 남은시간 : %d분", (int)((dieTime + 100 - vt.min) / 4));
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
        vt.fatality[currentWaterLv] = 0; // 현재 업그레이드 중인 대륙들에 대한 치사율 0단계로
        currentBlood = 0;

    }
    if (temp == 1) {
        vt.fatality[currentWaterLv] += 2; // 현재 업그레이드 중인 대륙들에 대한 치사율 2단계 업그레이드 / 물 업그레이드 시 리셋
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