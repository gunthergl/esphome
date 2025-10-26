#include "csi_sensor.h"
#include "esphome/core/log.h"

#ifdef USE_WIFI
#include "esphome/components/wifi/wifi_component.h"
#endif

#ifdef USE_ESP32
#include <esp_wifi.h>
#endif

namespace esphome {
namespace csi_sensor {

static const char *const TAG = "csi_sensor";

void CSISensor::setup() {
#ifndef USE_ESP32
  ESP_LOGE(TAG, "CSI requires ESP32/ESP-IDF.");
  return;
#else
  ESP_LOGI(TAG, "Setting up WiFi CSI...");
  // Ensure WiFi exists
#ifdef USE_WIFI
  if (!wifi::global_wifi_component) {
    ESP_LOGE(TAG, "WiFi component not available; CSI requires WiFi.");
    return;
  }
#endif

  wifi_csi_config_t csi_config = {
      .lltf_en = true,
      .htltf_en = true,
      .stbc_htltf2_en = true,
      .ltf_merge_en = true,
      .channel_filter_en = false,
      .manu_scale = false,
      .shift = 0,
  };

  esp_err_t ret = esp_wifi_set_csi_config(&csi_config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "esp_wifi_set_csi_config failed: %d", (int) ret);
    return;
  }

  ret = esp_wifi_set_csi_rx_cb(&CSISensor::csi_rx_callback, this);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "esp_wifi_set_csi_rx_cb failed: %d", (int) ret);
    return;
  }

  ret = esp_wifi_set_csi(true);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "esp_wifi_set_csi(true) failed: %d", (int) ret);
    return;
  }

  this->csi_enabled_ = true;
  ESP_LOGI(TAG, "CSI enabled.");
#endif
}

void CSISensor::loop() {
  const uint32_t now = millis();
  if (now - last_publish_ms_ >= publish_interval_ms_) {
    publish_cached_();
    last_publish_ms_ = now;
  }
}

#ifdef USE_ESP32
void CSISensor::csi_rx_callback(void *ctx, wifi_csi_info_t *info) {
  auto *self = static_cast<CSISensor *>(ctx);
  if (self == nullptr || !self->csi_enabled_)
    return;

  // cache latest values (do not call publish_state here)
  self->packet_count_++;
  self->last_rssi_ = info->rx_ctrl.rssi;
  self->last_csi_len_ = (int) info->len;
  self->last_channel_ = (int) info->rx_ctrl.channel;
  self->last_rate_ = (int) info->rx_ctrl.rate;
  self->last_noise_floor_ = (int) info->rx_ctrl.noise_floor;
  self->have_new_frame_ = true;
}
#endif

void CSISensor::publish_cached_() {
  // Always publish packet counter so you can see liveness
  if (packet_count_sensor_ != nullptr)
    packet_count_sensor_->publish_state((float) packet_count_);

  if (have_new_frame_) {
    if (last_rssi_sensor_ != nullptr)
      last_rssi_sensor_->publish_state((float) last_rssi_);
    if (csi_len_sensor_ != nullptr)
      csi_len_sensor_->publish_state((float) last_csi_len_);
    if (channel_sensor_ != nullptr)
      channel_sensor_->publish_state((float) last_channel_);
    if (rate_sensor_ != nullptr)
      rate_sensor_->publish_state((float) last_rate_);
    if (noise_floor_sensor_ != nullptr)
      noise_floor_sensor_->publish_state((float) last_noise_floor_);
    have_new_frame_ = false;
  }
}

void CSISensor::dump_config() {
  ESP_LOGCONFIG(TAG, "CSI Sensor:");
  ESP_LOGCONFIG(TAG, "  CSI enabled: %s", YESNO(csi_enabled_));
  ESP_LOGCONFIG(TAG, "  Publish interval: %u ms", (unsigned) publish_interval_ms_);
  LOG_SENSOR("  ", "Packet Count", packet_count_sensor_);
  LOG_SENSOR("  ", "Last RSSI", last_rssi_sensor_);
  LOG_SENSOR("  ", "CSI Length", csi_len_sensor_);
  LOG_SENSOR("  ", "Channel", channel_sensor_);
  LOG_SENSOR("  ", "Rate", rate_sensor_);
  LOG_SENSOR("  ", "Noise Floor", noise_floor_sensor_);
}

}  // namespace csi_sensor
}  // namespace esphome
