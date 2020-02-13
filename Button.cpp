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

#include "Button.h"

/**
 * Construct button instance
 * @param pin           button pin number
 * @param btn_logic     button logic, can be either ACTIVE_LOW
 *                      or ACTIVE_HIGH
 */
Button::Button(int pin, enum active_state btn_logic, bool use_int_pullup)
{
    if (use_int_pullup)
        pinMode(pin, INPUT_PULLUP);
    else
        pinMode(pin, INPUT);

    _pin = pin;
    _active_state = btn_logic;
    _last_press_time = 0;
    _last_press_val = LOW;
    _use_int_pullup = use_int_pullup;
}

/**
 * Return whether button is pressed
 * @return  true is button is pressed according to logic,
 *          false otherwise
 */
bool Button::is_pressed()
{
    int btn_val = digitalRead(_pin);

    if (_active_state == ACTIVE_HIGH)
        return (btn_val == HIGH);
    else
        return (btn_val == LOW);
}

/**
 * Returns true if a single press event is detected
 * @param debounce_ms   press debounce time (in milliseconds)
 */
bool Button::detect_single_press(long debounce_ms)
{
    int press_val = digitalRead(_pin);
    bool press_detected = false;
    if (_active_state == ACTIVE_HIGH)
    {
        if (press_val == HIGH && _last_press_val == LOW && millis() - _last_press_time > debounce_ms)
        {
            press_detected = true;
            _last_press_time = millis();
        }
    }
    else
    {
        if (press_val == LOW && _last_press_val == HIGH && millis() - _last_press_time > debounce_ms)
        {
            press_detected = true;
            _last_press_time = millis();
        }
    }

    _last_press_val = press_val;
    return press_detected;
}