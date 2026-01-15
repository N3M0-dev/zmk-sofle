# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_INPUT_PROCESSOR_MIN_SPEED)
  target_sources(app PRIVATE src/min_speed.c)
  target_include_directories(app PRIVATE include)
endif()
