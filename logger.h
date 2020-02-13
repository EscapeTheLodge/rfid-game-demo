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

#ifndef LOGGER_H
#define LOGGER_H

#define NO_LOGS         0x0
#define ERROR_LEVEL     0x1
#define INFO_LEVEL      0x2
#define DEBUG_LEVEL       0x3

#ifndef LOG_LEVEL
    #define LOG_LEVEL   0x2
#endif

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_DEBUG(message)     \
    {                          \
    Serial.print("DEBUG: ");   \
    Serial.println(message);   \
    }
#else
#define LOG_DEBUG(message)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define LOG_INFO(message)     \
    {                         \
    Serial.print("INFO: ");   \
    Serial.println(message);  \
    }
#else
#define LOG_INFO(message)
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define LOG_ERROR(message)     \
    {                          \
    Serial.print("ERROR: ");   \
    Serial.println(message);   \
    }
#else
#define LOG_ERROR(message)
#endif

#endif

