#include "./testbase.c"


static int testState = 0;
static int firstCarArriveOrder = 0;
static int firstCarFrom = 0;
static int firstCarSpeed = 0;

void newCar(int delaySec, int from, int speed) {
	
	if (testState == 0) {
		ArriveRoad();  // Car 1
        firstCarArriveOrder = delaySec;
        firstCarFrom = from;
        firstCarSpeed = speed;
		testState = 1;
		
	} else if (testState == 2) {
        Delay(firstCarArriveOrder);
        if (from == 0) {
            DPrintf("Car[%d] arrived WEST\n", 1);
        } else {
            DPrintf("Car[%d] arrived EAST\n", 1);
        }
		driveRoad(1, firstCarFrom, firstCarSpeed);

	} else if (Fork() == 0) {
		int ticket = ArriveRoad();
		Delay(delaySec);
        if (from == 0) {
            DPrintf("Car[%d] arrived WEST\n", ticket);
        } else {
            DPrintf("Car[%d] arrived EAST\n", ticket);
        }
        driveRoad(ticket, from, speed);
		Exit();
    }
}


void test0() {
    // nothing here
}

void initTest() {
    InitRoad();
}

void endTest() {
    if (testState == 1) {
        testState = 2;
        newCar(0, 0, 0);
    }
}


// 单向路上无撞击测试
// Single direction no-collision tests

void test1() {
    // 路上不准撞车: 1快2慢3快4更快5慢6最快
    // No crashes on the road: Car 1 fast, Car 2 slow, Car 3 fast, Car 4 faster, Car 5 slow, Car 6 fastest
    newCar(1, WEST, 900); 
    // 2会把34卡在屁股后面
    // 3 and 4 should go behind 2
    newCar(2, WEST, 500);
    newCar(3, WEST, 600);
    newCar(4, WEST, 800);
    // 5会把6卡在屁股后面
    // 5 should go behind 6
    newCar(5, WEST, 200);
    newCar(6, WEST, 1000);
}



// 以下是单向上路顺序测试
// Single direction Get-On-Road tests


void test2() {
    // 车1可以立即上路，车1开到2后，2来了应该可以立即上路
    // Car 1 can immediately go on the road, after Car 1 reaches Car 2, Car 2 should go immediately
    newCar(0, WEST, 200);
    newCar(20, WEST, 200);

    // 3上路之后4来了,但是4要等3开出第一个车位才能上路
    // After Car 3 goes on the road, Car 4 arrives, but Car 4 must wait for Car 3 to leave the first space
    newCar(50, WEST, 200);
    newCar(51, WEST, 200);

    // 车6会先到,但是它要等5到才能上路
    // Car 6 will arrive first, but it must wait for Car 5 to arrive before it can go on the road
    newCar(100, WEST, 200);
    newCar(80, WEST, 200);
}


void test3() {
    // 车1上路堵着路口，同向来了车24，在前一辆车1开到2之前，后一辆车2不能上路，车2上路，开到2之后，来了3，3应该可以立即上路，然后3开到2，4可以上路
    // Car 1 blocks the intersection, Car 2 can't go on the road before Car 1 reaches Car 2, Car 3 can go immediately after Car 2 leaves
    newCar(0, WEST, 200);
    newCar(5, WEST, 200);
    newCar(40, WEST, 200);
    newCar(5, WEST, 200);
}


void test4() {
    // 车1上路堵着路口，同向来了车43，在2到来之前，没有车可以上路，2来了以后2先上路，然后是3，4
    // Car 1 blocks the intersection, Car 2 can't go on the road until Car 2 arrives, after which Car 2 goes first, then Car 3, and Car 4
    newCar(0, WEST, 200);
    newCar(30, WEST, 200);
    newCar(10, WEST, 200);
    newCar(5, WEST, 200);
}


// 以下是双向交通测试
// Double direction traffic tests


void test5() {
    // 车1上路堵着路口，同向来了车4，对向来了3，同向来了2，然后车1开到2, 对向来了5，上路顺序是13254
    // Car 1 blocks the intersection, Car 4 comes from the same direction, Car 3 comes from the opposite direction, Car 2 arrives in the same direction, then Car 1 goes to Car 2, and Car 5 comes from the opposite direction. The order of going on the road is 1, 3, 2, 5, 4
    newCar(0, WEST, 100);
    newCar(30, WEST, 200);
    newCar(20, EAST, 200);
    newCar(5, WEST, 200);
    newCar(40, EAST, 200);
}


void test6() {
    // 车1上路堵着路口，对向来了车4，然后车1开出车位1，即路口，同向来了2。注意，在3到来之前，2和4都不可以上路，即使1开出去了。
    // 最终上路顺序是1324
    // 注意，"等待"的定义不限于队列里面那些"ready to go"的车，停车场里的也算
    // Car 1 blocks the intersection, Car 4 comes from the opposite direction, then Car 1 leaves the intersection, and Car 2 comes from the same direction. Before Car 3 arrives, neither Car 2 nor Car 4 can go on the road.
    // The final order of going on the road is 1, 3, 2, 4. Note that "waiting" is not limited to cars in the queue, but also those in the parking lot.
    newCar(0, WEST, 200);
    newCar(40, WEST, 200);
    newCar(300, EAST, 200);
    newCar(5, EAST, 200);
}


void test7() {
    // 车1上路堵着路口，同向来了车4，然后车1开到2，对向来了3, 同向来了2，对向来了5，上路顺序是13254
    // Car 1 blocks the intersection, Car 4 comes from the same direction, then Car 1 reaches Car 2, Car 3 comes from the opposite direction, Car 2 comes from the same direction, and Car 5 comes from the opposite direction. The order of going on the road is 1, 3, 2, 5, 4.
    newCar(0, WEST, 200);
    newCar(30, WEST, 200);
    newCar(20, EAST, 200);
    newCar(5, WEST, 200);
    newCar(40, EAST, 200);
}


void test8() {
    // 车1上路堵着路口，对向来了车4，同向来了3，对向来了2，然后同向来了5，上路顺序是12345
    // Car 1 blocks the intersection, Car 4 comes from the opposite direction, Car 3 comes from the same direction, Car 2 comes from the opposite direction, then Car 5 comes from the same direction. The order of going on the road is 1, 2, 3, 4, 5.
    newCar(0, WEST, 200);
    newCar(30, EAST, 200);
    newCar(20, WEST, 200);
    newCar(5, EAST, 200);
    newCar(40, WEST, 200);
}


void test9() {
    // 车123上路(没有堵住路口，或者说车位1), 对向来了车5，同向来了车6，对向来了车7，然后同向来了车4，上路顺序是1235476
    // Cars 1, 2, and 3 go on the road (without blocking the intersection, or Car 1 spot), Car 5 comes from the opposite direction, Car 6 comes from the same direction, Car 7 comes from the opposite direction, then Car 4 comes from the same direction. The order of going on the road is 1, 2, 3, 5, 4, 7, 6.
    newCar(0, WEST, 200);
    newCar(5, WEST, 200);
    newCar(10, WEST, 200);

    newCar(130, WEST, 200);
    newCar(70, EAST, 200);
    newCar(90, WEST, 200);
    newCar(110, EAST, 200);
}


void test10() {
    // 跟9类似：车1先从对向开出去，然后23上路也开出去, 对向来了车5，同向来了6，对向来了7，然后同向来了4
    // Similar to test 9: Car 1 exits from the opposite direction first, then cars 2 and 3 enter and also exit.
    // A car (5) arrives from the opposite direction, followed by car 6 from the same direction, then car 7 from the opposite direction, and finally car 4 from the same direction.
    
    // 但是这回上路顺序会变成1234567 // 参考piazza post 437
    // However, the road entry order this time will be 1-2-3-4-5-6-7 // Refer to Piazza post 437
    newCar(0, EAST, 400);
    newCar(205, WEST, 400);
    newCar(210, WEST, 400);

    newCar(430, WEST, 200);
    newCar(370, EAST, 200);
    newCar(390, WEST, 200);
    newCar(410, EAST, 200);
}


void test11() {
    // 同 test5, 先来了车4，对向来了3，同向来了2，对向来了5，最后车1才来，上路顺序是13254
    // Same as test5: Car 4 arrives first, then car 3 from the opposite direction, car 2 from the same direction, car 5 from the opposite direction, and finally car 1. The road entry order is 1-3-2-5-4.
    newCar(100, WEST, 100);
    newCar(30, WEST, 200);
    newCar(20, EAST, 200);
    newCar(5, WEST, 200);
    newCar(40, EAST, 200);
}


void test12() {
    // 测试驶出路口(第一个车位)的 side effect，顺序是 1-3-2-4
    // Test the side effect of leaving the first road position (intersection), the road entry order is 1-3-2-4.
    
    // 注意 1 驶出后还会卡一会，直到 3 来
    // Note that after car 1 exits, it will still block for a while until car 3 arrives.
    newCar(5, WEST, 500);
    newCar(0, WEST, 200);
    newCar(300, EAST, 200);
    newCar(10, EAST, 200);
}


void test13() {
    // 与 test10 相似的边际情况，参考 Piazza post 437
    // A similar edge case to test10, refer to Piazza post 437.
    
    // 1 进入, 对向来了 2, 同向来了 4, 1 驶出, 过了一会同向来了 3，顺序是 1-2-3-4
    // Car 1 enters, car 2 arrives from the opposite direction, car 4 from the same direction.
    // Car 1 exits, after a while, car 3 arrives from the same direction.
    // The road entry order is 1-2-3-4.
    newCar(0, WEST, 500);
    newCar(5, EAST, 200);
    newCar(300, EAST, 200);
    newCar(10, WEST, 200);
}


void test14() {
    // 车 1 上路堵着路口，对向来了车 4，同向来了车 2
    // Car 1 enters and blocks the intersection, car 4 arrives from the opposite direction, and car 2 arrives from the same direction.
    
    // 然后车 1 开出车位 1（即路口），注意，在 3 到来之前，2 和 4 都不可以上路，即使 1 开出去了
    // Then car 1 exits position 1 (the intersection).
    // Note: Before car 3 arrives, neither car 2 nor car 4 can enter the road, even though car 1 has exited.
    
    // 最终上路顺序是 1-3-2-4
    // The final road entry order is 1-3-2-4.
    newCar(0, WEST, 200);
    newCar(10, WEST, 200);
    newCar(300, EAST, 200);
    newCar(5, EAST, 200);
}


void test15() {
    // 参考 Piazza post 437
    // Refer to Piazza post 437.
    
    // 3 进入, 对向来了 1, 1 驶出, 过了一会对向来了 2，顺序是 1-2-3
    // Car 3 enters, car 1 arrives from the opposite direction, car 1 exits.
    // After a while, car 2 arrives from the opposite direction.
    // The road entry order is 1-2-3.
    newCar(5, WEST, 1000);
    newCar(100, WEST, 500);
    newCar(0, EAST, 500);
}


void test16() {
    // 参考 Piazza post 437
    // Refer to Piazza post 437.

    // 3 进入, 对向来了 1, 1 驶出, 过了一会同向来了 2，顺序是 1-2-3（注意 3 是紧贴着 2 上路的）
    // Car 3 enters, car 1 arrives from the opposite direction, car 1 exits.
    // After a while, car 2 arrives from the same direction.
    // The road entry order is 1-2-3 (note that car 3 follows closely behind car 2).
    newCar(5, WEST, 1000);
    newCar(100, EAST, 500);
    newCar(0, EAST, 500);
}


void Main(int argc, char const *argv[])
{
    initTest();
    test0();  // 不要修改这一行
    // Do not change this line
    endTest();
}

