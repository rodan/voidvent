// *************************************************************************************************
//
// Actual revision: $Revision: $
// Revision label:  $Name: $
// Revision state:  $State: $
//
// *************************************************************************************************
// Radio core access functions. Taken from TI reference code for CC430.
// *************************************************************************************************


// *************************************************************************************************
// Include section

// driver
#include <stdint.h>
#include <msp430.h>
#include "rf1a.h"

// *************************************************************************************************
// Global section


// *************************************************************************************************
// Define section
#define st(x)      			do { x } while (__LINE__ == -1)
#define ENTER_CRITICAL_SECTION(x)  	st( x = __get_SR_register(); __dint(); )
#define EXIT_CRITICAL_SECTION(x)	__bis_SR_register(x)


// *************************************************************************************************
// @fn          Strobe
// @brief       Send command to radio.
// @param       none
// @return      none
// *************************************************************************************************
unsigned char Strobe(unsigned char strobe)
{
	uint8_t statusByte = 0;
	uint16_t int_state, gdo_state;

	// Check for valid strobe command
	if ((strobe == 0xBD) || ((strobe > RF_SRES) && (strobe < RF_SNOP))) {
		ENTER_CRITICAL_SECTION(int_state);

		// Clear the Status read flag
		RF1AIFCTL1 &= ~(RFSTATIFG);

		// Wait for radio to be ready for next instruction
		while (!(RF1AIFCTL1 & RFINSTRIFG));

		// Write the strobe instruction
		if ((strobe > RF_SRES) && (strobe < RF_SNOP)) {

			gdo_state = ReadSingleReg(IOCFG2); // buffer IOCFG2 state
			WriteSingleReg(IOCFG2, 0x29); // c-ready to GDO2

			RF1AINSTRB = strobe;

			if ((RF1AIN & 0x04) == 0x04) {		// chip at sleep mode
				if ((strobe == RF_SXOFF) || (strobe == RF_SPWD) || (strobe == RF_SWOR)) { }
				else {
					while ((RF1AIN & 0x04) == 0x04);       			// c-ready ?

					__delay_cycles(9800);	// Delay for ~810usec at 12MHz CPU clock
				}
			}

			WriteSingleReg(IOCFG2, gdo_state); // restore IOCFG2 setting
		} else {	// chip active mode
			RF1AINSTRB = strobe;
		}

		statusByte = RF1ASTATB;

		while (!(RF1AIFCTL1 & RFSTATIFG));

		EXIT_CRITICAL_SECTION(int_state);
	}

	return statusByte;
}


// *************************************************************************************************
// @fn          ResetRadioCore
// @brief       Software reset radio core.
// @param       none
// @return      none
// *************************************************************************************************
void ResetRadioCore(void)
{
	Strobe(RF_SRES);                             // Reset the Radio Core
	Strobe(RF_SNOP);                             // Reset Radio Pointer
}

// *****************************************************************************
// @fn          WriteRfSettings
// @brief       Write the minimum set of RF configuration register settings
// @param       RF_SETTINGS *pRfSettings  Pointer to the structure that holds the rf settings
// @return      none
// *****************************************************************************
void WriteRfSettings(const RF_SETTINGS *pRfSettings) {
    WriteSingleReg(IOCFG2,   pRfSettings->iocfg2);
    WriteSingleReg(IOCFG1,   pRfSettings->iocfg1);
    WriteSingleReg(IOCFG0,   pRfSettings->iocfg0);
    WriteSingleReg(FIFOTHR,  pRfSettings->fifothr);
    WriteSingleReg(SYNC1,    pRfSettings->sync1);
    WriteSingleReg(SYNC0,    pRfSettings->sync0);
    WriteSingleReg(PKTLEN,   pRfSettings->pktlen);
    WriteSingleReg(PKTCTRL1, pRfSettings->pktctrl1);
    WriteSingleReg(PKTCTRL0, pRfSettings->pktctrl0);
    WriteSingleReg(ADDR,     pRfSettings->addr);
    WriteSingleReg(CHANNR,   pRfSettings->channr);
    WriteSingleReg(FSCTRL1,  pRfSettings->fsctrl1);
    WriteSingleReg(FSCTRL0,  pRfSettings->fsctrl0);
    WriteSingleReg(FREQ2,    pRfSettings->freq2);
    WriteSingleReg(FREQ1,    pRfSettings->freq1);
    WriteSingleReg(FREQ0,    pRfSettings->freq0);
    WriteSingleReg(MDMCFG4,  pRfSettings->mdmcfg4);
    WriteSingleReg(MDMCFG3,  pRfSettings->mdmcfg3);
    WriteSingleReg(MDMCFG2,  pRfSettings->mdmcfg2);
    WriteSingleReg(MDMCFG1,  pRfSettings->mdmcfg1);
    WriteSingleReg(MDMCFG0,  pRfSettings->mdmcfg0);
    WriteSingleReg(DEVIATN,  pRfSettings->deviatn);
    WriteSingleReg(MCSM2,    pRfSettings->mcsm2);
    WriteSingleReg(MCSM1,    pRfSettings->mcsm1);
    WriteSingleReg(MCSM0,    pRfSettings->mcsm0);
    WriteSingleReg(FOCCFG,   pRfSettings->foccfg);
    WriteSingleReg(BSCFG,    pRfSettings->bscfg);
    WriteSingleReg(AGCCTRL2, pRfSettings->agcctrl2);
    WriteSingleReg(AGCCTRL1, pRfSettings->agcctrl1);
    WriteSingleReg(AGCCTRL0, pRfSettings->agcctrl0);
    WriteSingleReg(WOREVT1,  pRfSettings->worevt1);
    WriteSingleReg(WOREVT0,  pRfSettings->worevt0);
    WriteSingleReg(WORCTRL,  pRfSettings->worctrl);
    WriteSingleReg(FREND1,   pRfSettings->frend1);
    WriteSingleReg(FREND0,   pRfSettings->frend0);
    WriteSingleReg(FSCAL3,   pRfSettings->fscal3);
    WriteSingleReg(FSCAL2,   pRfSettings->fscal2);
    WriteSingleReg(FSCAL1,   pRfSettings->fscal1);
    WriteSingleReg(FSCAL0,   pRfSettings->fscal0);
    WriteSingleReg(FSTEST,   pRfSettings->fstest);
    WriteSingleReg(PTEST,    pRfSettings->ptest);
    WriteSingleReg(AGCTEST,  pRfSettings->agctest);
    WriteSingleReg(TEST2,    pRfSettings->test2);
    WriteSingleReg(TEST1,    pRfSettings->test1);
    WriteSingleReg(TEST0,    pRfSettings->test0);
}

// *************************************************************************************************
// @fn          ReadSingleReg
// @brief       Read byte from register.
// @param       none
// @return      none
// *************************************************************************************************
unsigned char ReadSingleReg(unsigned char addr)
{
	unsigned char x;
	uint16_t int_state;

	ENTER_CRITICAL_SECTION(int_state);

	RF1AINSTR1B = (addr | RF_REGRD);
	x = RF1ADOUT1B;

	EXIT_CRITICAL_SECTION(int_state);

	return x;
}


// *************************************************************************************************
// @fn          WriteSingleReg
// @brief       Write byte to register.
// @param       none
// @return      none
// *************************************************************************************************
void WriteSingleReg(unsigned char addr, unsigned char value)
{
	__attribute__((unused)) volatile unsigned int i;
	uint16_t int_state;

	ENTER_CRITICAL_SECTION(int_state);

	while (!(RF1AIFCTL1 & RFINSTRIFG));     // Wait for the Radio to be ready for the next instruction

	RF1AINSTRW = ((addr | RF_REGWR) << 8) + value; // Send address + Instruction

	while (!(RFDINIFG & RF1AIFCTL1));

	i = RF1ADOUTB;                            // Reset RFDOUTIFG flag which contains status byte

	EXIT_CRITICAL_SECTION(int_state);
}


// *************************************************************************************************
// @fn          ReadBurstReg
// @brief       Read sequence of bytes from register.
// @param       none
// @return      none
// *************************************************************************************************
void ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count)
{
	unsigned int i;
	uint16_t int_state;

	ENTER_CRITICAL_SECTION(int_state);

	while (!(RF1AIFCTL1 & RFINSTRIFG));       // Wait for the Radio to be ready for next instruction

	RF1AINSTR1B = (addr | RF_REGRD);          // Send address + Instruction

	for (i = 0; i < (count - 1); i++) {
		while (!(RFDOUTIFG & RF1AIFCTL1));      // Wait for the Radio Core to update the RF1ADOUTB reg

		buffer[i] = RF1ADOUT1B;                 // Read DOUT from Radio Core + clears RFDOUTIFG
		// Also initiates auo-read for next DOUT byte
	}

	buffer[count - 1] = RF1ADOUT0B;           // Store the last DOUT from Radio Core

	EXIT_CRITICAL_SECTION(int_state);
}


// *************************************************************************************************
// @fn          WriteBurstReg
// @brief       Write sequence of bytes to register.
// @param       none
// @return      none
// *************************************************************************************************
void WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count)
{
	// Write Burst works wordwise not bytewise - bug known already
	unsigned char i;
	uint16_t int_state;

	ENTER_CRITICAL_SECTION(int_state);

	while (!(RF1AIFCTL1 & RFINSTRIFG));       // Wait for the Radio to be ready for next instruction

	RF1AINSTRW = ((addr | RF_REGWR) << 8) + buffer[0]; // Send address + Instruction

	for (i = 1; i < count; i++) {
		RF1ADINB = buffer[i];                   // Send data

		while (!(RFDINIFG & RF1AIFCTL1));       // Wait for TX to finish
	}

	i = RF1ADOUTB;                            // Reset RFDOUTIFG flag which contains status byte

	EXIT_CRITICAL_SECTION(int_state);
}


// *************************************************************************************************
// @fn          WritePATable
// @brief       Write data to power table
// @param       unsigned char value		Value to write
// @return      none
// *************************************************************************************************
void WritePATable(unsigned char value)
{
	unsigned char readbackPATableValue = 0;
	uint16_t int_state;

	ENTER_CRITICAL_SECTION(int_state);

	while (readbackPATableValue != value) {
		while (!(RF1AIFCTL1 & RFINSTRIFG));

		RF1AINSTRW = 0x7E00 + value;               // PA Table write (burst)

		while (!(RF1AIFCTL1 & RFINSTRIFG));

		RF1AINSTRB = RF_SNOP;                      // reset pointer


		while (!(RF1AIFCTL1 & RFINSTRIFG));

		RF1AINSTRB = 0xFE;                      // PA Table read (burst)

		while (!(RF1AIFCTL1 & RFDINIFG));

		RF1ADINB    = 0x00;                     //dummy write

		while (!(RF1AIFCTL1 & RFDOUTIFG));

		readbackPATableValue = RF1ADOUT0B;

		while (!(RF1AIFCTL1 & RFINSTRIFG));

		RF1AINSTRB = RF_SNOP;
	}

	EXIT_CRITICAL_SECTION(int_state);
}

// *****************************************************************************
// @fn          WriteBurstPATable
// @brief       Write to multiple locations in power table 
// @param       unsigned char *buffer   Pointer to the table of values to be written 
// @param       unsigned char count     Number of values to be written
// @return      none
// *****************************************************************************
void WriteBurstPATable(unsigned char *buffer, unsigned char count)
{
	volatile unsigned char i = 0;
	uint16_t int_state;

	ENTER_CRITICAL_SECTION(int_state);

	while (!(RF1AIFCTL1 & RFINSTRIFG)) ;
	RF1AINSTRW = 0x7E00 + buffer[i];    // PA Table burst write   

	for (i = 1; i < count; i++) {
		RF1ADINB = buffer[i];   // Send data
		while (!(RFDINIFG & RF1AIFCTL1)) ;      // Wait for TX to finish
	}
	i = RF1ADOUTB;              // Reset RFDOUTIFG flag which contains status byte

	while (!(RF1AIFCTL1 & RFINSTRIFG)) ;
	RF1AINSTRB = RF_SNOP;       // reset PA Table pointer

	EXIT_CRITICAL_SECTION(int_state);
}

void InitRadio(void)
{
  // Set the High-Power Mode Request Enable bit so LPM3 can be entered
  // with active radio enabled 
  PMMCTL0_H = 0xA5;
  PMMCTL0_L |= PMMHPMRE_L; 
  PMMCTL0_H = 0x00; 
  
  //WriteRfSettings(&rfSettings);
  //WriteSinglePATable(PATABLE_VAL);
}

