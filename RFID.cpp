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

#include "RFID.h"
#include "Pins.h"

#include "logger.h"

namespace RFID
{
	MFRC522 rfid(Pins::RFID_SS_PIN, Pins::RFID_RST_PIN);
	MFRC522::MIFARE_Key key;

	/* Authorized NUIDs */
	byte authorizedNUIDs[][8] = {
		{0x04, 0x50, 0x9A, 0x6A, 0x15, 0x4A, 0x80}};

	/* Penalty NUIDs */
	byte penaltyNUIDs[][8] = {
		{0x04, 0x5F, 0x97, 0x6A, 0x15, 0x4A, 0x81},
		{0x04, 0x6A, 0x82, 0xCA, 0x83, 0x64, 0x80}};
		
	/* Array that will store new NUID */
	byte nuidPICC[4];

	void setup()
	{
		/* Init SPI RFID reader */
		SPI.begin();	 // Init SPI bus
		rfid.PCD_Init(); // Init MFRC522

		for (byte i = 0; i < 6; i++)
		{
			key.keyByte[i] = 0xFF;
		}
	};

	bool is_authorized(byte *tag)
	{
		int idx;
		bool matchFound = false;
		int rows = sizeof(authorizedNUIDs) / sizeof(authorizedNUIDs[0]);
		int cols = sizeof(authorizedNUIDs[0]) / sizeof(byte);

		for (int idx = 0; idx < rows; idx++)
		{
			if (!memcmp(tag, authorizedNUIDs[idx], NUID_LENGTH))
				matchFound = true;
		}

		return matchFound;
	};

	bool is_penalty(byte *tag)
	{
		int idx;
		bool matchFound = false;
		int rows = sizeof(penaltyNUIDs) / sizeof(penaltyNUIDs[0]);
		int cols = sizeof(penaltyNUIDs[0]) / sizeof(byte);

		for (int idx = 0; idx < rows; idx++)
		{
			if (!memcmp(tag, penaltyNUIDs[idx], NUID_LENGTH))
				matchFound = true;
		}

		return matchFound;
	};

	bool is_new_card()
	{
		return rfid.PICC_IsNewCardPresent();
	};

	static void print_hex(byte *buffer, byte bufferSize)
	{
		for (byte i = 0; i < bufferSize; i++)
		{
			Serial.print(buffer[i] < 0x10 ? " 0" : " ");
			Serial.print(buffer[i], HEX);
		}
	};

	byte *read_card_serial()
	{
		if (!rfid.PICC_ReadCardSerial())
			return NULL;

		/* Check UID of MIFARE Classic tag */
		if (rfid.uid.uidByte[0] != nuidPICC[0] ||
			rfid.uid.uidByte[1] != nuidPICC[1] ||
			rfid.uid.uidByte[2] != nuidPICC[2] ||
			rfid.uid.uidByte[3] != nuidPICC[3])
		{
			LOG_INFO("A new card has been detected.");

			/* Store NUID into nuidPICC array */
			for (byte i = 0; i < 4; i++)
			{
				nuidPICC[i] = rfid.uid.uidByte[i];
			}
		}
		else
		{
			LOG_INFO("Card read previously.");
		}

		Serial.println(F("The NUID tag is:"));
		Serial.print(F("In hex: "));
		print_hex(rfid.uid.uidByte, rfid.uid.size);
		Serial.println();

		return rfid.uid.uidByte;
	};

	void halt()
	{
		/* Halt PICC */
		rfid.PICC_HaltA();
		/* Stop encryption on PCD */
		rfid.PCD_StopCrypto1();
	};

} // namespace RFID
