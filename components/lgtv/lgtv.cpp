#include "lgtv.h"

namespace esphome {
namespace lgtv {

  void LGTV::setup() {}

  void LGTV::loop() {
      delay(500);
      this->query_power_status();
      this->query_input_status();
      this->query_volume_status();
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

  void LGTV::power(bool on) {
    if (on) {
      send_command("ka 01 01");
    } else {
      send_command("ka 01 00");
    }
  }

  void LGTV::volume_mute(bool mute) {
    if (mute) {
      send_command("ke 01 00");
    } else {
      send_command("ke 01 01");
    }
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