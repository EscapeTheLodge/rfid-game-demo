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
#ifndef PINS_H
#define PINS_H

namespace Pins {
    enum {
        DISP_CLK_PIN = 2,
        DISP_DIO_PIN = 3,
        START_BTN_PIN = 4,
        MAGNET_PIN = 5,
        RFID_RST_PIN = 9,
        RFID_SS_PIN = 10,
    };
} //namespace Pins

#endif