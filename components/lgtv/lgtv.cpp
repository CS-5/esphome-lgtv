#include "lgtv.h"

namespace esphome {
namespace lgtv {

  void LGTV::setup() {}

  void LGTV::loop() {
    static unsigned long last_query_time = 0;
    unsigned long current_time = millis();

    if (current_time - last_query_time > poll_interval) {
      // Regularly query the TV for its state
      this->query_power_status();
      delay(500);
      this->query_input_status();
      delay(500);
      this->query_volume_status();
      last_query_time = current_time;
    }
  }

  void LGTV::send_command(const char *command) {
    this->parent_->write_str(command);
    this->parent_->write_byte(0x0D);
    delay(100);
    this->read_response();
  }

  void LGTV::query_power_status() {
    send_command("ka 01 FF");
  }

  void LGTV::query_input_status() {
    send_command("xb 01 FF");
  }

  void LGTV::query_volume_status() {
    send_command("kf 01 FF");
  }

  void LGTV::power_on() {
    send_command("ka 01 01");
  }

  void LGTV::power_off() {
    send_command("ka 01 00");
  }

  void LGTV::volume_mute_on() {
    send_command("ke 01 00");
  }

  void LGTV::volume_mute_off() {
    send_command("ke 01 01");
  }

  void LGTV::set_volume(int volume) {
    char command[10];
    snprintf(command, sizeof(command), "kf 01 %02X", volume);
    send_command(command);
  }

  void LGTV::set_input(int input) {
    char command[10];
    snprintf(command, sizeof(command), "xb 01 %02X", input);
    send_command(command);
  }

  void LGTV::read_response() {
    if (this->available()) {
      uint8_t response[100];
      size_t len = this->read_array(response, sizeof(response) - 1);
      response[len] = '\0';  // Null-terminate the response string
      
      if (memcmp(response, "a ", 2) == 0) {
        // Power status response
        if (response[6] == '0') {
          power_status_sensor->publish_state(0); // Off
        } else {
          power_status_sensor->publish_state(1); // On
        }
      } else if (memcmp(response, "b ", 2) == 0) {
        // Input status response
        int input = strtol((char*)&response[6], nullptr, 16);
        input_status_sensor->publish_state(input);
      } else if (memcmp(response, "f ", 2) == 0) {
        // Volume status response
        int volume = strtol((char*)&response[6], nullptr, 16);
        volume_status_sensor->publish_state(volume);
      }
    }
  }

}
}