
#include <iostream>
#include <cstring>
#include <systemd/sd-bus.h>

// Signal을 받았을 때 실행되는 콜백
static int on_notify(
    sd_bus_message* m,
    void* userdata,
    sd_bus_error* error
) {
    (void)userdata;
    (void)error;

    const char* message = nullptr;

    // Signal에 담긴 문자열 꺼내기
    int ret = sd_bus_message_read(m, "s", &message);

    if (ret < 0) {
        std::cerr << "메시지 읽기 실패: "
                  << std::strerror(-ret) << "\n";
        return ret;
    }

    std::cout << "Notify 수신: " << message << "\n";

    return 1;
}

int main() {
    sd_bus* bus = nullptr;
    sd_bus_slot* slot = nullptr;
    int ret = 0;

    // Session Bus 연결
    ret = sd_bus_default_user(&bus);

    // 연결 실패 확인
    if (ret < 0) {
        std::cerr << "버스 연결 실패: "
                  << std::strerror(-ret) << "\n";
        goto cleanup;
    }

    // Notify Signal 구독
    ret = sd_bus_add_match(
        bus,
        &slot,
        "type='signal',"
        "interface='org.example.Lab2Notification',"
        "member='Notify',"
        "path='/org/example/Lab2Notification'",
        on_notify,
        nullptr
    );

    //  구독 실패 확인
    if (ret < 0) {
        std::cerr << "Signal 구독 실패: "
                  << std::strerror(-ret) << "\n";
        goto cleanup;
    }

    std::cout << "알림 수신 대기 중...\n";

    while (true) {

        //  들어온 메시지 처리
        ret = sd_bus_process(bus, nullptr);

        // 오류 처리
        if (ret < 0) {
            std::cerr << "메시지 처리 실패: "
                      << std::strerror(-ret) << "\n";
            break;
        }

        // 처리한 작업이 있으면 반복
        if (ret > 0) {
            continue;
        }

        //  새로운 이벤트 대기
        ret = sd_bus_wait(bus, UINT64_MAX);

        if (ret < 0) {
            std::cerr << "버스 대기 실패: "
                      << std::strerror(-ret) << "\n";
            break;
        }
    }
 
cleanup:
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return ret < 0 ? 1 : 0;
}
