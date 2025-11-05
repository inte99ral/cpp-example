#include <bits/stdc++.h>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "testTimer/testTimer.hpp"
#include "testScene/testScene.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    TestSceneObject *sceneObj = new TestSceneObject();
    delete sceneObj;

    TestTimerObject *timerObj = new TestTimerObject();
    timerObj->Timer5Sec();
    delete timerObj;

    return 0;
}