# Sofle ZMK Keyboard Configuration

This is my custom ZMK firmware configuration for Sofle split keyboard with advanced mouse movement features.

## Features

### Custom Speed Curve Input Processor

This configuration uses a custom piecewise linear speed curve for mouse movement acceleration, providing more precise control than the default exponential/linear acceleration.

**How it works:**
- Define time-speed control points (similar to a fan curve)
- Speed is linearly interpolated between points
- Sub-pixel precision tracking for smooth movement
- Automatic timeout detection resets acceleration when keys are released

**How to customize the speed curve:**

Edit the `zip_speed_curve_xy` node in `config/eyelash_sofle.keymap`:

```c
zip_speed_curve_xy: zip_speed_curve_xy {
    compatible = "zmk,input-processor-speed-curve";
    #input-processor-cells = <0>;
    type = <INPUT_EV_REL>;
    codes = <INPUT_REL_X>, <INPUT_REL_Y>;
    
    // Customize your speed curve
    // Just like tweaking a fan curve for your machine
    curve-points = <0 50>,       // Start: 50 px/s
                   <300 200>,    // 300ms: 200 px/s
                   <1000 800>;   // 1 second: 800 px/s

    trigger-period-ms = <10>;    // Update interval (100 Hz)
    track-remainders;            // Enable sub-pixel precision
};
```

### Other Features

- **ZMK Studio support** - Configure keymap via USB (left side only)
- **Soft off mode** - Hold Q+S+Z for 2 seconds to enter deep sleep
- **Low power optimization** - Improved battery life

## Keymap

![Sofle_Keymap](keymap-drawer/eyelash_sofle.svg)

