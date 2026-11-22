
#include <iostream>
#include <cstring>
#include <systemd/sd-bus.h>

constexpr const char* SERVICE =
    "org.example.Lab2Notification";

constexpr const char* PATH =
    "/org/example/Lab2Notification";

// 서버가 제공하는 인터페이스 정의
static const sd_bus_vtable notification_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_SIGNAL("Notify", "s", 0),
    SD_BUS_VTABLE_END
};

int main() {
    sd_bus* bus = nullptr;
    sd_bus_slot* slot = nullptr;
    int ret = 0;

    // TODO 1: Session Bus 연결
    ret = sd_bus_default_user(&bus);

    if (ret < 0) {
        std::cerr << "버스 연결 실패: "
                  << std::strerror(-ret) << "\n";
        goto cleanup;
    }

    // TODO 2: 서비스 이름 등록
    ret = sd_bus_request_name(bus, SERVICE, 0);

    if (ret < 0) {
        std::cerr << "서비스 등록 실패: "
                  << std::strerror(-ret) << "\n";
        goto cleanup;
    }

    // TODO 3: 객체 및 인터페이스 등록
    ret = sd_bus_add_object_vtable(
        bus,
        &slot,
        PATH,
        SERVICE,
        notification_vtable,
        nullptr
    );

    if (ret < 0) {
        std::cerr << "인터페이스 등록 실패: "
                  << std::strerror(-ret) << "\n";
        goto cleanup;
    }

    std::cout << "D-Bus 서버 시작\n";

    for (int i = 1; i <= 10; i++) {

        // 대기 중인 D-Bus 요청 처리
        do {
            ret = sd_bus_process(bus, nullptr);
        } while (ret > 0);

        if (ret < 0) {
            std::cerr << "버스 처리 오류: "
                      << std::strerror(-ret) << "\n";
            break;
        }

        // 최대 2초 동안 버스 이벤트 대기
        ret = sd_bus_wait(bus, 2000000ULL);

        if (ret < 0) {
            std::cerr << "버스 대기 오류: "
                      << std::strerror(-ret) << "\n";
            break;
        }

        // TODO 4: Signal 발행
        ret = sd_bus_emit_signal(
            bus,
            PATH,
            SERVICE,
            "Notify",
            "s",
            "Hello from D-Bus server!"
        );

        // TODO 5: 전송 결과 확인
        if (ret < 0) {
            std::cerr << "Signal 전송 실패: "
                      << std::strerror(-ret) << "\n";
            break;
        }

        std::cout << "Notify Signal 발행: "
                  << i << "\n";
    }

    // 남아 있는 출력 메시지 전송
    if (bus && ret >= 0) {
        int flush_ret = sd_bus_flush(bus);
        if (flush_ret < 0) {
            ret = flush_ret;
            std::cerr << "버스 flush 실패: "
                      << std::strerror(-ret) << "\n";
        }
    }

cleanup:
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);

    return ret < 0 ? 1 : 0;
}
