#include <bits/stdc++.h>
#include <windows.h>
#include <ViGEmClient.h>

using namespace std;

// * 레지스트리에서 ViGEmBus 드라이버 설치 여부 확인
bool IsViGEmBusInstalled() {
    HKEY hKey;
    LONG result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Services\\ViGEmBus",
        0,
        KEY_READ,
        &hKey
    );

    if (result == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    } else {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    cout << "=== ViGEm 예제 프로그램 ===" << endl;

    // * 드라이버 설치 여부 확인
    if (!IsViGEmBusInstalled()) {
        cout << "[경고] ViGEmBus 드라이버가 설치되어 있지 않습니다!" << endl;
        // if (!InstallViGEmBusDriver()) {
        //     std::cerr << "프로그램을 종료합니다." << std::endl;
        //     return -1;
        // } else {
        //     std::cout << "설치가 완료된 후 프로그램을 다시 실행해주세요." << std::endl;
        //     return 0;
        // }
    }
    else {
        cout << "[알림] ViGEmBus 드라이버가 설치되어 있습니다." << endl;
    }
    
    return 0;
}