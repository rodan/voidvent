#ifndef __RF1A_H__
#define __RF1A_H__

#include <stdint.h>

typedef struct {
    uint8_t iocfg2;     // GDO2 Output Configuration
    uint8_t iocfg1;     // GDO1 Output Configuration
    uint8_t iocfg0;     // GDO0 Output Configuration
    uint8_t fifothr;    // RX FIFO and TX FIFO thresholds
    uint8_t sync1;      // Sync word, high byte
    uint8_t sync0;      // Sync word, low byte
    uint8_t pktlen;     // Packet Length
    uint8_t pktctrl1;   // Packet Automation Control
    uint8_t pktctrl0;   // Packet Automation Control
    uint8_t addr;       // Device address
    uint8_t channr;     // Channel number
    uint8_t fsctrl1;    // Frequency synthesizer control
    uint8_t fsctrl0;    // Frequency synthesizer control
    uint8_t freq2;      // Frequency Control Word, High Byte
    uint8_t freq1;      // Frequency Control Word, Middle Byte
    uint8_t freq0;      // Frequency Control Word, Low Byte
    uint8_t mdmcfg4;    // Modem Configuration
    uint8_t mdmcfg3;    // Modem Configuration
    uint8_t mdmcfg2;    // Modem Configuration
    uint8_t mdmcfg1;    // Modem Configuration
    uint8_t mdmcfg0;    // Modem Configuration
    uint8_t deviatn;    // Modem deviation setting
    uint8_t mcsm2;      // Main Radio Control State Machine Configuration
    uint8_t mcsm1;      // Main Radio Control State Machine Configuration
    uint8_t mcsm0;      // Main Radio Control State Machine Configuration
    uint8_t foccfg;     // Frequency Offset Compensation Configuration
    uint8_t bscfg;      // Bit Synchronization configuration
    uint8_t agcctrl2;   // Automatic gain control
    uint8_t agcctrl1;   // Automatic gain control
    uint8_t agcctrl0;   // Automatic gain control
    uint8_t worevt1;    // High Byte Event0 Timeout
    uint8_t worevt0;    // Low Byte Event0 Timeout
    uint8_t worctrl;    // Wake On Radio Control
    uint8_t frend1;     // Front End RX Configuration
    uint8_t frend0;     // Front End TX Configuration
    uint8_t fscal3;     // Frequency Synthesizer Calibration
    uint8_t fscal2;     // Frequency Synthesizer Calibration
    uint8_t fscal1;     // Frequency Synthesizer Calibration
    uint8_t fscal0;     // Frequency Synthesizer Calibration
    uint8_t fstest;     // Frequency synthesizer calibration control
    uint8_t ptest;      // Production test
    uint8_t agctest;    // AGC test
    uint8_t test2;      // Various test settings
    uint8_t test1;      // Various test settings
    uint8_t test0;      // Various test settings
} RF_SETTINGS;

#ifdef __cplusplus
extern "C" {
#endif

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
void WriteRfSettings(const RF_SETTINGS *pRfSettings);

#ifdef __cplusplus
}
#endif

#endif


