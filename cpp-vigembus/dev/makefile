# Makefile

## Default Setting ===============================================

### .PHONY 타겟 선언 (실제 파일과 구분)
.PHONY: default all run clean

### 기본 타겟 설정
default: all

## Variable ======================================================

### Files
OUTPUT   := main.exe
SRCS     := $(wildcard src/*.cpp src/*/*.cpp main.cpp)
OBJS     := $(SRCS:%.cpp=build/%.o)

### g++

#### g++ > 컴파일러 선택
CXX      := g++

#### g++ > 컴파일러 옵션 플래그
# * -O2          | loop unrolling, function inlining, 메모리,속도희생 빼고 전부 최적화
# * -Wall        | 모든 모호한 코딩에 대해서 경고를 보내는 옵션
# * -g           | 디버깅 정보 포함
# * -static      | 정적,공유 라이브러리 중 정적을 우선. 속도는 빠르나 파일 사이즈가 커짐
# * -std=gnu++17 | C++17 Clang GNU 확장기능 유효
# * -pthread     | GCC의 스레드 지원 모드 활성화
CXXFLAGS := -O2 \
			-Wall \
			-g \
			-std=gnu++17 \
			-pthread

#### g++ > 전처리단계 헤더 경로 추가
# * -I"include"                                              | Local
# * -I"D:/Program Files/ViGEm/ViGEmClient-1.16.18.0/include" | ViGEm
CXXINCS  := -I"include" \
			-I"D:/Program Files/ViGEm/ViGEmClient-1.16.18.0/include"

#### g++ > 링킹단계 라이브러리 경로 추가
# * -L"lib"                                              | Local
CXXLIBS  := -L"lib"

#### g++ > 링킹단계 라이브러리 링크, -lX 은 libX.so 또는 libX.a 를 의미
# * -lm          | Math 라이브러리
# * -lsetupapi   | SetupAPI 라이브러리
CXXLINKS := -lm \
			-lsetupapi

## Target ========================================================

### 기본 타겟: 실행 파일 생성
all: $(OUTPUT)

### 링크단계 실행 파일 빌드
$(OUTPUT): $(OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CXXLIBS) $(CXXLINKS)

#### 전처리단계 오브젝트 파일 생성
build/%.o: %.cpp
	@IF NOT EXIST "$(subst /,\,$(dir $@))" MKDIR "$(subst /,\,$(dir $@))"
	$(CXX) $< -o $@ -c $(CXXFLAGS) $(CXXINCS)

run: all
	cmd /c chcp 65001>nul && $(OUTPUT)
# * "@./$(OUTPUT)" 으로 즉시 구동해도 되나, 유니코드 문제 해결을 위해 우회함

clean:
	@IF EXIST $(OUTPUT) DEL /Q $(OUTPUT)
	@IF EXIST "build" RMDIR /S /Q "build"