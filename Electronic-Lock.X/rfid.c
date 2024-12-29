
#include "headers/internal_setting.h"
#include "headers/rfid.h"
#include "headers/uart.h"
#include "string.h"
#include <xc.h>
#include <pic18f4520.h>

#define MAX_STORAGE 9
#define MAX_ID_LEN 15

extern char display_info[MAX_INFO_LEN];
extern int info_len;

extern int mode;
extern unsigned char digit;

// Only store 8 ID and the max numerber of characters in each ID is 14
char database[MAX_STORAGE][MAX_ID_LEN];
char input_ID[MAX_ID_LEN];
int input_len = 0;
int start_idx = 0;
int end_idx = 1;

void rfid_init(){
    start_idx = 0;
    end_idx = 1;
    for(int i = 0; i < MAX_STORAGE; ++i){
        database[i][0] = '\0';
    }
    uart_init();
}

unsigned char database_empty(){
    return (start_idx + 1) % (MAX_STORAGE) == end_idx;
}

unsigned char database_full(){
    return (start_idx == end_idx);
}

unsigned char rfid_read(){
    int reach_end = 0;
    
    unsigned char read_char = uart_read();
    if(read_char == 0x02)
        return 0;
    else if(read_char == 0x03){
        reach_end = 1;
    }
    else{
        input_ID[input_len ++] = read_char;
    }
    
    if(reach_end){
        // TODO
        // add error detection by checking the checksum
        
        if(input_len >= MAX_ID_LEN)
            input_ID[MAX_ID_LEN - 1] = '\0';
        input_len = (int)strlen(input_ID);
        
        unsigned char ret;
        if(digit == 0b11){      // Registration Mode
            ret = insert_ID();
        }
        else if(digit == 0b01){ // Deregistration Mode
            ret = remove_ID();
        }
        else if(digit == 0b10){ // Check Mode(Ready)
            ret = check_ID();
        }
        input_ID[0] = '\0';
        input_len = 0;
        
        // 1:success & 2: fail
        return ret;
    }
    return 0;
}

unsigned char insert_ID(){
    
    if(database_full()){
        strcpy(display_info, "The dataset is full");
        info_len = (int)strlen(display_info);
        screen_display();
        return 2;
    }
    
    int idx = find_ID();
    unsigned char ret_status = 0;
    if(idx != -1){
        strcpy(display_info, "ID have been in database:");
        ret_status = 2;
    }
    else{
        strcpy(display_info, "New User ID:");
        ret_status = 1;
        strcpy(database[end_idx], input_ID);
        end_idx = (end_idx + 1) % MAX_STORAGE;
    }
    info_len = (int)strlen(display_info);
    for(int i = 0; input_ID[i] != '\0'; ++i)
        display_info[info_len ++] = input_ID[i];
    screen_display();
    return ret_status;
}

unsigned char remove_ID(){
    if(database_empty()){
        strcpy(display_info, "The dataset is empty");
        info_len = (int)strlen(display_info);
        screen_display();
        return 2;
    }
    
    int idx = find_ID();
    unsigned char ret_status = 0;
    if(idx == -1){
        strcpy(display_info, "ID Not Found:");
        ret_status = 2;
    }
    else{
        strcpy(display_info, "Remove ID:");
        ret_status = 1;
        
        // end_idx = end_idx - 1
        end_idx = (end_idx + MAX_STORAGE - 1) % MAX_STORAGE;
        int next_i;
        for(int i = idx; i != end_idx; i = next_i){
            next_i = (i + 1) % MAX_STORAGE;
            strcpy(database[i], database[next_i]);
        }
    }
    
    info_len = (int)strlen(display_info);
    for(int i = 0; input_ID[i] != '\0'; ++i)
        display_info[info_len ++] = input_ID[i];
    screen_display();
    return ret_status;
}

unsigned char check_ID(){
    int idx = find_ID();
    if(idx != -1){
        strcpy(display_info, "Hello ");
        info_len = (int)strlen(display_info);
        for(int i = 0; input_ID[i] != '\0'; ++i)
            display_info[info_len ++] = input_ID[i];
        screen_display();
        return 1;
    }
    strcpy(display_info, "ID Not Found:");
    info_len = (int)strlen(display_info);
    for(int i = 0; input_ID[i] != '\0'; ++i)
        display_info[info_len ++] = input_ID[i];
    screen_display();
    return 2;
}

int find_ID(){
    for(int i = (start_idx + 1) % MAX_STORAGE; i != end_idx; i = (i + 1) % MAX_STORAGE){
        if(strcmp(database[i], input_ID) == 0)
            return i;
    }
    return -1;
}