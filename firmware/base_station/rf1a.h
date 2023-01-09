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
// Prototype section
void InitRadio(void);
void ResetRadioCore(void);

unsigned char Strobe(unsigned char strobe);
unsigned char ReadSingleReg(unsigned char addr);
void WriteSingleReg(unsigned char addr, unsigned char value);
void ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count);
void WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count);
void ResetRadioCore(void);
void WritePATable(unsigned char value);
void WriteBurstPATable(unsigned char *buffer, unsigned char count);
void WaitForXT2(void);