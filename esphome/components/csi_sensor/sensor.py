import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["wifi"]
ESP_PLATFORMS = ["esp32"]

csi_ns = cg.esphome_ns.namespace("csi_sensor")
CSISensor = csi_ns.class_("CSISensor", cg.Component)

CONF_PUBLISH_INTERVAL = "publish_interval_ms"
CONF_PACKET_COUNT = "packet_count"
CONF_LAST_RSSI = "last_rssi"
CONF_CSI_LEN = "csi_len"
CONF_CHANNEL = "channel"
CONF_RATE = "rate"
CONF_NOISE_FLOOR = "noise_floor"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(CSISensor),
        cv.Optional(CONF_PUBLISH_INTERVAL, default=1000): cv.positive_int,
        cv.Optional(CONF_PACKET_COUNT): sensor.sensor_schema(
            unit_of_measurement="",
            icon="mdi:counter",
            accuracy_decimals=0,
        ).optional(),
        cv.Optional(CONF_LAST_RSSI): sensor.sensor_schema(
            unit_of_measurement="dBm",
            icon="mdi:wifi",
            accuracy_decimals=0,
        ).optional(),
        cv.Optional(CONF_CSI_LEN): sensor.sensor_schema(
            unit_of_measurement="bytes",
            icon="mdi:waves",
            accuracy_decimals=0,
        ).optional(),
        cv.Optional(CONF_CHANNEL): sensor.sensor_schema(
            unit_of_measurement="",
            icon="mdi:radio-tower",
            accuracy_decimals=0,
        ).optional(),
        cv.Optional(CONF_RATE): sensor.sensor_schema(
            unit_of_measurement="",
            icon="mdi:speedometer",
            accuracy_decimals=0,
        ).optional(),
        cv.Optional(CONF_NOISE_FLOOR): sensor.sensor_schema(
            unit_of_measurement="dBm",
            icon="mdi:chart-bell-curve",
            accuracy_decimals=0,
        ).optional(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_publish_interval(config[CONF_PUBLISH_INTERVAL]))
    await cg.register_component(var, config)

    if CONF_PACKET_COUNT in config:
        s = await sensor.new_sensor(config[CONF_PACKET_COUNT])
        cg.add(var.set_packet_count_sensor(s))
    if CONF_LAST_RSSI in config:
        s = await sensor.new_sensor(config[CONF_LAST_RSSI])
        cg.add(var.set_last_rssi_sensor(s))
    if CONF_CSI_LEN in config:
        s = await sensor.new_sensor(config[CONF_CSI_LEN])
        cg.add(var.set_csi_len_sensor(s))
    if CONF_CHANNEL in config:
        s = await sensor.new_sensor(config[CONF_CHANNEL])
        cg.add(var.set_channel_sensor(s))
    if CONF_RATE in config:
        s = await sensor.new_sensor(config[CONF_RATE])
        cg.add(var.set_rate_sensor(s))
    if CONF_NOISE_FLOOR in config:
        s = await sensor.new_sensor(config[CONF_NOISE_FLOOR])
        cg.add(var.set_noise_floor_sensor(s))
