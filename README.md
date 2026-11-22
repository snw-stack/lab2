# lab2

리눅스 시스템 프로그래밍을 연습하면서 만든 예제들이다. 파일과 디렉터리 읽기부터 시작해서 프로세스, 시그널, 파이프, 메시지 큐, `mmap`, D-Bus까지 실습별로 정리했다. `rust-matrix`에는 Rust로 만든 행렬 덧셈 예제를 따로 넣었다.

## 실습 목록

| 폴더 | 내용 |
| --- | --- |
| [03-file-directory](03-file-directory/) | `test.txt`에 내용을 추가한 뒤 다시 읽고, 디렉터리 항목의 이름과 inode를 출력한다. |
| [04-recursive-ls](04-recursive-ls/) | 숨김 항목을 건너뛰고, 이름순으로 정렬한 목록을 하위 디렉터리까지 출력한다. |
| [05-typing-practice](05-typing-practice/) | `sentences.txt`의 문장을 따라 치고 타수(CPM)와 오타를 확인한다. `typing_upgrade.cpp`는 한글도 UTF-8 문자 단위로 세고, 편집 거리로 오타를 계산한다. |
| [06-process-signal](06-process-signal/) | `fork`, `execl`, `waitpid`로 자식 프로세스를 실행하고, 부모와 자식 사이에 `SIGUSR1`과 `SIGUSR2`를 주고받는다. |
| [07-ipc](07-ipc/) | `flock`의 공유 잠금과 배타 잠금, 파이프의 논블로킹 읽기, `select`로 여러 파이프를 감시하는 방법을 다룬다. |
| [08-pipe-chat](08-pipe-chat/) | 한 터미널에서 부모와 자식 프로세스가 파이프 두 개로 번갈아 채팅한다. `exit`를 입력하면 끝난다. |
| [09-message-queue](09-message-queue/) | POSIX 메시지 큐로 문자열을 주고받는다. `mq_chat.cpp`는 터미널 두 개에서 실행하는 채팅 예제다. |
| [10-sm-mmap](10-sm-mmap/) | `mmap`으로 `input.txt`의 앞 32바이트까지 읽거나 파일 전체를 `output.txt`로 복사한다. |
| [11-D-Bus](11-D-Bus/) | 서버가 세션 버스에 `Notify` 시그널을 보내고 클라이언트가 받아서 출력한다. |
| [rust-matrix](rust-matrix/) | 입력받은 행과 열 크기로 행렬 두 개를 만든다. 각 칸을 1~9 사이의 난수로 채운 뒤, 같은 위치의 값을 더해 출력한다. |

## 실행 방법

C++ 예제는 Linux나 WSL에서 실행한다. `g++`로 소스 파일을 하나씩 컴파일하면 된다. `sentences.txt`나 `input.txt`를 읽는 예제는 해당 실습 폴더에서 실행한다.

저장소 루트에서 재귀 디렉터리 목록 예제를 실행하려면 다음과 같이 하면 된다.

```bash
cd 04-recursive-ls
g++ -std=c++17 recursivels.cpp -o recursivels
./recursivels sample
```

### 메시지 큐 채팅

```bash
cd 09-message-queue
g++ -std=c++17 mq_chat.cpp -o mq_chat -lrt
```

각 터미널에서 `09-message-queue` 폴더로 이동한 뒤 각각 `./mq_chat A`, `./mq_chat B`를 실행한다. A부터 메시지를 입력하고, 채팅을 끝낼 때는 `exit`를 입력한다.

### D-Bus 알림

Ubuntu에서는 `libsystemd-dev`, `pkg-config`, `dbus` 패키지가 필요하다. 아래처럼 실행하면 서버와 클라이언트가 같은 세션 버스를 쓴다. 서버는 시그널을 10번 보내고 종료한다. 수신 확인이 끝나면 `Ctrl+C`로 클라이언트를 종료한다.

```bash
cd 11-D-Bus
g++ -std=c++17 bus.cpp -o bus $(pkg-config --cflags --libs libsystemd)
g++ -std=c++17 client.cpp -o client $(pkg-config --cflags --libs libsystemd)
dbus-run-session -- bash -c './bus & exec ./client'
```

### Rust 행렬 덧셈

```bash
cd rust-matrix
cargo run
```

행과 열을 순서대로 입력한다. 예를 들어 `2`와 `3`을 입력하면 2행 3열 행렬 두 개와 덧셈 결과가 출력된다.
