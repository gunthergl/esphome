#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#ifdef USE_ESP32
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <esp_err.h>
#endif

namespace esphome {
namespace csi_sensor {

class CSISensor : public sensor::Sensor, public PollingComponent {
 public:
  // Config
  void set_publish_interval(uint32_t ms) { publish_interval_ms_ = ms; }

  // Optional sensors
  void set_packet_count_sensor(sensor::Sensor *s) { packet_count_sensor_ = s; }
  void set_last_rssi_sensor(sensor::Sensor *s) { last_rssi_sensor_ = s; }
  void set_csi_len_sensor(sensor::Sensor *s) { csi_len_sensor_ = s; }
  void set_channel_sensor(sensor::Sensor *s) { channel_sensor_ = s; }
  void set_rate_sensor(sensor::Sensor *s) { rate_sensor_ = s; }
  void set_noise_floor_sensor(sensor::Sensor *s) { noise_floor_sensor_ = s; }

  // Lifecycle
  void setup() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void dump_config() override;

 protected:
#ifdef USE_ESP32
  static void csi_rx_callback(void *ctx, wifi_csi_info_t *info);
#endif
  void publish_cached_();

  // State updated from callback (cache only; published in loop)
  volatile uint32_t packet_count_{0};
  volatile int last_rssi_{0};
  volatile int last_csi_len_{0};
  volatile int last_channel_{0};
  volatile int last_rate_{0};
  volatile int last_noise_floor_{0};
  volatile bool have_new_frame_{false};

  // Config
  uint32_t publish_interval_ms_{1000};
  uint32_t last_publish_ms_{0};
  bool csi_enabled_{false};

  // Sensors
  sensor::Sensor *packet_count_sensor_{nullptr};
  sensor::Sensor *last_rssi_sensor_{nullptr};
  sensor::Sensor *csi_len_sensor_{nullptr};
  sensor::Sensor *channel_sensor_{nullptr};
  sensor::Sensor *rate_sensor_{nullptr};
  sensor::Sensor *noise_floor_sensor_{nullptr};
};

}  // namespace csi_sensor
}  // namespace esphome
