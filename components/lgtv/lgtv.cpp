#include "lgtv.h"

void LGTVComponent::setup() {}

void LGTVComponent::loop() {
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

void LGTVComponent::send_command(const char *command) {
  write_str(command);
  write_byte(0x0D);
  delay(100);
  this->read_response();
}

void LGTVComponent::query_power_status() {
  send_command("ka 01 FF");
}

void LGTVComponent::query_input_status() {
  send_command("xb 01 FF");
}

void LGTVComponent::query_volume_status() {
  send_command("kf 01 FF");
}

void LGTVComponent::power_on() {
  send_command("ka 01 01");
}

void LGTVComponent::power_off() {
  send_command("ka 01 00");
}

void LGTVComponent::volume_mute_on() {
  send_command("ke 01 00");
}

void LGTVComponent::volume_mute_off() {
  send_command("ke 01 01");
}

void LGTVComponent::set_volume(int volume) {
  char command[10];
  snprintf(command, sizeof(command), "kf 01 %02X", volume);
  send_command(command);
}

void LGTVComponent::set_input(int input) {
  char command[10];
  snprintf(command, sizeof(command), "xb 01 %02X", input);
  send_command(command);
}

void LGTVComponent::read_response() {
  if (available()) {
    String response = readStringUntil(0x0D);
  
    if (response.startsWith("a ")) {
      // Power status response
      if (response[6] == '0') {
        power_status_sensor->publish_state(0); // Off
      } else {
        power_status_sensor->publish_state(1); // On
      }
    } else if (response.startsWith("b ")) {
      // Input status response
      int input = strtol(response.substring(6).c_str(), nullptr, 16);
      input_status_sensor->publish_state(input);
    } else if (response.startsWith("f ")) {
      // Volume status response
      int volume = strtol(response.substring(6).c_str(), nullptr, 16);
      volume_status_sensor->publish_state(volume);
    }
  }
}