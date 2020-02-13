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

#include "Pins.h"
#include "Button.h"
#include "Display.h"
#include "RFID.h"

#include <TimerOne.h>

#include "logger.h"

/**
 * DEFINITIONS
 */
#define ON 1
#define OFF 0
#define BTN_DEBOUNCE_MS   250

/**
 * State machine states
 */
typedef enum
{
	STATE_IDLE = 0,
	STATE_READY,
	STATE_COUNTDOWN,
	STATE_END,
	NUM_STATES
} fsm_state_t;

/* State machine current state */
fsm_state_t cur_state = STATE_READY;

/* Clock variables */
int8_t disp_values[] = {0x00, 0x00, 0x00, 0x00};
unsigned int clock_point = 1;
unsigned int do_disp_refresh;
unsigned int millisecond_10;
signed int remaining_sec = 900;
unsigned int penalty_sec = 120;
unsigned int second;
unsigned int minute;
byte *uidByte;

/* Button to start the game */
Button btn_start(Pins::START_BTN_PIN, ACTIVE_LOW, true);

/**
 * @brief Initial setup
 *
 */
void setup()
{
	Serial.begin(9600);
	LOG_INFO("*** RFID GAME DEMO ***");

	/* Initialize display module */
	Display::setup();
	/* Display available time to solve game */
	update_display_time();
	Display::display(disp_values);

	/* Timing for 10ms */
	Timer1.initialize(10000);

	/* Initialize RFID reader */
	RFID::setup();

	/* Initialize magnet */
	digitalWrite(Pins::MAGNET_PIN, 1);
}

/**
 * @brief Update remaining time
 *
 */
void timer_isr()
{
	millisecond_10++;
	do_disp_refresh = ON;
	if (millisecond_10 == 100)
	{
		remaining_sec--;
		millisecond_10 = 0;
		clock_point = (~clock_point) & 0x01;
	}
}

/**
 * @brief Update countdown data for display
 *
 */
void update_display_time()
{

	/* Show/hide clock point */
	if (clock_point)
		Display::point(ON);
	else
		Display::point(OFF);

	/* Convert remaining seconds to minute and seconds */
	minute = remaining_sec / 60;
	second = remaining_sec % 60;

	/* Time's up, display 0xDEAD */
	if (minute == 0 && second == 0)
	{
		disp_values[0] = 0xD;
		disp_values[1] = 0xE;
		disp_values[2] = 0xA;
		disp_values[3] = 0xD;
		Display::point(OFF);
	}
	/* Display remaining time */
	else
	{
		disp_values[0] = minute / 10;
		disp_values[1] = minute % 10;
		disp_values[2] = second / 10;
		disp_values[3] = second % 10;
	}

	/* Display updated time once */
	do_disp_refresh = OFF;
}

/**
 * @brief Start/resume timer
 *
 */
void timer_resume()
{
	Timer1.resume();
}

/**
 * @brief Pause timer
 *
 */
void timer_pause()
{
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}

/**
 * @brief Reset timer
 *
 */
void timer_reset()
{
	timer_pause();
	millisecond_10 = 0;
	remaining_sec = TM1637_COUNTDOWN_S;
	do_disp_refresh = ON;
}

/**
 * @brief Wait for user to press the start button,
 * start countdown and release magnet.
 *
 */
void wait_for_start()
{
	/* Wait for start button to be pressed */
	if (btn_start.detect_single_press(BTN_DEBOUNCE_MS))
	{
		Timer1.attachInterrupt(timer_isr);
		timer_resume();
		cur_state = STATE_COUNTDOWN;

		/* Release magnet */
		digitalWrite(Pins::MAGNET_PIN, 0);
	}
}

/**
 * @brief Display the countdown and wait for user to swipe a
 * rfid tag. If the tag is correct, stop the timer and end the game.
 * If the tag is a penalty tag, decrease remaining time.
 * End the game if time's up.
 *
 */
void process_countdown()
{
	/* Time's up, game over */
	if (remaining_sec == 0)
	{
		LOG_INFO("Time's up!");
		timer_pause();
		cur_state = STATE_END;
	}

	/* Update timer */
	if (do_disp_refresh == ON)
	{
		update_display_time();
		Display::display(disp_values);
	}

	/* Reset the loop if no new card present */
	if (!RFID::is_new_card())
		return;

	uidByte = RFID::read_card_serial();

	/* Verify if the rfid tag has been read */
	if (!uidByte)
		return;

	/* Check for tag match */
	if (RFID::is_authorized(uidByte))
	{
		LOG_INFO("AUTHORIZED");
		timer_pause();
		Display::point(ON);
		Display::display(disp_values);
		cur_state = STATE_END;
		goto rfid_halt;
	}
	else if (RFID::is_penalty(uidByte))
	{
		LOG_INFO("PENALTY");
		remaining_sec -= penalty_sec;

		/* Avoid underflow */
		if(remaining_sec < 0){
			remaining_sec = 0;
		}
	}

	else
	{
		LOG_INFO("DENIED");
	}

rfid_halt:
	LOG_INFO("HALT RFID");
	RFID::halt();
}

void loop()
{
	if (cur_state >= NUM_STATES)
	{
		LOG_ERROR("invalid state");
		return;
	}

	switch (cur_state)
	{
	case STATE_IDLE:
		delay(500);
		break;
	case STATE_READY:
		wait_for_start();
		break;
	case STATE_COUNTDOWN:
		process_countdown();
		break;
	case STATE_END:
		LOG_INFO("End of game");
		cur_state = STATE_IDLE;
		break;
	default:
		break;
	}
}
