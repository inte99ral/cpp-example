#include <bits/stdc++.h>
#include <windows.h>
#include <ViGEmClient.h>

using namespace std;

bool isViGEmBusResponsive() {
    // * 클라이언트 객체 생성
    PVIGEM_CLIENT client = vigem_alloc();
    if (client == nullptr) return false;

    // * ViGEmBus 드라이버에 연결 시도
    const auto ret = vigem_connect(client);

    // * 클라이언트 객체 해제
    vigem_disconnect(client);
    vigem_free(client);

    // * VIGEM_ERROR_NONE == ret 인지 확인
    return VIGEM_SUCCESS(ret);
}

int main(int argc, char* argv[]) {
    // * 드라이버 설치 여부 확인
    if (!isViGEmBusResponsive()) cout << "[경고] ViGEmBus 드라이버가 설치되어 있지 않습니다!" << endl;
    else cout << "[알림] ViGEmBus 드라이버가 설치되어 있습니다." << endl;

    return 0;
}