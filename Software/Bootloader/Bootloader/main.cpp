#include "mbed.h"
#include "EUSBSerial.h"
#include "FlashIAPBlockDevice.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

#define DEBUG(...) {if (pc != nullptr) {pc->printf(__VA_ARGS__);}}

EUSBSerial* pc = nullptr;


// =============== THREADS ===============
    volatile bool led_running = true;
    void led_indicator() {
        PwmOut led(PB_9);

        bool dir = true;
        float val = 0.5;

        while (led_running) {
            led.write(val);
            if (dir) {
                val += 0.05;
                if (val >= 1.0)
                    dir = false;
            }
            else {
                val -= 0.05;
                if (val <= 0.2)
                    dir = true;
            }

            ThisThread::sleep_for(10ms);
        }

        led.write(0);
    }


    void _setup_watchdog() {
        Watchdog &watchdog = Watchdog::get_instance();
        watchdog.start(30000);
        watchdog.kick();
    }


    void _kick_watchdog() {
        Watchdog &watchdog = Watchdog::get_instance();
        
        if (!watchdog.is_running()){
            watchdog.start(30000);
        }


        while (true) {
            watchdog.kick();
            ThisThread::sleep_for(25s);
        }
    }
// =======================================


bool serial_boot();

int main() {
    // TODO read S1 / S2 first as this will be the flash entry point for radio and MCP
    int boot_status = 0;

    pc = new EUSBSerial(0x3232, 0x0000);

    ThisThread::sleep_for(500ms);
    pc->printf("ID\n");
    ThisThread::sleep_for(500ms);
            
    char buf[6];
    pc->readline(buf, 6);

    if (buf[0] == 'B' &&
        buf[1] == 'O' &&
        buf[2] == 'O' &&
        buf[3] == 'T')
    {
        boot_status = 1;
    }


    Thread t;
    Thread t2;
    if (boot_status != 0) {
        t.start(led_indicator);
        t2.start(_kick_watchdog);
    }

    switch (boot_status) {
        case 1:
            serial_boot();
            break;
        default:
            break;
    }

    if (boot_status != 0) {
        led_running = false;
        t.join();
    }

    if (pc != nullptr)
        delete pc;

    _setup_watchdog();
    mbed_start_application(POST_APPLICATION_ADDR);
}

size_t _serial_get_program_size() {
    pc->printf("SIZE\n");
    ThisThread::sleep_for(10ms);
    char buf[25] = {0};
    pc->readline(buf, 25);

    size_t program_size = 0;
    for (int i = 0; i<25; i++) {
        if (buf[i] < '0' || buf[i] > '9')
            break;
        program_size *= 10;
        program_size += buf[i] - '0';
    }

    return program_size;
}

bool _get_bytes(char* buf, size_t len) {
    pc->printf("DATA\n");
    for (int i = 0; i<len; i++) {
        bool res = pc->get_c(buf+i);
        if (!res)
            return false;
    }
    return true;
}


bool serial_boot() {    
    pc->printf("log: 0x%X | 0x%X\n", POST_APPLICATION_ADDR, POST_APPLICATION_SIZE);
    FlashIAPBlockDevice bd(POST_APPLICATION_ADDR, POST_APPLICATION_SIZE);
    bd.init();


    size_t program_size = _serial_get_program_size();

    pc->printf("log: 0x%X\n", program_size);

    if (program_size > POST_APPLICATION_SIZE) {
        pc->printf("log: ERROR | Application larger than ROM\n");
        return false;
    }

    uint32_t addr = 0;
    char buf[256] = {0};

    while (addr < program_size) {
        if (addr % bd.get_erase_size() == 0) {
            pc->printf("log: erasing block\n");
            bd.erase(addr, bd.get_erase_size());
        }

        memset(buf, 0, 256);

        size_t to_read = (program_size - addr < 256) ? program_size - addr : 256;
        //pc->printf("log:expecting %d bytes\n", to_read);
        if (!_get_bytes(buf, to_read)) {
            DigitalOut errorled(PB_10);
            errorled.write(1);
            pc->printf("log:failed to read ... continuing\n");
            ThisThread::sleep_for(100ms);
            continue;
        }
        //pc->printf("log:got all bytes ... writing\n");

        for (int i = 0; i<to_read; i++) {
            bd.program(&buf[i], addr, 1);
            addr += 1;
        }

        //pc->printf("log:wrote to address 0x%X\n", addr);

        //ThisThread::sleep_for(20ms);
        //pc->printf("log: wrote %d bytes\n", to_read);
    }

    bd.deinit();

    pc->printf("DONE\n");
    return true;
}
