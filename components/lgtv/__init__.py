import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

POLLING_INTERVAL = "polling_interval"
lgtv_ns = cg.esphome_ns.namespace('lgtv')
LGTVComponent = lgtv_ns.class_('LGTVComponent', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LGTVComponent),
    cv.Optional(POLLING_INTERVAL, default='60000'): cv.positive_int,
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_poll_interval(config[POLLING_INTERVAL]))