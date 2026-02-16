#pragma once

// Disable RGB in sleep mode
#ifdef RGB_MATRIX_ENABLE
    #define RGB_DISABLE_WHEN_USB_SUSPENDED
    #define RGBLIGHT_SLEEP
#endif

// Fix Chatter
#undef DEBOUNCE
#define DEBOUNCE 15

// Performance
#define USB_POLLING_INTERVAL_MS 1
