#include "csi_sensor.h"
#ifdef USE_WIFI
#include "esphome/core/log.h"

namespace esphome {
namespace csi_sensor {

static const char *const TAG = "csi_sensor.sensor";

void CSISensor::dump_config() { LOG_SENSOR("", "CSI", this); }

}  // namespace csi_sensor
}  // namespace esphome
#endif
