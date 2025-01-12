
#include "headers/internal_setting.h"
#include "headers/info.h"
#include "headers/uart.h"
#include "string.h"
#include "stdio.h"
#include <xc.h>
#include <pic18f4520.h>

extern char display_info[MAX_INFO_LEN];
extern int info_len;

unsigned char info_read(){
    int reach_end = 0;
    
    unsigned char read_char = uart_read();
    display_info[info_len ++] = read_char;
    if(read_char == '\n'){
        reach_end = 1;
        display_info[info_len] = '\0';
    }
    
    if(reach_end){
        unsigned char ret = check_user();
        screen_display();
        return ret;
    }
    return 0;
}

unsigned char check_user(){
    // Check whether the first 5 elements are the same as "Hello"
    char hello[] = "Hello";
    for(int i = 0; i < 5; ++i){
        if(hello[i] != display_info[i])
            return 0;
    }
    return 1;
}