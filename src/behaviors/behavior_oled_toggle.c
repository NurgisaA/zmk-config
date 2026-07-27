/*
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_oled_toggle

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_CHOSEN(zephyr_display)
static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
#endif

struct behavior_oled_toggle_config {
    bool central_target;
};

struct behavior_oled_toggle_data {
    bool blanked;
};

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    (void)event;
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_oled_toggle_config *config = dev->config;
    struct behavior_oled_toggle_data *data = dev->data;

    const bool is_central = !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL);
    if (config->central_target != is_central) {
        return ZMK_BEHAVIOR_OPAQUE;
    }

#if DT_HAS_CHOSEN(zephyr_display)
    if (!device_is_ready(display)) {
        LOG_ERR("Display device is not ready");
        return -ENODEV;
    }

    int err = data->blanked ? display_blanking_off(display) : display_blanking_on(display);
    if (err) {
        LOG_ERR("Failed to %s OLED display: %d", data->blanked ? "restore" : "blank", err);
        return err;
    }

    data->blanked = !data->blanked;
#endif

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    (void)binding;
    (void)event;
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_oled_toggle_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define OLED_TOGGLE_INST(n)                                                                       \
    static const struct behavior_oled_toggle_config oled_toggle_config_##n = {                    \
        .central_target = DT_INST_PROP_OR(n, central_target, false),                              \
    };                                                                                             \
    static struct behavior_oled_toggle_data oled_toggle_data_##n;                                 \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, &oled_toggle_data_##n, &oled_toggle_config_##n,        \
                            POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                     \
                            &behavior_oled_toggle_driver_api);

DT_INST_FOREACH_STATUS_OKAY(OLED_TOGGLE_INST)
