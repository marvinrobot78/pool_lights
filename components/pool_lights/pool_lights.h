#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"

namespace esphome {
  namespace pool_lights {

    class PoolLights : public light::LightOutput, public Component {
     protected:
      float brightness, on_off_state;
      int colour;
      GPIOPin *pin_;

     public:
      void setup() override {
        //delay(5000);
      }

      void set_output_pin(GPIOPin *pin) {
        this->pin_ = pin;
        this->pin_->setup();
        this->pin_->digital_write(false);
      }

      esphome::light::LightTraits get_traits() override {
        auto traits = esphome::light::LightTraits();
        traits.set_supported_color_modes({esphome::light::ColorMode::RGB, esphome::light::ColorMode::BRIGHTNESS});
        return traits;
      }
      
      void setup_state(esphome::light::LightState *new_state) override {
        brightness = new_state->current_values.get_brightness();
        on_off_state = new_state->current_values.get_state();
        colour = get_colour_from_rgb(new_state);
      }

      void write_state(esphome::light::LightState *new_state) override {
        float new_brightness = new_state->current_values.get_brightness();
        float new_on_off_state = new_state->current_values.get_state();
        int new_colour = get_colour_from_rgb(new_state);
        
        if (this->brightness != new_brightness) {
          if (this->brightness >= 0.67 && new_brightness < 0.67 && new_brightness > 0.33) {
            control_by_time(900);
          } else if (this->brightness >= 0.67 && new_brightness < 0.33) {
            control_by_time(900);
            control_by_time(900);
          } else if (this->brightness >= 0.33 && new_brightness < 0.33) {
	        control_by_time(900);
          } else if (this->brightness >= 0.33 && this->brightness < 0.67 && new_brightness >= 0.67) {
            control_by_time(900);
            control_by_time(900);
          } else if (this->brightness < 0.33 && new_brightness >= 0.67) {
            control_by_time(900);
          } else if (this->brightness < 0.33 && new_brightness < 0.67 && new_brightness > 0.33) {
            control_by_time(900);
            control_by_time(900);
          }
        }
        
        if (this->colour != new_colour) {
          if (new_colour == 6) {
            ESP_LOGD("custom","Setting light colour: white");
            control_by_time(550);
          } else if (new_colour == 5) {
            ESP_LOGD("custom","Setting light colour: light blue");
            control_by_time(500);
          } else if (new_colour == 4) {
            ESP_LOGD("custom","Setting light colour: green");
            control_by_time(450);
          } else if (new_colour == 3) {
            ESP_LOGD("custom","Setting light colour: yellow");
            control_by_time(400);
          } else if (new_colour == 2) {
            ESP_LOGD("custom","Setting light colour: red");
            control_by_time(350);
          } else if (new_colour == 1) {
            ESP_LOGD("custom","Setting light colour: blue");
            control_by_time(250);
          } else if (new_colour == 0) {
            ESP_LOGD("custom","Setting light colour: pink");
            control_by_time(300);
          }
        }
        
        if (this->on_off_state == 0.0 && new_on_off_state > 0) {
          delay(1000);
          this->pin_->digital_write(true);
        } else if (this->on_off_state > 0 && new_on_off_state == 0.0) {
          this->pin_->digital_write(false);
          delay(2000);
        }

        this->brightness = new_brightness;
        this->on_off_state = new_on_off_state;
        this->colour = new_colour;
      }
      
      
      int get_colour_from_rgb(esphome::light::LightState *new_state) {
        float red, green, blue;
        red = new_state->current_values.get_red();
        green = new_state->current_values.get_green();
        blue = new_state->current_values.get_blue();
        
        if (red >= 0.5 && green >= 0.5 && blue >= 0.5) {
          //White
          return 6;
        } else if (red >= 0.5 && green < 0.5 && blue < 0.5) {
          //Red
          return 2;
        } else if (red < 0.5 && green >= 0.5 && blue < 0.5) {
          //Green
          return 4;
        } else if (red < 0.5 && green < 0.5 && blue >= 0.5) {
          //Dark blue
          return 1;
        } else if (red >= 0.5 && green >= 0.5 && blue < 0.5) {
          //Yellow
          return 3;
        } else if (red >= 0.5 && green < 0.5 && blue >= 0.5) {
          //Pink
          return 0;
        } else if (red < 0.5 && green >= 0.5 && blue >= 0.5) {
          //Light blue
          return 5;
        } else {
          ESP_LOGD("custom","Unknown colour");
          return 0;
        }
      }
      
      void control_by_time(long time) {
        this->pin_->digital_write(false);
        delay(time);
        this->pin_->digital_write(true);
        delay(1500);
      }
    };
  }
}
