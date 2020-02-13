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

#include "Display.h"
#include "Pins.h"

namespace Display
{
    TM1637 tm1637(Pins::DISP_CLK_PIN, Pins::DISP_DIO_PIN);

    void setup()
    {
        /* Init TM1637 display */
        tm1637.set(BRIGHTEST);
        tm1637.init();
    };

    void point(bool enable)
    {
        tm1637.point(enable);
    };

    void display(int8_t data[])
    {
        tm1637.display(data);
    };
} // namespace Display