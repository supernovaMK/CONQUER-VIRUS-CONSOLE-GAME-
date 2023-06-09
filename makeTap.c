#include "makeTap.h"
#include "data.h"
void removeTap() {
    SetCurrentCursorPos(5, 45);
    Colorset(black, white);
    for (int i = 0; i < 63; i++) {
        for (int j = 0; j < 14; j++) {
            SetCurrentCursorPos(5 + i, 45 + j);
            printf(" ");
        }
    }
}

void initTap() {
    for (int i = 0; i < 5; i++) { vt.fatality[i] = 0; }
    for (int i = 0; i < 5; i++) { vt.propagation[i] = 0; }
    st.power = 0;
    st.health = 0;
    st.speed = 0;
    rt.animal = 0;
    rt.blood = 0;
    rt.air = 0;
    rt.water = 0;
    rt.port = 0;
    it.oil = 0;
    it.jamming = 0;
    it.toground = 0;
    it.engine = 0;
    it.hostage = 0;
}
void drawBox(int fstPosX, int fstPosY, int width, int height) {
    SetCurrentCursorPos(fstPosX, fstPosY);
    for (int i = 0; i < width; i++) {
        SetCurrentCursorPos(fstPosX + i, fstPosY);
        if (i == 0) {
            printf("┌");
        }
        else if (i == width - 1) {
            printf("┐");
        }
        else {
            printf("─");
        }
    }
    for (int i = 0; i < height; i++) {
        SetCurrentCursorPos(fstPosX, fstPosY + i + 1);
        printf("│");
        SetCurrentCursorPos(fstPosX + width - 1, fstPosY + i + 1);
        printf("│");
    }
    for (int i = 0; i < width; i++) {
        SetCurrentCursorPos(fstPosX + i, fstPosY + height);
        if (i == 0) {
            printf("└");
        }
        else if (i == width - 1) {
            printf("┘");
        }
        else {
            printf("─");
        }
    }
}
//void colorChangeVirusTap(int i, int j) {
//    removeTap();
//    if (i == 0) {
//        Colorset(black, sky);
//        drawBox(10, 47, 16, 6);
//        SetCurrentCursorPos(17, 48);
//        if (currentPropagation < 2) {
//            printf("%d G", goldPropagation[currentPropagation]);
//        }
//        else {
//            printf("불가");
//        }
//        SetCurrentCursorPos(15, 50);
//        printf("공 격 력");
//        SetCurrentCursorPos(16, 52);
//        if (st.power != 10)
//            printf("Lv. %d", st.power);
//        else
//            printf("Lv. MAX");
//        Colorset(black, white);
//        drawBox(30, 47, 16, 6);
//        SetCurrentCursorPos(37, 48);
//        if (st.health != 10)
//            printf("%d G", goldHealth[st.health]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(35, 50);
//        printf("내 구 력");
//        SetCurrentCursorPos(36, 52);
//        if (st.health != 10)
//            printf("Lv. %d", st.health);
//        else
//            printf("Lv. MAX");
//        drawBox(50, 47, 16, 6);
//        SetCurrentCursorPos(57, 48);
//        if (st.speed != 5)
//            printf("%d G", goldSpeed[st.speed]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(56, 50);
//        printf("속 도");
//        SetCurrentCursorPos(56, 52);
//        if (st.speed != 5)       //최고 레벨이 아니면
//            printf("Lv. %d", st.speed);     //스피드 고정
//        else {
//            printf("Lv. MAX");
//        }
//    }
//    else if (i == 1) {
//        Colorset(black, white);
//        drawBox(10, 47, 16, 6);
//        SetCurrentCursorPos(17, 48);
//        if (st.power != 10)
//            printf("%d G", goldPower[st.power]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(15, 50);
//        printf("공 격 력");
//        SetCurrentCursorPos(16, 52);
//        if (st.power != 10)
//            printf("Lv. %d", st.power);
//        else
//            printf("Lv. MAX");
//        Colorset(black, sky);
//        drawBox(30, 47, 16, 6);
//        SetCurrentCursorPos(37, 48);
//        if (st.health != 10)
//            printf("%d G", goldHealth[st.health]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(35, 50);
//        printf("내 구 력");
//        SetCurrentCursorPos(36, 52);
//        if (st.health != 10)
//            printf("Lv. %d", st.health);
//        else
//            printf("Lv. MAX");
//        Colorset(black, white);
//        drawBox(50, 47, 16, 6);
//        SetCurrentCursorPos(57, 48);
//        if (st.speed != 5)
//            printf("%d G", goldSpeed[st.speed]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(56, 50);
//        printf("속 도");
//        SetCurrentCursorPos(56, 52);
//        if (st.speed != 5)       //최고 레벨이 아니면
//            printf("Lv. %d", st.speed);     //스피드 고정
//        else {
//            printf("Lv. MAX");
//        }
//    }
//    else if (i == 2) {
//        Colorset(black, white);
//        drawBox(10, 47, 16, 6);
//        SetCurrentCursorPos(17, 48);
//        if (st.power != 10)
//            printf("%d G", goldPower[st.power]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(15, 50);
//        printf("공 격 력");
//        SetCurrentCursorPos(16, 52);
//        if (st.power != 10)
//            printf("Lv. %d", st.power);
//        else
//            printf("Lv. MAX");
//        drawBox(30, 47, 16, 6);
//        SetCurrentCursorPos(37, 48);
//        if (st.health != 10)
//            printf("%d G", goldHealth[st.health]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(35, 50);
//        printf("내 구 력");
//        SetCurrentCursorPos(36, 52);
//        if (st.health != 10)
//            printf("Lv. %d", st.health);
//        else
//            printf("Lv. MAX");
//        Colorset(black, sky);
//        drawBox(50, 47, 16, 6);
//        SetCurrentCursorPos(57, 48);
//        if (st.speed != 5)
//            printf("%d G", goldSpeed[st.speed]);
//        else
//            printf("불가");
//        SetCurrentCursorPos(56, 50);
//        printf("속 도");
//        SetCurrentCursorPos(56, 52);
//        if (st.speed != 5)       //최고 레벨이 아니면
//            printf("Lv. %d", st.speed);     //스피드 고정
//        else {
//            printf("Lv. MAX");
//        }
//    }
//    Colorset(black, white);
//
//}
void colorChangeVirusTap(int i, int j) {
    removeTap();
    if (i == 0) {
        Colorset(black, sky);
        drawBox(10, 47, 16, 6);
        SetCurrentCursorPos(17, 48);
        if (currentPropagation < 2) {
            printf("%d G", goldPropagation[currentPropagation]);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(15, 50);
        printf("전 파 력");
        SetCurrentCursorPos(16, 52);
        if (vt.propagation[j] - rt.animal < 2) {
            printf("Lv. %d", vt.propagation[j] - rt.animal);
        }
        else { printf("Lv. MAX"); }
        Colorset(black, white);
        drawBox(30, 47, 16, 6);
        SetCurrentCursorPos(37, 48);
        if (currentFatality < 2) {
            printf("%d G", goldFatality[currentFatality]);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(35, 50);
        printf("치 사 율");
        SetCurrentCursorPos(36, 52);
        if (currentBlood == 1 && vt.fatality[j] - rt.blood * 2 < 2) {
            printf("Lv. %d", vt.fatality[j] - rt.blood * 2);
        }
        else if (currentBlood != 1 && vt.fatality[j] < 2) {
            printf("Lv. %d", vt.fatality[j]);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(50, 47, 16, 6);
        SetCurrentCursorPos(57, 48);
        if (vt.goldup != 2)
            printf("%d G", goldGoldup[vt.goldup]);
        else
            printf("불가");
        SetCurrentCursorPos(53, 50);
        printf("재화 획득량");
        SetCurrentCursorPos(56, 52);
        if (vt.goldup != 2)       //최고 레벨이 아니면
            printf("Lv. %d", vt.goldup);     //스피드 고정
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 1) {
        Colorset(black, white);
        drawBox(10, 47, 16, 6);
        SetCurrentCursorPos(17, 48);
        if (currentPropagation < 2) {
            printf("%d G", goldPropagation[currentPropagation]);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(15, 50);
        printf("전 파 력");
        SetCurrentCursorPos(16, 52);
        if (vt.propagation[j] - rt.animal < 2) {
            printf("Lv. %d", vt.propagation[j] - rt.animal);
        }
        else { printf("Lv. MAX"); }
        Colorset(black, sky);
        drawBox(30, 47, 16, 6);
        SetCurrentCursorPos(37, 48);
        if (currentFatality < 2) {
            printf("%d G", goldFatality[currentFatality]);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(35, 50);
        printf("치 사 율");
        SetCurrentCursorPos(36, 52);
        if (currentBlood == 1 && vt.fatality[j] - rt.blood * 2 < 2) {
            printf("Lv. %d", vt.fatality[j] - rt.blood * 2);
        }
        else if (currentBlood != 1 && vt.fatality[j] < 2) {
            printf("Lv. %d", vt.fatality[j]);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, white);
        drawBox(50, 47, 16, 6);
        SetCurrentCursorPos(57, 48);
        if (vt.goldup != 2)
            printf("%d G", goldGoldup[vt.goldup]);
        else
            printf("불가");
        SetCurrentCursorPos(53, 50);
        printf("재화 획득량");
        SetCurrentCursorPos(56, 52);
        if (vt.goldup != 2)       //최고 레벨이 아니면
            printf("Lv. %d", vt.goldup);     //스피드 고정
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 2) {
        Colorset(black, white);
        drawBox(10, 47, 16, 6);
        SetCurrentCursorPos(17, 48);
        if (currentPropagation < 2) {
            printf("%d G", goldPropagation[currentPropagation]);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(15, 50);
        printf("전 파 력");
        SetCurrentCursorPos(16, 52);
        if (vt.propagation[j] - rt.animal < 2) {
            printf("Lv. %d", vt.propagation[j] - rt.animal);
        }
        else { printf("Lv. MAX"); }
        drawBox(30, 47, 16, 6);
        SetCurrentCursorPos(37, 48);
        if (currentFatality < 2) {
            printf("%d G", goldFatality[currentFatality]);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(35, 50);
        printf("치 사 율");
        SetCurrentCursorPos(36, 52);
        if (currentBlood == 1 && vt.fatality[j] - rt.blood * 2 < 2) {
            printf("Lv. %d", vt.fatality[j] - rt.blood * 2);
        }
        else if (currentBlood != 1 && vt.fatality[j] < 2) {
            printf("Lv. %d", vt.fatality[j]);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, sky);
        drawBox(50, 47, 16, 6);
        SetCurrentCursorPos(57, 48);
        if (vt.goldup != 2)
            printf("%d G", goldGoldup[vt.goldup]);
        else
            printf("불가");
        SetCurrentCursorPos(53, 50);
        printf("재화 획득량");
        SetCurrentCursorPos(56, 52);
        if (vt.goldup != 2)       //최고 레벨이 아니면
            printf("Lv. %d", vt.goldup);     //스피드 고정
        else {
            printf("Lv. MAX");
        }
    }
    Colorset(black, white);

}
void colorChangeRouteTap(int i) {
    removeTap();
    if (i == 0) {
        Colorset(black, sky);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(17, 46);
        if (rt.animal < 1) {
            printf("%d G", goldAnimal);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(16, 48);
        printf("동 물");
        SetCurrentCursorPos(16, 50);
        if (rt.animal < 1) {
            printf("Lv. %d", rt.animal);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, white);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(37, 46);
        if (rt.blood < 1) {
            printf("%d G", goldBlood);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(36, 48);
        printf("혈 액");
        SetCurrentCursorPos(36, 50);
        if (rt.blood < 1) {
            printf("Lv. %d", rt.blood);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(57, 46);
        if (rt.air < 1) {
            printf("%d G", goldAir);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(56, 48);
        printf("대 기");
        SetCurrentCursorPos(56, 50);
        if (rt.air < 1) {
            printf("Lv. %d", rt.air);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(26, 53);
        if (rt.water != 4)
            printf("%d G", goldWater[rt.water]);
        else
            printf("불가");
        SetCurrentCursorPos(27, 55);
        printf("물");
        SetCurrentCursorPos(25, 57);
        if (rt.water != 4)
            printf("Lv. %d", rt.water);
        else {
            printf("Lv. MAX");
        }
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(47, 53);
        if (rt.port != 5)
            printf("%d G", goldHealing[rt.port]);
        else
            printf("불가");
        SetCurrentCursorPos(45, 55);
        printf("선 착 장");
        SetCurrentCursorPos(46, 57);
        if (rt.port != 5)
            printf("Lv. %d", rt.port);
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 1) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(17, 46);
        if (rt.animal < 1) {
            printf("%d G", goldAnimal);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(16, 48);
        printf("동 물");
        SetCurrentCursorPos(16, 50);
        if (rt.animal < 1) {
            printf("Lv. %d", rt.animal);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, sky);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(37, 46);
        if (rt.blood < 1) {
            printf("%d G", goldBlood);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(36, 48);
        printf("혈 액");
        SetCurrentCursorPos(36, 50);
        if (rt.blood < 1) {
            printf("Lv. %d", rt.blood);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, white);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(57, 46);
        if (rt.air < 1) {
            printf("%d G", goldAir);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(56, 48);
        printf("대 기");
        SetCurrentCursorPos(56, 50);
        if (rt.air < 1) {
            printf("Lv. %d", rt.air);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(26, 53);
        if (rt.water != 4)
            printf("%d G", goldWater[rt.water]);
        else
            printf("불가");
        SetCurrentCursorPos(27, 55);
        printf("물");
        SetCurrentCursorPos(25, 57);
        if (rt.water != 4)
            printf("Lv. %d", rt.water);
        else {
            printf("Lv. MAX");
        }
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(47, 53);
        if (rt.port != 5)
            printf("%d G", goldHealing[rt.port]);
        else
            printf("불가");
        SetCurrentCursorPos(45, 55);
        printf("선 착 장");
        SetCurrentCursorPos(46, 57);
        if (rt.port != 5)
            printf("Lv. %d", rt.port);
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 2) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(17, 46);
        if (rt.animal < 1) {
            printf("%d G", goldAnimal);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(16, 48);
        printf("동 물");
        SetCurrentCursorPos(16, 50);
        if (rt.animal < 1) {
            printf("Lv. %d", rt.animal);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(37, 46);
        if (rt.blood < 1) {
            printf("%d G", goldBlood);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(36, 48);
        printf("혈 액");
        SetCurrentCursorPos(36, 50);
        if (rt.blood < 1) {
            printf("Lv. %d", rt.blood);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, sky);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(57, 46);
        if (rt.air < 1) {
            printf("%d G", goldAir);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(56, 48);
        printf("대 기");
        SetCurrentCursorPos(56, 50);
        if (rt.air < 1) {
            printf("Lv. %d", rt.air);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, white);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(26, 53);
        if (rt.water != 4)
            printf("%d G", goldWater[rt.water]);
        else
            printf("불가");
        SetCurrentCursorPos(27, 55);
        printf("물");
        SetCurrentCursorPos(25, 57);
        if (rt.water != 4)
            printf("Lv. %d", rt.water);
        else {
            printf("Lv. MAX");
        }
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(47, 53);
        if (rt.port != 5)
            printf("%d G", goldHealing[rt.port]);
        else
            printf("불가");
        SetCurrentCursorPos(45, 55);
        printf("선 착 장");
        SetCurrentCursorPos(46, 57);
        if (rt.port != 5)
            printf("Lv. %d", rt.port);
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 3) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(17, 46);
        if (rt.animal < 1) {
            printf("%d G", goldAnimal);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(16, 48);
        printf("동 물");
        SetCurrentCursorPos(16, 50);
        if (rt.animal < 1) {
            printf("Lv. %d", rt.animal);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(37, 46);
        if (rt.blood < 1) {
            printf("%d G", goldBlood);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(36, 48);
        printf("혈 액");
        SetCurrentCursorPos(36, 50);
        if (rt.blood < 1) {
            printf("Lv. %d", rt.blood);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(57, 46);
        if (rt.air < 1) {
            printf("%d G", goldAir);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(56, 48);
        printf("대 기");
        SetCurrentCursorPos(56, 50);
        if (rt.air < 1) {
            printf("Lv. %d", rt.air);
        }
        else {
            printf("Lv. MAX");
        }
        Colorset(black, sky);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(26, 53);
        if (rt.water != 4)
            printf("%d G", goldWater[rt.water]);
        else
            printf("불가");
        SetCurrentCursorPos(27, 55);
        printf("물");
        SetCurrentCursorPos(25, 57);
        if (rt.water != 4)
            printf("Lv. %d", rt.water);
        else {
            printf("Lv. MAX");
        }
        Colorset(black, white);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(47, 53);
        if (rt.port != 5)
            printf("%d G", goldHealing[rt.port]);
        else
            printf("불가");
        SetCurrentCursorPos(45, 55);
        printf("선 착 장");
        SetCurrentCursorPos(46, 57);
        if (rt.port != 5)
            printf("Lv. %d", rt.port);
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 4) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(17, 46);
        if (rt.animal < 1) {
            printf("%d G", goldAnimal);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(16, 48);
        printf("동 물");
        SetCurrentCursorPos(16, 50);
        if (rt.animal < 1) {
            printf("Lv. %d", rt.animal);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(37, 46);
        if (rt.blood < 1) {
            printf("%d G", goldBlood);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(36, 48);
        printf("혈 액");
        SetCurrentCursorPos(36, 50);
        if (rt.blood < 1) {
            printf("Lv. %d", rt.blood);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(57, 46);
        if (rt.air < 1) {
            printf("%d G", goldAir);
        }
        else {
            printf("불가");
        }
        SetCurrentCursorPos(56, 48);
        printf("대 기");
        SetCurrentCursorPos(56, 50);
        if (rt.air < 1) {
            printf("Lv. %d", rt.air);
        }
        else {
            printf("Lv. MAX");
        }
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(26, 53);
        if (rt.water != 4)
            printf("%d G", goldWater[rt.water]);
        else
            printf("불가");
        SetCurrentCursorPos(27, 55);
        printf("물");
        SetCurrentCursorPos(25, 57);
        if (rt.water != 4)
            printf("Lv. %d", rt.water);
        else {
            printf("Lv. MAX");
        }
        Colorset(black, sky);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(47, 53);
        if (rt.port != 5)
            printf("%d G", goldHealing[rt.port]);
        else
            printf("불가");
        SetCurrentCursorPos(45, 55);
        printf("선 착 장");
        SetCurrentCursorPos(46, 57);
        if (rt.port != 5)
            printf("Lv. %d", rt.port);
        else {
            printf("Lv. MAX");
        }
    }
}

void colorChangeShipTap(int i) {
    removeTap();
    if (i == 0) {
        Colorset(black, sky);
        drawBox(10, 47, 16, 6);
        SetCurrentCursorPos(17, 48);
        if (st.power != 10)
            printf("%d G", goldPower[st.power]);
        else
            printf("불가");
        SetCurrentCursorPos(15, 50);
        printf("공 격 력");
        SetCurrentCursorPos(16, 52);
        if (st.power != 10)
            printf("Lv. %d", st.power);
        else
            printf("Lv. MAX");
        Colorset(black, white);
        drawBox(30, 47, 16, 6);
        SetCurrentCursorPos(37, 48);
        if (st.health != 10)
            printf("%d G", goldHealth[st.health]);
        else
            printf("불가");
        SetCurrentCursorPos(35, 50);
        printf("내 구 력");
        SetCurrentCursorPos(36, 52);
        if (st.health != 10)
            printf("Lv. %d", st.health);
        else
            printf("Lv. MAX");
        drawBox(50, 47, 16, 6);
        SetCurrentCursorPos(57, 48);
        if (st.speed != 5)
            printf("%d G", goldSpeed[st.speed]);
        else
            printf("불가");
        SetCurrentCursorPos(56, 50);
        printf("속 도");
        SetCurrentCursorPos(56, 52);
        if (st.speed != 5)       //최고 레벨이 아니면
            printf("Lv. %d", st.speed);     //스피드 고정
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 1) {
        Colorset(black, white);
        drawBox(10, 47, 16, 6);
        SetCurrentCursorPos(17, 48);
        if (st.power != 10)
            printf("%d G", goldPower[st.power]);
        else
            printf("불가");
        SetCurrentCursorPos(15, 50);
        printf("공 격 력");
        SetCurrentCursorPos(16, 52);
        if (st.power != 10)
            printf("Lv. %d", st.power);
        else
            printf("Lv. MAX");
        Colorset(black, sky);
        drawBox(30, 47, 16, 6);
        SetCurrentCursorPos(37, 48);
        if (st.health != 10)
            printf("%d G", goldHealth[st.health]);
        else
            printf("불가");
        SetCurrentCursorPos(35, 50);
        printf("내 구 력");
        SetCurrentCursorPos(36, 52);
        if (st.health != 10)
            printf("Lv. %d", st.health);
        else
            printf("Lv. MAX");
        Colorset(black, white);
        drawBox(50, 47, 16, 6);
        SetCurrentCursorPos(57, 48);
        if (st.speed != 5)
            printf("%d G", goldSpeed[st.speed]);
        else
            printf("불가");
        SetCurrentCursorPos(56, 50);
        printf("속 도");
        SetCurrentCursorPos(56, 52);
        if (st.speed != 5)       //최고 레벨이 아니면
            printf("Lv. %d", st.speed);     //스피드 고정
        else {
            printf("Lv. MAX");
        }
    }
    else if (i == 2) {
        Colorset(black, white);
        drawBox(10, 47, 16, 6);
        SetCurrentCursorPos(17, 48);
        if (st.power != 10)
            printf("%d G", goldPower[st.power]);
        else
            printf("불가");
        SetCurrentCursorPos(15, 50);
        printf("공 격 력");
        SetCurrentCursorPos(16, 52);
        if (st.power != 10)
            printf("Lv. %d", st.power);
        else
            printf("Lv. MAX");
        drawBox(30, 47, 16, 6);
        SetCurrentCursorPos(37, 48);
        if (st.health != 10)
            printf("%d G", goldHealth[st.health]);
        else
            printf("불가");
        SetCurrentCursorPos(35, 50);
        printf("내 구 력");
        SetCurrentCursorPos(36, 52);
        if (st.health != 10)
            printf("Lv. %d", st.health);
        else
            printf("Lv. MAX");
        Colorset(black, sky);
        drawBox(50, 47, 16, 6);
        SetCurrentCursorPos(57, 48);
        if (st.speed != 5)
            printf("%d G", goldSpeed[st.speed]);
        else
            printf("불가");
        SetCurrentCursorPos(56, 50);
        printf("속 도");
        SetCurrentCursorPos(56, 52);
        if (st.speed != 5)       //최고 레벨이 아니면
            printf("Lv. %d", st.speed);     //스피드 고정
        else {
            printf("Lv. MAX");
        }
    }
    Colorset(black, white);

}
void colorChangeItemTap(int i) {
    removeTap();
    if (i == 0) {
        Colorset(black, sky);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(13, 47);
        printf("가짜기름유포");
        SetCurrentCursorPos(17, 49);
        printf("%d 개", itemList[0]);
        Colorset(black, white);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(35, 47);
        printf("긴급회복");
        SetCurrentCursorPos(37, 49);
        printf("%d 개", itemList[1]);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(54, 47);
        printf("속도 최대");
        SetCurrentCursorPos(57, 49);
        printf("%d 개", itemList[2]);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(25, 54);
        printf("어    뢰");
        SetCurrentCursorPos(27, 56);
        printf("%d 개", itemList[3]);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(45, 54);
        printf("인 질 극");
        SetCurrentCursorPos(47, 56);
        printf("%d 개", itemList[4]);
    }
    else if (i == 1) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(13, 47);
        printf("가짜기름유포");
        SetCurrentCursorPos(17, 49);
        printf("%d 개", itemList[0]);
        Colorset(black, sky);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(35, 47);
        printf("긴급회복");
        SetCurrentCursorPos(37, 49);
        printf("%d 개", itemList[1]);
        Colorset(black, white);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(54, 47);
        printf("속도 최대");
        SetCurrentCursorPos(57, 49);
        printf("%d 개", itemList[2]);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(25, 54);
        printf("어    뢰");
        SetCurrentCursorPos(27, 56);
        printf("%d 개", itemList[3]);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(45, 54);
        printf("인 질 극");
        SetCurrentCursorPos(47, 56);
        printf("%d 개", itemList[4]);
    }
    else if (i == 2) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(13, 47);
        printf("가짜기름유포");
        SetCurrentCursorPos(17, 49);
        printf("%d 개", itemList[0]);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(35, 47);
        printf("긴급회복");
        SetCurrentCursorPos(37, 49);
        printf("%d 개", itemList[1]);
        Colorset(black, sky);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(54, 47);
        printf("속도 최대");
        SetCurrentCursorPos(57, 49);
        printf("%d 개", itemList[2]);
        Colorset(black, white);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(25, 54);
        printf("어    뢰");
        SetCurrentCursorPos(27, 56);
        printf("%d 개", itemList[3]);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(45, 54);
        printf("인 질 극");
        SetCurrentCursorPos(47, 56);
        printf("%d 개", itemList[4]);
    }
    else if (i == 3) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(13, 47);
        printf("가짜기름유포");
        SetCurrentCursorPos(17, 49);
        printf("%d 개", itemList[0]);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(35, 47);
        printf("긴급회복");
        SetCurrentCursorPos(37, 49);
        printf("%d 개", itemList[1]);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(54, 47);
        printf("속도 최대");
        SetCurrentCursorPos(57, 49);
        printf("%d 개", itemList[2]);
        Colorset(black, sky);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(25, 54);
        printf("어    뢰");
        SetCurrentCursorPos(27, 56);
        printf("%d 개", itemList[3]);
        Colorset(black, white);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(45, 54);
        printf("인 질 극");
        SetCurrentCursorPos(47, 56);
        printf("%d 개", itemList[4]);
    }
    else if (i == 4) {
        Colorset(black, white);
        drawBox(10, 45, 16, 6);
        SetCurrentCursorPos(13, 47);
        printf("가짜기름유포");
        SetCurrentCursorPos(17, 49);
        printf("%d 개", itemList[0]);
        drawBox(30, 45, 16, 6);
        SetCurrentCursorPos(35, 47);
        printf("긴급회복");
        SetCurrentCursorPos(37, 49);
        printf("%d 개", itemList[1]);
        drawBox(50, 45, 16, 6);
        SetCurrentCursorPos(54, 47);
        printf("속도 최대");
        SetCurrentCursorPos(57, 49);
        printf("%d 개", itemList[2]);
        drawBox(20, 52, 16, 6);
        SetCurrentCursorPos(25, 54);
        printf("어    뢰");
        SetCurrentCursorPos(27, 56);
        printf("%d 개", itemList[3]);
        Colorset(black, sky);
        drawBox(40, 52, 16, 6);
        SetCurrentCursorPos(45, 54);
        printf("인 질 극");
        SetCurrentCursorPos(47, 56);
        printf("%d 개", itemList[4]);
    }
    Colorset(black, white);
}

void selectedTap(int i) {
    if (i == 0) {
        removeTap();
        Colorset(black, brightYellow);
        drawBox(10, 42, 12, 2);
        SetCurrentCursorPos(13, 43);
        printf("바이러스");
        Colorset(black, white);
        drawBox(25, 42, 12, 2);
        SetCurrentCursorPos(28, 43);
        printf("감염경로");
        drawBox(40, 42, 12, 2);
        SetCurrentCursorPos(43, 43);
        printf("배 강화");
        drawBox(55, 42, 12, 2);
        SetCurrentCursorPos(59, 43);
        printf("아이템");
    }
    else if (i == 1) {
        removeTap();
        Colorset(black, white);
        drawBox(10, 42, 12, 2);
        SetCurrentCursorPos(13, 43);
        printf("바이러스");
        Colorset(black, brightYellow);
        drawBox(25, 42, 12, 2);
        SetCurrentCursorPos(28, 43);
        printf("감염경로");
        Colorset(black, white);
        drawBox(40, 42, 12, 2);
        SetCurrentCursorPos(43, 43);
        printf("배 강화");
        drawBox(55, 42, 12, 2);
        SetCurrentCursorPos(59, 43);
        printf("아이템");
    }
    else if (i == 2) {
        removeTap();
        Colorset(black, white);
        drawBox(10, 42, 12, 2);
        SetCurrentCursorPos(13, 43);
        printf("바이러스");
        drawBox(25, 42, 12, 2);
        SetCurrentCursorPos(28, 43);
        printf("감염경로");
        Colorset(black, brightYellow);
        drawBox(40, 42, 12, 2);
        SetCurrentCursorPos(43, 43);
        printf("배 강화");
        Colorset(black, white);
        drawBox(55, 42, 12, 2);
        SetCurrentCursorPos(59, 43);
        printf("아이템");
    }
    else if (i == 3) {
        removeTap();
        Colorset(black, white);
        drawBox(10, 42, 12, 2);
        SetCurrentCursorPos(13, 43);
        printf("바이러스");
        drawBox(25, 42, 12, 2);
        SetCurrentCursorPos(28, 43);
        printf("감염경로");
        drawBox(40, 42, 12, 2);
        SetCurrentCursorPos(43, 43);
        printf("배 강화");
        Colorset(black, brightYellow);
        drawBox(55, 42, 12, 2);
        SetCurrentCursorPos(59, 43);
        printf("아이템");
    }
    Colorset(black, white);
}

int drawFirstTap() {
    Colorset(black, white);
    int key = 0;
    int check = 0;
    SetCurrentCursorPos(2, 40);
    int curPosX = 2;
    int curPosY = 40;
    for (int i = 0; i < TAP_WIDTH; i++) {
        SetCurrentCursorPos(curPosX + i * 2, curPosY);
        printf("─");
    }
    for (int i = 0; i < TAP_WIDTH; i++) {
        SetCurrentCursorPos(curPosX + i * 2, curPosY + TAP_HEIGHT - 1);
        printf("─");
    }
    for (int i = 0; i < TAP_HEIGHT; i++) {
        SetCurrentCursorPos(curPosX, curPosY + i);
        if (i == 0) {
            printf("┌");
        }
        else if (i == TAP_HEIGHT - 1) {
            printf("└");
        }
        else printf("│");
    }
    for (int i = 1; i < TAP_HEIGHT - 1; i++) {
        SetCurrentCursorPos(curPosX + TAP_WIDTH - 1, curPosY + i);
        printf("│");
    }
    for (int i = 0; i < TAP_HEIGHT; i++) {
        SetCurrentCursorPos(curPosX + (TAP_WIDTH - 1) * 2, curPosY + i);
        if (i == 0) {
            printf("┐");
        }
        else if (i == TAP_HEIGHT - 1) {
            printf("┘");
        }
        else printf("│");
    }

    selectedTap(0);
}
/*Time 관련*/
void printTime() {
    Colorset(black, white);
    SetCurrentCursorPos(126, 39);
    printf("                              ");
    SetCurrentCursorPos(126, 39);
    printf("현재시간 : %d일 %d시간 %d분", vt.min / 5760, vt.min % 5760 / 240, vt.min % 240 / 4);
}

/*Gold 관련*/
void printGold() {
    Colorset(black, white);
    SetCurrentCursorPos(76, 39);
    printf("         ");
    SetCurrentCursorPos(76, 39);
    printf("%dG", vt.gold);
}

void makeExplane() {
    SetCurrentCursorPos(156, 0);
    printf("상황판");
    drawBox(153, 1, 45, 21);
    SetCurrentCursorPos(156, 23);
    printf("유저 정보");
    drawBox(153, 24, 45, 14);
    SetCurrentCursorPos(156, 39);
    printf("강화 설명");
    drawBox(153, 40, 45, 19);


}
void printUpEx() {
    printuserInformation();
    Colorset(black, white);
    for (int i = 0; i < 17; i++) {
        SetCurrentCursorPos(156, 42 + i);
        printf("                                         ");
    }
    if (currentTap == 0) {
        if (check == 0) {
            SetCurrentCursorPos(156, 42);
            Colorset(black, brightYellow);
            printf("바이러스 업그레이드");
            Colorset(black, white);
            SetCurrentCursorPos(156, 44);
            printf("▷ 바이러스의 전파력, 살상력 같은 것들을 ");
            SetCurrentCursorPos(156, 45);
            printf("   강화합니다.");
        }
        else {
            if (currentvt == 0) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("전파력 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 바이러스가 퍼지는 속도를 더욱 ");
                SetCurrentCursorPos(156, 45);
                printf("   빠르게 합니다.");
                SetCurrentCursorPos(156, 48);
                Colorset(black, red);
                printf("※ 목표는 사망자수를 늘리는 것임을");
                SetCurrentCursorPos(156, 49);
                printf("   잊지마십시오.");
            }
            else if (currentvt == 1) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("치사율 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 바이러스에 감염된 사람이 사망에 이를 ");
                SetCurrentCursorPos(156, 45);
                printf("   확률을 더욱 크게 만들 수 있습니다.");
                SetCurrentCursorPos(156, 48);
                Colorset(black, red);
                printf("※ 사망자수는 목표에 다다르는 길이지만");
                SetCurrentCursorPos(156, 49);
                printf("   사망한 사망자수는 감염자에 포함되지");
                SetCurrentCursorPos(156, 50);
                printf("   않아 퍼지는 속도가 느려질 수 있습니다.");
            }
            else if (currentvt == 2) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("재화 획득량 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 백신 배를 부술 때 획득하는 재화량을 ");
                SetCurrentCursorPos(156, 45);
                printf("   두 배로 업데이트할 수 있습니다.");
                SetCurrentCursorPos(156, 48);
                Colorset(black, green);
                printf("   0레벨 - 10G 획득");
                SetCurrentCursorPos(156, 49);
                printf("   1레벨 - 20G 획득");
                SetCurrentCursorPos(156, 50);
                printf("   2레벨 - 40G 획득");
            }
        }
    }
    else if (currentTap == 1) {
        if (check == 0) {
            SetCurrentCursorPos(156, 42);
            Colorset(black, brightYellow);
            printf("감염경로 업그레이드");
            Colorset(black, white);
            SetCurrentCursorPos(156, 44);
            printf("▷ 바이러스에 관련된 부가적인 요소");
            SetCurrentCursorPos(156, 45);
            printf("   (타 대륙 감염, 적 약화, 회복량증가)");
            SetCurrentCursorPos(156, 46);
            printf("   를 강화합니다.");
        }
        else {
            if (currentrt == 0) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("동물 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 해당 대륙 내부에서의 전파력을");
                SetCurrentCursorPos(156, 45);
                printf("    강화합니다.");
                SetCurrentCursorPos(156, 48);
                Colorset(black, red);
                printf("※ 해당 대륙은 해금된 모든 대륙을");
                SetCurrentCursorPos(156, 49);
                printf("   말하며, 물 강화 시 초기화됩니다.");
            }
            else if (currentrt == 1) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("혈액 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 해당 대륙 내부에서의 바이러스에 ");
                SetCurrentCursorPos(156, 45);
                printf("   돌연변이를 생성해, 살상력을 랜덤");
                SetCurrentCursorPos(156, 46);
                printf("   하게 상승시킵니다.");
                SetCurrentCursorPos(156, 49);
                Colorset(black, red);
                printf("※ 해당 대륙은 해금된 모든 대륙을");
                SetCurrentCursorPos(156, 50);
                printf("   말하며, 물 강화 시 초기화됩니다.");
            }
            else if (currentrt == 2) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("대기 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 바이러스의 전파력의 일부분이 타 대륙");
                SetCurrentCursorPos(156, 45);
                printf("   의 감염자수에 약간 영향받습니다.");
                SetCurrentCursorPos(156, 48);
                Colorset(black, red);
                printf("※ 대기를 강화하지 않을 경우 해당 대륙");
                SetCurrentCursorPos(156, 49);
                printf("   의 감염자수에만 영향을 받으며");
                SetCurrentCursorPos(156, 50);
                printf("   강화할 경우 전체 감염자수에 약간");
                SetCurrentCursorPos(156, 51);
                printf("   영향받습니다.");
            }
            else if (currentrt == 3) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("물 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 전파력이 작용할 수 있는 다양한");
                SetCurrentCursorPos(156, 45);
                printf("   대륙을 해금합니다.");
                SetCurrentCursorPos(156, 48);
                Colorset(black, green);
                printf("   0레벨 - 오세아니아");
                SetCurrentCursorPos(156, 49);
                printf("   1레벨 - 아프리카 해금");
                SetCurrentCursorPos(156, 50);
                printf("   2레벨 - 남아메리카 해금");
                SetCurrentCursorPos(156, 51);
                printf("   3레벨 - 북아메리카 해금");
                SetCurrentCursorPos(156, 52);
                printf("   4레벨 - 유라시아 해금");
            }
            else if (currentrt == 4) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("선착장 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 각 대륙에 붙어있는 선착장에서 받을 수");
                SetCurrentCursorPos(156, 45);
                printf("   있는 수리 속도를 빠르게 합니다.");
            }
        }
    }
    else if (currentTap == 2) {
        if (check == 0) {
            SetCurrentCursorPos(156, 42);
            Colorset(black, brightYellow);
            printf("배 업그레이드");
            Colorset(black, white);
            SetCurrentCursorPos(156, 44);
            printf("▷ 플레이어가 움직이는 배의 성능을 ");
            SetCurrentCursorPos(156, 45);
            printf("   강화합니다.");
            SetCurrentCursorPos(156, 46);
            printf("   (공격력, 내구력, 속도)");
        }
        else {
            if (currentst == 0) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("공격력 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 적배를 부딪혔을 경우 적배에");
                SetCurrentCursorPos(156, 45);
                printf("   주는 데미지를 강화합니다.");
            }
            else if (currentst == 1) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("내구력 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 플레이어의 체력을 강화하여 적배에");
                SetCurrentCursorPos(156, 45);
                printf("   자주 부딪혀도 침몰하지 않습니다.");
            }
            else if (currentst == 2) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("속도 업그레이드");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 플레이어의 속도를 강화하여 ");
                SetCurrentCursorPos(156, 45);
                printf("   빠르게 이동할 수 있도록 합니다.");
            }
        }
    }
    else if (currentTap == 3) {
        if (check == 0) {
            SetCurrentCursorPos(156, 42);
            Colorset(black, brightYellow);
            printf("아이템 사용");
            Colorset(black, white);
            SetCurrentCursorPos(156, 44);
            printf("▷ 적 배를 침몰시켜서 확률적으로 ");
            SetCurrentCursorPos(156, 45);
            printf("   획득한 아이템을 사용할 수 있는 탭");
        }
        else {
            if (currentit == 0) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("가짜기름유포 사용");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 현재 맵에 있는 모든 배의 속도를 ");
                SetCurrentCursorPos(156, 45);
                printf("   감소시킵니다.");
            }
            else if (currentit == 1) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("긴급회복 사용");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 현재 배의 체력을 100% 회복합니다.");
            }
            else if (currentit == 2) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("속도 최대 사용");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 잠깐 배의 속도를 최대 강화 수치로");
                SetCurrentCursorPos(156, 45);
                printf("   적용시킵니다. (일정시간후 초기화)");
            }
            else if (currentit == 3) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("어뢰 사용");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 맵상에 있는 모든 배를 파괴합니다.");
                SetCurrentCursorPos(156, 47);
                Colorset(black, red);
                printf("※ 부숴진 배의 아래 랜덤으로 아이템 생성");
            }
            else if (currentit == 4) {
                SetCurrentCursorPos(156, 42);
                Colorset(black, sky);
                printf("인질극 사용");
                Colorset(black, white);
                SetCurrentCursorPos(156, 44);
                printf("▷ 배를 부쉈을때 획득할 수 있는 재화");
                SetCurrentCursorPos(156, 45);
                printf("   의 양을 3배 증가시킵니다.");
                SetCurrentCursorPos(156, 47);
                Colorset(black, red);
                printf("※ 일정시간 후 초기화");
                SetCurrentCursorPos(156, 48);
                printf("※ 사용 중 한번 더 사용시 효과는 ");
                SetCurrentCursorPos(156, 49);
                printf("   그대로지만 시간은 추가됨");
            }
        }
    }
}

void printLogEx() {
    for (int i = 0; i < logcnt; i++) {
        if (i % 2 == 0)
            Colorset(black, sky);
        else
            Colorset(black, skyBlue);
        SetCurrentCursorPos(155, 2 + i);
        printf("                                         ");
        SetCurrentCursorPos(155, 2 + i);
        printf("%s", log[i]);
    }
}

void printuserInformation() {
    SetCurrentCursorPos(156, 27);
    Colorset(black, white);
    printf("현재 배 체력 :");
    SetCurrentCursorPos(170, 27);
    printf("                           ");
    SetCurrentCursorPos(170, 27);
    Colorset(black, brightYellow);
    printf("%d / %d", head->next->health, head->next->maxhp);

    SetCurrentCursorPos(156, 30);
    if (itemwork[2] == 1 && vt.min - itemstart2 <= 60) {
        Colorset(black, white);
        printf("속도 최대 아이템 사용중");
        Colorset(black, brightYellow);
        SetCurrentCursorPos(179, 30);
        printf("%d분 남았습니다.", (int)(120 - (vt.min - itemstart2)) / 4);
    }
    else printf("                                       ");

    SetCurrentCursorPos(156, 31);
    if (itemwork[4] == 1 && vt.min - itemstart4 <= 500) {
        Colorset(black, white);
        printf("인질극 아이템 사용중");
        Colorset(black, brightYellow);
        SetCurrentCursorPos(178, 31);
        printf("%d분 남았습니다.", (int)(500 - (vt.min - itemstart4)) / 4);
    }
    else printf("                                       ");
}