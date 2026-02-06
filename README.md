# SSU_OS_24-2

> 숭실대학교 컴퓨터학부\
> 2024학년도 2학기 운영체제 설계 (xv6 기반)

xv6를 기반으로 시스템 콜, 스케줄러, 메모리 관리, MMU를 확장·구현한
프로젝트입니다.

------------------------------------------------------------------------

## 구성

  과제    내용
  ------- ----------------------------------------------
  HW1     `lseek` 시스템 콜 구현  
  HW2     SSU Scheduler (MLFQ  + Aging)  
  HW3-1   지연 할당 / 지연 해제 (`ssusbrk`, `memstat`)  
  HW3-2   Simple Software MMU 시뮬레이터  

------------------------------------------------------------------------

## 디렉토리 구조

    SSU_OS_24-2/
    ├── hw1/        # lseek 시스템 콜
    ├── hw2/        # SSU Scheduler (MLFQ)
    └── hw3/
        ├── 3-1/    # ssusbrk / memstat
        └── 3-2/    # Software MMU

------------------------------------------------------------------------

# HW1 --- lseek 시스템 콜

### 목표

xv6에 파일 오프셋을 이동할 수 있는 `lseek()` 시스템 콜 추가.

### 인터페이스

``` c
int lseek(int fd, int offset, int whence);
```

-   `SEEK_SET`
-   `SEEK_CUR`
-   `SEEK_END`

### 구현 내용

-   `struct file`의 `off` 값 갱신
-   오프셋 범위 검증
-   시스템 콜 등록 파일 수정
    -   `syscall.h`
    -   `syscall.c`
    -   `sysproc.c`
    -   `user.h`
    -   `usys.S`

### 실행 예시

``` bash
$ lseektest hello.txt 6 World
Before : Hello SSU
After  : Hello World
```

------------------------------------------------------------------------

# HW2 --- SSU Scheduler (MLFQ)

## 설계 개요

-   4단계 다단계 피드백 큐(MLFQ)
-   동적 우선순위 조정
-   Aging 기법 (250 tick 이상 대기 시 승격)
-   단일 CPU 환경
-   idle, init, 쉘 프로세스는 항상 레벨 3에만 존재

## 타임 슬라이스

| 레벨 | Time Quantum |
|-----|-----|
| 0 | 10 tick  |
| 1 | 20 tick |
| 2 | 40 tick |
| 3 | 80 tick |

## 동작 방식

-   매 tick마다 필드 증가 규칙
    -   running → cpu_burst++
    -   runnable → cpu_wait++
    -   sleeping → io_wait_time++
-   높은 우선순위 큐부터 검사
-   타임 슬라이스 초과 시 하위 큐로 이동
-   250 tick 이상 RUNNABLE 상태 대기 시 상위 큐로 승격
-   동일 큐 내 우선순위:
    -   `io_wait_time` 값이 큰 프로세스 우선
    -   동일 시 나중에 들어온 프로세스 우선

## 추가된 프로세스 필드

``` c
int q_level;
int cpu_burst;
int cpu_wait;
int io_wait_time;
int end_time;
```

------------------------------------------------------------------------

# HW3-1 --- SSU Memory Allocation

## ssusbrk()

``` c
int ssusbrk(int page_size, int tick);
```

### 1) 지연 할당 (page_size \> 0)

-   가상 메모리만 증가
-   실제 접근 시 page fault 발생 → 물리 페이지 할당

### 2) 지연 해제 (page_size \< 0)

-   지정한 tick 이후 메모리 해제
-   해제 요청 누적 가능
-   해제 시점에 `memstat()` 호출

------------------------------------------------------------------------

## memstat()

출력 내용:

-   가상 페이지 개수
-   물리 페이지 개수
-   현재 매핑된 PDE / PTE 정보

예시:

    vp: 5, pp: 4
    PDE - 0x...
    PTE - 0x... - 0x...

------------------------------------------------------------------------

# HW3-2 --- Simple Software MMU

독립 실행형 MMU 시뮬레이터.

## 빌드

``` bash
gcc -o ssu_mmu ssu_mmu.c -lm
```

## 실행

``` bash
./ssu_mmu 32 4096
```

## PTE 구조 (32bit)

  | 비트 | 의미 |
|-----|-----|
| 12--31 | PFN  |
| 1 | 접근 비트 |
| 0 | 유효 비트 |

## 구현 함수

``` c
void alloc_page_table(int address_space_bits, int page_bytes);

int mmu_address_translation(unsigned int virtual_address,
                            unsigned int *physical_address);
```

반환 값:

-   `SUCCESS`
-   `NOT_VALID`
-   `NOT_ACCESSIBLE`

------------------------------------------------------------------------

# 🐳 Docker 환경

Docker 이미지:

    kangminhee/xv6-docker

Docker Hub:
https://hub.docker.com/repository/docker/kangminhee/xv6-docker/general

## 실행 방법

``` bash
docker pull kangminhee/xv6-docker
docker run -it kangminhee/xv6-docker
```

------------------------------------------------------------------------

## 실행 환경

-   기반: MIT xv6-public
-   Linux + QEMU 환경
-   단일 CPU 설정
