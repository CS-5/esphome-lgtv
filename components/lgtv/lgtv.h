#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace lgtv {

  class LGTV : public uart::UARTDevice, public Component {
  public:
    LGTV() : UARTDevice(nullptr) {}
    LGTV(uart::UARTComponent *parent) : UARTDevice(parent) {}

    void set_uart_parent(uart::UARTComponent *parent) { this->parent_ = parent; }
    void set_poll_interval(uint32_t interval) { this->poll_interval = interval; }

    sensor::Sensor *power_status_sensor = new sensor::Sensor();
    sensor::Sensor *input_status_sensor = new sensor::Sensor();
    sensor::Sensor *volume_status_sensor = new sensor::Sensor();
    int poll_interval;

    void setup() override;
    void loop() override;

    void send_command(const char *command);
    void query_power_status();
    void query_input_status();
    void query_volume_status();
    void power_on();
    void power_off();
    void volume_mute_on();
    void volume_mute_off();
    void set_volume(int volume);
    void set_input(int input);
    void read_response();
  };
  
}
}

