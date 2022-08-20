# Build Options
#   change yes to no to disable
#
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no
PIMORONI_TRACKBALL_ENABLE = no
OLED_ENABLE = no
ENCODER_ENABLE = no
CIRQUE_ENABLE = no
MOUSEKEY_ENABLE = yes
PMW3360_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3360

ifeq ($(strip $(CIRQUE_ENABLE)), yes)
   POINTING_DEVICE_ENABLE = yes
   POINTING_DEVICE_DRIVER = cirque_pinnacle_i2c
endif

ifeq ($(strip $(PMW3360_ENABLE)), yes)
    POINTING_DEVICE_ENABLE := yes
    POINTING_DEVICE_DRIVER = pmw3360
    QUANTUM_LIB_SRC += spi_master.c
endif
