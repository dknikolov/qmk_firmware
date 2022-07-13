#pragma once

#define COMBO_COUNT 15
#define COMBO_TERM 75

#ifdef RGBLIGHT_ENABLE
#    undef RGBLIGHT_ANIMATIONS
// Comment the line below to save space and reduce the number of animations
//#    define RGBLIGHT_ANIMATIONS
// If you comment the line above, uncomment the lines below until "HERE"
#    define RGBLIGHT_SLEEP
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_TWINKLE
// "HERE"
#    ifdef RGBLIGHT_HUE_STEP
#       undef RGBLIGHT_HUE_STEP
#    endif
#    ifdef RGBLIGHT_SAT_STEP
#       undef RGBLIGHT_SAT_STEP
#    endif
#    ifdef RGBLIGHT_VAL_STEP
#       undef RGBLIGHT_VAL_STEP
#    endif
#    define RGBLIGHT_HUE_STEP 8
#    define RGBLIGHT_SAT_STEP 8
#    define RGBLIGHT_VAL_STEP 8
#    define RGBLIGHT_SLEEP
#    define RGBLIGHT_LAYERS
#    define RGBLIGHT_MAX_LAYERS 32
//#    define RGBLIGHT_STARTUP_ANIMATION true
#endif  // RGBLIGHT_ENABLE

#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_KEYPRESSES  // reacts to keypresses (will slow down matrix scan by a lot)
// #   define RGB_MATRIX_KEYRELEASES // reacts to keyreleases (not recommened)
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
// #   define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
#    define RGB_DISABLE_WHEN_USB_SUSPENDED true  // turn off effects when suspended
// #    define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_UP_DOWN
// #   define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200 // limits maximum brightness of LEDs to 200 out of 255. If not defined maximum brightness is set to 255
// #   define EECONFIG_RGB_MATRIX (uint32_t *)16

#    if defined(__AVR__) && !defined(__AVR_AT90USB1286__) && !defined(KEYBOARD_launchpad)
// Anything you disable here gets disabled globaly. 
// #        define DISABLE_RGB_MATRIX_ALPHAS_MODS
// #        define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN
// #        define DISABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
// #        define DISABLE_RGB_MATRIX_BREATHING
// #        define DISABLE_RGB_MATRIX_BAND_SAT
// #        define DISABLE_RGB_MATRIX_BAND_VAL
// #        define DISABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
// #        define DISABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
// #        define DISABLE_RGB_MATRIX_BAND_SPIRAL_SAT
// #        define DISABLE_RGB_MATRIX_BAND_SPIRAL_VAL
// #        define DISABLE_RGB_MATRIX_CYCLE_ALL
// #        define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
// #        define DISABLE_RGB_MATRIX_CYCLE_UP_DOWN
// #        define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
// #        define DISABLE_RGB_MATRIX_DUAL_BEACON
// #        define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
// #        define DISABLE_RGB_MATRIX_CYCLE_SPIRAL
// #        define DISABLE_RGB_MATRIX_RAINBOW_BEACON
// #        define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
// #        define DISABLE_RGB_MATRIX_RAINDROPS
// #        define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
// #        define DISABLE_RGB_MATRIX_DIGITAL_RAIN
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
// #        define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
// #        define DISABLE_RGB_MATRIX_SPLASH
// #        define DISABLE_RGB_MATRIX_MULTISPLASH
// #        define DISABLE_RGB_MATRIX_SOLID_SPLASH
// #        define DISABLE_RGB_MATRIX_SOLID_MULTISPLASH
#    endif  // AVR
#endif      // RGB_MATRIX_ENABLE

#ifdef MOUSEKEY_ENABLE
#  define MOUSEKEY_INTERVAL 16
#  define MOUSEKEY_DELAY 0
#  define MOUSEKEY_TIME_TO_MAX 60
#  define MOUSEKEY_MAX_SPEED 7
#  define MOUSEKEY_WHEEL_DELAY 0
#endif

#define TAP_CODE_DELAY 25

#define LEADER_TIMEOUT 300
#define LEADER_PER_KEY_TIMING

#define TAPPING_FORCE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT
