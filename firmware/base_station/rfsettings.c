
#include "rf1a.h"
#include "intertechno.h"

//#define RF_INTERTECHNO_FIXED
#define RF_FIXED_LT_FIFO

#if defined(RF_DEFAULTS)
// Address Config = No address check 
// Base Frequency = 799.999878 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 799.999878 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 115.051 
// Deviation = 47.607422 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-FSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 203.125000 
// Sync Word Qualifier Mode = 16/16 sync word bits detected 
// TX Power = unknown 
// Whitening = true 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x2e,  // IOCFG0        GDO0 Output Configuration
    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x45,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x0f,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x1e,  // FREQ2         Frequency Control Word, High Byte
    0xc4,  // FREQ1         Frequency Control Word, Middle Byte
    0xec,  // FREQ0         Frequency Control Word, Low Byte
    0x8c,  // MDMCFG4       Modem Configuration
    0x22,  // MDMCFG3       Modem Configuration
    0x02,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x47,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x00,  // MCSM0         Main Radio Control State Machine Configuration
    0x36,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x03,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xf0,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xea,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x88,  // TEST2         Various Test Settings
    0x31,  // TEST1         Various Test Settings
    0x0b,  // TEST0         Various Test Settings
};

#elif defined(RF_INTERTECHNO_FIXED)
// Address Config = No address check 
// Base Frequency = 433.919830 
// CRC Autoflush = false 
// CRC Enable = false 
// Carrier Frequency = 433.919830 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 2.28119 
// Deviation = 47.607422 
// Device Address = 0 
// Manchester Enable = false 
// Modulation Format = ASK/OOK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Fixed packet length mode. Length configured in PKTLEN register 
// Preamble Count = 2 
// RX Filter BW = 203.125000 
// Sync Word Qualifier Mode = 16/16 sync word bits detected 
// TX Power = 10 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x09,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x00,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x0f,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x10,  // FREQ2         Frequency Control Word, High Byte
    0xb0,  // FREQ1         Frequency Control Word, Middle Byte
    0x71,  // FREQ0         Frequency Control Word, Low Byte
    0x86,  // MDMCFG4       Modem Configuration
    0x70,  // MDMCFG3       Modem Configuration
    0x32,  // MDMCFG2       Modem Configuration
    0x02,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x47,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x00,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x43,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xf0,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x11,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x88,  // TEST2         Various Test Settings
    0x31,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_FIXED_LT_FIFO)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 38.3835 
// Deviation = 19.042969 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-GFSK 
// PA Ramping = false 
// Packet Length = 5 
// Packet Length Mode = Fixed packet length mode. Length configured in PKTLEN register 
// Preamble Count = 4 
// RX Filter BW = 101.562500 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = unknown 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0x05,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x04,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x08,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xca,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x93,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x34,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x18,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x43,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xf0,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_1KBAUD_HIGHSENS_FSK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 1.19948 
// Deviation = 5.157471 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-FSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 58.035714 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x05,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xf5,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x03,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x15,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x03,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_1KBAUD_HIGHSENS_OOK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = false 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Synchronous serial mode 
// Data Rate = 1.19948 
// Deviation = 5.157471 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = false 
// Modulation Format = ASK/OOK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Infinite packet length mode 
// Preamble Count = 4 
// RX Filter BW = 58.035714 
// Sync Word Qualifier Mode = No preamble/sync 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x0b,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x2d,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x12,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xf5,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x30,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x15,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x03,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x11,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_1KBAUD_LOWPWR_FSK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 1.19948 
// Deviation = 5.157471 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-FSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 58.035714 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x05,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xf5,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x83,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x15,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x03,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_1KBAUD_LOWPWR_OOK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = false 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Synchronous serial mode 
// Data Rate = 1.19948 
// Deviation = 5.157471 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = false 
// Modulation Format = ASK/OOK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Infinite packet length mode 
// Preamble Count = 4 
// RX Filter BW = 58.035714 
// Sync Word Qualifier Mode = No preamble/sync 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x0b,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x2d,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x12,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xf5,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0xb0,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x15,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x03,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x11,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_38KBAUD_HIGHSENS_FSK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 38.3835 
// Deviation = 20.629883 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-GFSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 101.562500 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x05,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xca,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x13,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x35,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x43,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_38KBAUD_HIGHSENS_OOK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = false 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Synchronous serial mode 
// Data Rate = 38.3835 
// Deviation = 20.629883 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = false 
// Modulation Format = ASK/OOK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Infinite packet length mode 
// Preamble Count = 4 
// RX Filter BW = 101.562500 
// Sync Word Qualifier Mode = No preamble/sync 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x0b,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x2d,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x12,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xca,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x30,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x35,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x43,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x11,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_38KBAUD_LOWPWR_FSK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 38.3835 
// Deviation = 20.629883 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-GFSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 101.562500 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x29,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x05,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x08,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xca,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x93,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x35,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x43,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#elif defined(RF_38KBAUD_LOWPWR_OOK)
// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = false 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Synchronous serial mode 
// Data Rate = 38.3835 
// Deviation = 20.629883 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = false 
// Modulation Format = ASK/OOK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Infinite packet length mode 
// Preamble Count = 4 
// RX Filter BW = 101.562500 
// Sync Word Qualifier Mode = No preamble/sync 
// TX Power = 0 
// Whitening = false 
// Rf settings for CC430
const RF_SETTINGS rfSettings = {
    0x0b,  // IOCFG2        GDO2 Output Configuration
    0x2e,  // IOCFG1        GDO1 Output Configuration
    0x2d,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xd3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0xff,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x12,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x08,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x21,  // FREQ2         Frequency Control Word, High Byte
    0x62,  // FREQ1         Frequency Control Word, Middle Byte
    0x76,  // FREQ0         Frequency Control Word, Low Byte
    0xca,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0xb0,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xf8,  // MDMCFG0       Modem Configuration
    0x35,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x6c,  // BSCFG         Bit Synchronization Configuration
    0x43,  // AGCCTRL2      AGC Control
    0x40,  // AGCCTRL1      AGC Control
    0x91,  // AGCCTRL0      AGC Control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xfb,  // WORCTRL       Wake On Radio Control
    0x56,  // FREND1        Front End RX Configuration
    0x11,  // FREND0        Front End TX Configuration
    0xe9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2a,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1f,  // FSCAL0        Frequency Synthesizer Calibration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7f,  // PTEST         Production Test
    0x3f,  // AGCTEST       AGC Test
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
#endif

#if 0
// Rf settings for CC430 generated by SmartRF Studio
const RF_SETTINGS rfSettings_fixed = {
    0x29,  // IOCFG2 (RF_RDYn) GDO2 Output Configuration
    0x09,  //0x2E,  // IOCFG1        GDO1 Output Configuration
    0x06,  // IOCFG0        GDO0 Output Configuration
    INTERTECHNO_SEQ_SIZE,  // PKTLEN        Packet Length
    0x00,  // PKTCTRL0      Packet Automation Control
    0x10,  // FREQ2         Frequency Control Word, High Byte
    0xB0,  // FREQ1         Frequency Control Word, Middle Byte
    0x71,  // FREQ0         Frequency Control Word, Low Byte
    0x86,  // MDMCFG4       Modem Configuration
    0x70,  // MDMCFG3       Modem Configuration
    0x32,  // MDMCFG2       Modem Configuration
    0x02,  // MDMCFG1       Modem Configuration
    0x30,  // MCSM1         Main Radio Control State Machine Configuration
    0x00,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x43,  // AGCCTRL2      AGC control
    0x40,  // AGCCTRL1      AGC control
    0x91,  // AGCCTRL0      AGC control
    0x80,  // WOREVT1       High Byte Event0 Timeout
    0x00,  // WOREVT0       Low Byte Event0 Timeout
    0xF0,  // WORCTRL       Wake On Radio Control
    0x11,  // FREND0        Front End TX Configuration
    0xE9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2A,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1F,  // FSCAL0        Frequency Synthesizer Calibration
    0x09,  // TEST0         Various Test Settings
};
#endif
