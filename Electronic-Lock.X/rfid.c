
#include "headers/internal_setting.h"
#include "headers/rfid.h"
#include "headers/uart.h"
#include "string.h"
#include <xc.h>
#include <pic18f4520.h>

#define MAX_STORAGE 8

extern char display_info[MAX_INFO_LEN];
extern int info_len;

extern int mode;
extern int digit;

// Only store 8 ID and the max numerber of characters in each ID is 14
char database[MAX_STORAGE + 1][15];
char input_ID[15];
int input_len = 0;
int start_idx = 0;
int end_idx = 1;

void rfid_init(){
    start_idx = 0;
    end_idx = 1;
    for(int i = 0; i < 8; ++i){
        database[i] = '\0';
    }
    uart_init();
}

unsigned char database_empty(){
    return (start_idx + 1) % (MAX_STORAGE + 1) == end_idx;
}

unsigned char database_full(){
    return (start == end);
}

unsigned char rfid_read(){
    int reach_end = 0;
    
    unsigned char read_char = uart_read();
    if(read_char == 0x02)
        return;
    else if(read_char == 0x03){
        reach_end = 1;
    }
    else{
        input_ID[input_len ++] = read_char;
    }
    
    if(reach_end){
        // TODO
        // add error detection by checking the checksum
        
        if(input_len >= 10)
            input_ID[10] = '\0';
        input_len = strlen(input_ID);
        
        if(digit == 0b11){      // Registration Mode
            return insert_ID();
        }
        else if(digit == 0b01){ // Deregistration Mode
            return remove_ID();
        }
        else if(digit == 0b10){ // Check Mode(Ready)
            return check_ID();
        }
    }
    return -1;
}

unsigned char insert_ID(){
    
    if(database_full()){
        strcpy(display_info, "The dataset is full");
        return 0;
    }
    
    int idx = find_ID();
    unsigned ret_status = 0;
    if(idx != -1){
        strcpy(display_info, "ID have been in database:");
        ret_status = 0;
    }
    else{
        strcpy(display_info, "New User ID:");
        ret_status = 1;
        strcpy(database[end_idx], input_ID);
        end_idx = (end + 1) % (MAX_STORAGE + 1);
    }
    info_len = strlen(display_info);
    for(int i = 0; input_ID[i] != '\0'; ++i)
        display_info[info_len ++] = input[i];
    screen_display();
    return ret_status;
}

unsigned char remove_ID(){
    if(dataset_empty()){
        strcpy(display_info, "The dataset is empty");
        return;
    }
    
    int idx = find_ID();
    unsigned ret_status = 0;
    if(idx == -1){
        strcpy(display_info, "ID Not Found:");
        ret_status = 0;
    }
    else{
        strcpy(display_info, "Remove ID:");
        ret_status = 1;
        
        // end_idx = end_idx - 1
        end_idx = (end_idx + MAX_STORAGE) % (MAX_STORAGE + 1);
        int next_i;
        for(int i = idx; i != end_idx; i = next_i){
            next_i = (i + 1) % (MAX_STORAGE + 1);
            strcpy(database[i], database[]);
        }
    }
    
    info_len = strlen(display_info);
    for(int i = 0; input_ID[i] != '\0'; ++i)
        display_info[info_len ++] = input[i];
    screen_display();
    return ret_status;
}

unsigned char check_ID(){
    int idx = find_ID();
    if(idx != -1){
        strcpy(display_info, "Hello ");
        info_len = strlen(display_info);
        for(int i = 0; input_ID[i] != '\0'; ++i)
            display_info[info_len ++] = input[i];
        screen_display();
        return 1;
    }
    strcpy(display_info, "ID Not Found:");
    info_len = strlen(display_info);
    for(int i = 0; input_ID[i] != '\0'; ++i)
        display_info[info_len ++] = input[i];
    screen_display();
    return 0;
}

unsigned char find_ID(){
    for(int i = (start + 1) % (MAX_STORAGE + 1); i != end_idx; i = (i + 1) % (MAX_STORAGE + 1)){
        if(strcmp(database[i], input_ID) == 0)
            return i;
    }
    return -1;
}