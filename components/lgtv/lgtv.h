#pragma once

#include "esphome.h"

class LGTVComponent : public Component, public UARTDevice {
 public:
  LGTVComponent(UARTComponent *parent) : UARTDevice(parent) {}

  Sensor *power_status_sensor = new Sensor();
  Sensor *input_status_sensor = new Sensor();
  Sensor *volume_status_sensor = new Sensor();
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