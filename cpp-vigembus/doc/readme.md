# cpp-vigembus：가상 게임패드 컨트롤러 프로젝트

## 개요

&nbsp; `ViGEm`(Virtual Gamepad Emulation) 은 가상 게임패드를 에뮬레이션하기 위한 프레임워크입니다. 가상 Xbox/DS4 컨트롤러를 생성하고 조작하기 위해 사용됩니다.

&nbsp; ViGEm은 크게 ViGEmBus(이용자용 드라이버) + ViGEmClient(개발자용 클라이언트 라이브러리) 두 부분으로 구성됩니다.

-   **ViGEmBus(드라이버)**는 Windows 커널 모드 드라이버로, 다른 프로그램(유저모드)에서 생성한 입력을 운영체제와 게임이 표준 컨트롤러처럼 받게 해주는 역할입니다.

-   **ViGEmClient(클라이언트 라이브러리)**는 개발자용 유저모드 라이브러리로, 어플리케이션이 ViGEmBus에 명령을 보낼 수 있게 합니다.

## 설치

### ViGEmBus 드라이버 설치

-   [공식 GitHub 링크](https://github.com/ViGEm/ViGEmBus/releases)

최신 버전의 `ViGEmBus_Setup_x64.msi` 설치하면 쉽게 설치할 수 있습니다.

### ViGEmClient SDK 설치

-   [공식 GitHub 링크](https://github.com/ViGEm/ViGEmClient/releases)

&nbsp; 설치 후, 통상적으로는 `C:\Program Files\ViGEm\Client\` 경로에 다음의 두 파일이 있습니다.

-   `include/ViGEm/Client.h`
-   `lib/ViGEmClient.lib`

&nbsp; 예를들어 지금 조건 그대로 g++ 로 빌드한다면 `g++ main.cpp -I"C:\Program Files\ViGEm\Client\include" -o test.exe` 과 같은 명령어를 쉘에 입력하면 됩니다. src 에서 라이브러리가 cpp 코드로 구현되어 있기 때문에 링크 옵션은 필요없습니다.

## 사용 예시

> [!WARN]
>
> &nbsp; ViGEmClient 는 `<windows.h>` 에서 typedef 타입정의된 데이터타입을 사용합니다.
> &nbsp; `#include <windows.h>` 를 반드시 먼저 선언해주어야 합니다.

&nbsp;

## 배포 시 주의점

&nbsp; 사용자 모드(.exe)는 커널에 새 가상 HID 장치를 직접 등록할 권한이 없습니다. 사용자는 별도로 ViGEmBus 드라이버를 설치해야 합니다.

&nbsp; `ViGEmBus 드라이버 설치 확인 + 자동 설치 유도 코드 (C++)` 코드를 추가해주면 사용자 편의성을 더할 수 있습니다.

-   당연히 프로젝트에 ViGEmClient SDK (ViGEmClient.lib, ViGEmClient.dll) 가 포함되어 있어야 합니다.
-   `ShellExecute` 를 쓰므로 관리자 권한이 필요합니다.
-   `ViGEmBusSetup_x64.msi` (또는 x86) 설치 파일이 프로그램 실행 파일과 같은 폴더에 있어야 합니다.

```cpp
#include <windows.h>
#include <ViGEm/Client.h>
#include <iostream>
#include <filesystem>

bool IsViGEmBusInstalled() {
    // 레지스트리에서 ViGEmBus 드라이버 설치 여부 확인
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
}

bool InstallViGEmBusDriver() {
    std::string installerPath = "ViGEmBusSetup_x64.msi";
    if (!std::filesystem::exists(installerPath)) {
        std::cerr << "[오류] 설치 파일이 없습니다: " << installerPath << std::endl;
        return false;
    }

    std::cout << "[안내] ViGEmBus 드라이버를 설치합니다..." << std::endl;

    // 관리자 권한으로 MSI 실행
    HINSTANCE res = ShellExecuteA(
        NULL,
        "runas",              // 관리자 권한 요청
        "msiexec.exe",
        ("/i " + installerPath + " /qn").c_str(), // /qn: 조용히 설치
        NULL,
        SW_SHOW
    );

    if ((INT_PTR)res <= 32) {
        std::cerr << "[오류] 설치 실행에 실패했습니다." << std::endl;
        return false;
    }

    std::cout << "[안내] 설치 진행 중... 잠시 후 다시 시도하세요." << std::endl;
    return true;
}

int main() {
    std::cout << "=== ViGEm 예제 프로그램 ===" << std::endl;

    // 1️⃣ 드라이버 설치 여부 확인
    if (!IsViGEmBusInstalled()) {
        std::cout << "[경고] ViGEmBus 드라이버가 설치되어 있지 않습니다." << std::endl;
        if (!InstallViGEmBusDriver()) {
            std::cerr << "프로그램을 종료합니다." << std::endl;
            return -1;
        } else {
            std::cout << "설치가 완료된 후 프로그램을 다시 실행해주세요." << std::endl;
            return 0;
        }
    }

    // 2️⃣ ViGEm 클라이언트 초기화
    PVIGEM_CLIENT client = vigem_alloc();
    if (client == nullptr) {
        std::cerr << "[오류] ViGEm 클라이언트 할당 실패." << std::endl;
        return -1;
    }

    const auto ret = vigem_connect(client);
    if (!VIGEM_SUCCESS(ret)) {
        std::cerr << "[오류] ViGEmBus에 연결 실패. (코드 " << ret << ")" << std::endl;
        vigem_free(client);
        return -1;
    }

    std::cout << "[성공] ViGEmBus 연결 완료!" << std::endl;

    // 3️⃣ Xbox 360 컨트롤러 생성
    auto pad = vigem_target_x360_alloc();
    const auto pir = vigem_target_add(client, pad);
    if (!VIGEM_SUCCESS(pir)) {
        std::cerr << "[오류] 가상 컨트롤러 추가 실패!" << std::endl;
        vigem_target_free(pad);
        vigem_disconnect(client);
        vigem_free(client);
        return -1;
    }

    std::cout << "[성공] 가상 Xbox 360 컨트롤러 생성됨." << std::endl;

    // 4️⃣ 테스트 입력
    XUSB_REPORT report;
    ZeroMemory(&report, sizeof(report));
    report.wButtons = XUSB_GAMEPAD_A; // A버튼 누름 상태로 설정
    vigem_target_x360_update(client, pad, report);
    std::cout << "[테스트] A 버튼 입력 전송 완료!" << std::endl;

    Sleep(1000); // 1초 대기
    ZeroMemory(&report, sizeof(report));
    vigem_target_x360_update(client, pad, report);
    std::cout << "[테스트] A 버튼 해제." << std::endl;

    // 종료
    vigem_target_remove(client, pad);
    vigem_target_free(pad);
    vigem_disconnect(client);
    vigem_free(client);

    std::cout << "[완료] 프로그램 종료." << std::endl;
    return 0;
}
```

&nbsp; 동작은 다음과 같습니다.

| 단계 | 설명                                                               |
| ---- | ------------------------------------------------------------------ |
| ①    | `RegOpenKeyExA()` 로 ViGEmBus 설치 여부를 확인                     |
| ②    | 없으면 `msiexec.exe /i ViGEmBusSetup_x64.msi /qn` 으로 조용히 설치 |
| ③    | 설치가 완료되면 사용자에게 재실행 안내                             |
| ④    | 이후 정상적으로 ViGEmBus 연결 및 가상 Xbox 컨트롤러 생성           |
| ⑤    | `A 버튼 입력 → 해제` 테스트 후 종료                                |

&nbsp; 다음과 같은 주의사항이 있습니다.

-   `/qn` 옵션을 제거하면 설치 UI가 표시됩니다.
-   자동 설치 후 재부팅이 필요할 수도 있습니다 (드라이버 설치 특성상).
-   vigem_connect() 가 실패하면 반드시 관리자 권한으로 실행해보세요.
