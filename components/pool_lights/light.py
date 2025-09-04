from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_OUTPUT_ID, CONF_PIN
from esphome.cpp_helpers import gpio_pin_expression

rgb_ns = cg.esphome_ns.namespace("pool_lights")
RGBLightOutput = rgb_ns.class_("PoolLights", light.LightOutput)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(RGBLightOutput),
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)

    pin = await gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_output_pin(pin))
