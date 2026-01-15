/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_input_processor_min_speed

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/input_processor.h>
#include <zephyr/logging/log.h>

#include <zmk/input_processors/min_speed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/**
 * @brief Check if a given code is in the processor's code array
 */
static bool code_matches(const struct zip_min_speed_config *cfg, uint16_t code) {
    for (size_t i = 0; i < cfg->codes_len; i++) {
        if (cfg->codes[i] == code) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Process input event and apply minimum speed if needed
 */
static int zip_min_speed_handle_event(const struct device *dev,
                                       struct input_event *event,
                                       uint32_t param1, uint32_t param2,
                                       struct zmk_input_processor_state *state) {
    const struct zip_min_speed_config *cfg = dev->config;

    // Only process matching event types and codes
    if (event->type != cfg->type || !code_matches(cfg, event->code)) {
        return 0;
    }

    // Get the current value
    int32_t value = event->value;
    
    // If value is 0, don't modify it
    if (value == 0) {
        return 0;
    }

    // Get absolute value
    int32_t abs_value = (value < 0) ? -value : value;
    
    // If absolute value is below minimum, boost it to minimum while preserving sign
    if (abs_value < cfg->min_value) {
        event->value = (value < 0) ? -cfg->min_value : cfg->min_value;
        
        LOG_DBG("Min speed applied: original=%d, new=%d (min=%d)", 
                value, event->value, cfg->min_value);
    }

    return 0;
}

/**
 * @brief Initialize the minimum speed input processor
 */
static int zip_min_speed_init(const struct device *dev) {
    LOG_DBG("Initializing minimum speed input processor: %s", dev->name);
    return 0;
}

static const struct zmk_input_processor_driver_api zip_min_speed_driver_api = {
    .handle_event = zip_min_speed_handle_event,
};

#define ZIP_MIN_SPEED_INST(n)                                                           \
    static const uint16_t zip_min_speed_codes_##n[] = DT_INST_PROP(n, codes);         \
    static const struct zip_min_speed_config zip_min_speed_config_##n = {             \
        .type = DT_INST_PROP(n, type),                                                 \
        .codes = zip_min_speed_codes_##n,                                              \
        .codes_len = DT_INST_PROP_LEN(n, codes),                                       \
        .min_value = DT_INST_PROP(n, min_value),                                       \
    };                                                                                  \
    static struct zip_min_speed_data zip_min_speed_data_##n = {};                     \
    DEVICE_DT_INST_DEFINE(n, zip_min_speed_init, NULL,                                \
                          &zip_min_speed_data_##n, &zip_min_speed_config_##n,         \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,            \
                          &zip_min_speed_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ZIP_MIN_SPEED_INST)
