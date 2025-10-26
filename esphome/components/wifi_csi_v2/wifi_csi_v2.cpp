#include "wifi_csi_v2.h"
#ifdef USE_WIFI
#include "esphome/core/log.h"

namespace esphome {
namespace wifi_csi_v2 {

static const char *const TAG = "wifi_csi_v2.sensor";

void WiFiCSISensor::dump_config() { LOG_SENSOR("", "WiFi CSI", this); }

}  // namespace wifi_csi_v2
}  // namespace esphome
#endif
