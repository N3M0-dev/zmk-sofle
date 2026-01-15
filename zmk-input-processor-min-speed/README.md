# ZMK Minimum Speed Input Processor

A custom ZMK module that provides a minimum speed threshold for mouse movement inputs.

## Overview

This input processor ensures that mouse movements always start from a minimum speed you define. When you press a mouse movement key, instead of starting from zero and accelerating, the cursor will immediately move at your configured minimum speed and then accelerate according to your existing acceleration settings.

## How It Works

The processor examines each mouse movement event:
- If the value is **0** (no movement), it passes through unchanged
- If the absolute value is **below the minimum threshold**, it boosts the value to the minimum while preserving direction
- If the absolute value is **above the minimum threshold**, it passes through unchanged

For example, with a minimum value of 100:
- Input value: 0 → Output: 0 (no change)
- Input value: 50 → Output: 100 (boosted to minimum)
- Input value: -30 → Output: -100 (boosted to minimum, negative preserved)
- Input value: 150 → Output: 150 (already above minimum)

## Installation

### For Local Builds

1. Clone or copy this module to your zmk-config directory structure
2. Add the module to your `config/west.yml`:

```yaml
projects:
  - name: zmk-input-processor-min-speed
    path: ../zmk-input-processor-min-speed
  - name: zmk
    remote: zmkfirmware
    revision: main
    import: app/west.yml
```

### For GitHub Actions

Add the module as a local path in your west.yml (if building from the same repository).

## Configuration

### 1. Enable the Module

Add to your board's `.conf` file (e.g., `eyelash_sofle.conf`):

```properties
CONFIG_ZMK_INPUT_PROCESSOR_MIN_SPEED=y
```

### 2. Include in Your Keymap

Add to the top of your `.keymap` file:

```c
#include <input/processors.dtsi>
#include <../zmk-input-processor-min-speed/dts/processors.dtsi>
```

### 3. Apply to Input Listeners

Modify your mouse movement input listener configuration:

```c
&mmv_input_listener { 
    input-processors = <&zip_min_speed_xy 100 &zip_xy_scaler 2 1>; 
};
```

The first parameter (100) is your minimum speed value. Adjust it based on your preference:
- Lower values (50-100): More subtle minimum speed
- Medium values (100-200): Noticeable minimum speed
- Higher values (200+): Aggressive minimum speed

### 4. Enable and Configure via Overlay

Create or modify your `.overlay` file:

```devicetree
/ {
    zip_min_speed_xy {
        status = "okay";
        min-value = <100>;  // Adjust this value
    };
};
```

## Pre-defined Processors

The module provides three pre-configured processors:

- `&zip_min_speed_xy` - Applies minimum speed to both X and Y axes
- `&zip_min_speed_x` - Applies minimum speed to X axis only
- `&zip_min_speed_y` - Applies minimum speed to Y axis only

## Custom Configuration

You can create custom instances with different settings:

```devicetree
/ {
    my_min_speed: my_min_speed {
        compatible = "zmk,input-processor-min-speed";
        #input-processor-cells = <1>;
        type = <INPUT_EV_REL>;
        codes = <INPUT_REL_X>, <INPUT_REL_Y>;
        min-value = <150>;
        status = "okay";
    };
};
```

Then use it in your input listener:

```c
&mmv_input_listener { 
    input-processors = <&my_min_speed 150>; 
};
```

## Combining with Other Processors

The minimum speed processor works well in combination with other processors. The order matters:

```c
// Apply minimum speed BEFORE scaling
&mmv_input_listener { 
    input-processors = <&zip_min_speed_xy 100 &zip_xy_scaler 2 1>; 
};

// This applies minimum speed to raw values, then scales them
```

## Tuning Tips

1. **Start Low**: Begin with a minimum value around 50-100
2. **Test Movement**: Try small movements and see if they feel responsive
3. **Adjust Gradually**: Increase by 25-50 at a time until comfortable
4. **Consider Acceleration**: Balance with your `time-to-max-speed-ms` and `acceleration-exponent` settings

## Example Configuration

Complete example from `eyelash_sofle.keymap`:

```c
#define ZMK_POINTING_DEFAULT_MOVE_VAL 800
#define ZMK_POINTING_DEFAULT_SCRL_VAL 20

#include <input/processors.dtsi>
#include <../zmk-input-processor-min-speed/dts/processors.dtsi>
#include <zephyr/dt-bindings/input/input-event-codes.h>
// ... other includes ...

&mmv_input_listener { input-processors = <&zip_min_speed_xy 100 &zip_xy_scaler 2 1>; };
&msc_input_listener { input-processors = <&zip_scroll_scaler 2 1>; };

&mmv {
    time-to-max-speed-ms = <1000>;
    acceleration-exponent = <2>;
};
```

## Troubleshooting

**Problem**: Mouse doesn't move at all
- Check that `CONFIG_ZMK_INPUT_PROCESSOR_MIN_SPEED=y` is in your .conf file
- Verify the processor status is "okay" in your overlay

**Problem**: Minimum speed is too aggressive
- Lower the `min-value` parameter
- Start with 50 and adjust upward

**Problem**: No noticeable difference
- Increase the `min-value` parameter
- Ensure the processor is listed BEFORE any scaling processors

## License

MIT License - See LICENSE file for details

## Contributing

Issues and pull requests welcome! This is a community-maintained module.
