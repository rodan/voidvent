
#include <stdio.h>
#include <string.h>
#include "glue.h"
#include "sig.h"
#include "intertechno.h"
#include "version.h"
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

    uint8_t it_family = 12;
    uint8_t it_device = 6;

    if (f == '?') {
        display_menu();
    } else if (strstr(input, "on")) {
        sig0_on;
        it_tx_cmd(((it_family - 1) << 4) + it_device - 1, INTERTECHNO_CMD_ON);
    } else if (strstr(input, "off")) {
        sig0_on;
        it_tx_cmd(((it_family - 1) << 4) + it_device - 1, INTERTECHNO_CMD_OFF);
    } else {
        //uart_tx_str("\r\n", 2);
    }
}
