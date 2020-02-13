/*
 *   Copyright (c) 2020 The Lodge

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * Library for handling a button
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

enum active_state {
    ACTIVE_HIGH,
    ACTIVE_LOW
};

class Button {
    public:
        Button(int pin, enum active_state btn_logic, bool use_int_pullup);
        bool is_pressed();
        bool detect_single_press(long debounce_ms);
    private:
        int _pin;
        int _active_state;
        long _last_press_time;
        int _last_press_val;
        bool _use_int_pullup;
};

#endif