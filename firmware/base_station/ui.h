#ifndef __UI_H__
#define __UI_H__

#include "proj.h"

void display_version(void);
void display_menu(void);
void display_test(void);
void print_buf(uint8_t * data, const uint16_t size);
void print_buf16(uint16_t * data, const uint16_t size);
void parse_user_input(void);

#endif
