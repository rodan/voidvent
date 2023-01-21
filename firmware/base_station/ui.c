
#include <stdio.h>
#include <string.h>
#include "glue.h"
#include "sig.h"
#include "intertechno.h"
#include "radio.h"
#include "rf1a.h"
#include "version.h"
#include "test.h"
#include "ui.h"

extern uart_descriptor bc;

static const char menu_str[]="\
 available commands:\r\n\r\n\
\033[33;1m?\033[0m   - show menu\r\n\
\033[33;1mon\033[0m  - power on device L6\r\n\
\033[33;1moff\033[0m - power off device L6\r\n";

void display_menu(void)
{
    display_version();
    uart_print(&bc, menu_str);
}

void display_version(void)
{
    char sconv[CONV_BASE_10_BUF_SZ];

    uart_print(&bc, "it b");
    uart_print(&bc, _utoa(sconv, BUILD));
    uart_print(&bc, "\r\n");
}

void print_buf(uint8_t * data, const uint16_t size)
{
    uint16_t bytes_remaining = size;
    uint16_t bytes_to_be_printed, bytes_printed = 0;
    char itoa_buf[CONV_BASE_10_BUF_SZ];
    uint16_t i;

    while (bytes_remaining > 0) {

        if (bytes_remaining > 16) {
            bytes_to_be_printed = 16;
        } else {
            bytes_to_be_printed = bytes_remaining;
        }

        uart_print(&bc, _utoh16(itoa_buf, bytes_printed));
        uart_print(&bc, ": ");

        for (i = 0; i < bytes_to_be_printed; i++) {
            uart_print(&bc, _utoh8(itoa_buf, data[bytes_printed + i]));
            if (i & 0x1) {
                uart_print(&bc, " ");
            }
        }

        uart_print(&bc, "\r\n");
        bytes_printed += bytes_to_be_printed;
        bytes_remaining -= bytes_to_be_printed;
    }
}

#define PARSER_CNT 16

void parse_user_input(void)
{
#if defined UART_RX_USES_RINGBUF
    struct ringbuf *rbr = uart_get_rx_ringbuf(&bc);
    uint8_t rx;
    uint8_t c = 0;
    char input[PARSER_CNT];

    memset(input, 0, PARSER_CNT);

    // read the entire ringbuffer
    while (ringbuf_get(rbr, &rx)) {
        if (c < PARSER_CNT-1) {
            input[c] = rx;
        }
        c++;
    }
#else
    char *input = uart_get_rx_buf(&bc);
#endif
    char f = input[0];

    if (f == '?') {
        display_menu();
    } else if (strstr(input, "on")) {
        sig1_on;
        it_tx_cmd((INTERTECHNO_FAMILY << 4) + INTERTECHNO_DEVICE, INTERTECHNO_CMD_ON);
    } else if (strstr(input, "off")) {
        sig1_on;
        it_tx_cmd((INTERTECHNO_FAMILY << 4) + INTERTECHNO_DEVICE, INTERTECHNO_CMD_OFF);
    } else if (strstr(input, "test")) {
        test_transmit();
    } else {
        //uart_tx_str("\r\n", 2);
    }
}
