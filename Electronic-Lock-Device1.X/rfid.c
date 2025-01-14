
#include "headers/internal_setting.h"
#include "headers/rfid.h"
#include "headers/uart.h"
#include "string.h"
#include "stdio.h"
#include <xc.h>
#include <pic18f4520.h>

#define MAX_STORAGE 5
#define MAX_ID_LEN 15

extern char display_info[MAX_INFO_LEN];
extern int info_len;

extern int mode;
extern unsigned char digit;

// Only store 5 ID and the max numerber of characters in each ID is 14
char database[MAX_STORAGE][MAX_ID_LEN];
char input_ID[MAX_ID_LEN];
int input_len = 0;
int end_idx = 0;

void rfid_init(){   
    end_idx = 0;
    
    for(int i = 0; i < MAX_STORAGE; ++i){
        for(int j = 0; j < MAX_ID_LEN; ++j)
            database[i][j] = '\0';
    }
    for(int i = 0; i < MAX_ID_LEN; ++i)
        input_ID[i] = '\0';
    input_len = 0;
}

unsigned char database_empty(){
    return end_idx == 0;
}

unsigned char database_full(){
    return (end_idx >= MAX_STORAGE);
}

unsigned char rfid_read(){
    int reach_end = 0;
    
    unsigned char read_char = uart_read();
    if(read_char == 0x02){
        for(int i = 0; i < MAX_ID_LEN; ++i)
            input_ID[i] = '\0';
        input_len = 0;
        return 0;
    }
    else if(read_char == 0x03){
        reach_end = 1;
    }
    else{
        input_ID[input_len ++] = read_char;
    }
    
    if(reach_end){
        // error detection by checking the checksum
        unsigned char valid_char = 1;
        int upper = char_to_hex(input_ID[10]);
        int lower = char_to_hex(input_ID[11]);
        unsigned char check_byte = 0;
        if(upper == -1 || lower == -1)
             valid_char = 0;
        else check_byte = upper * 16 + lower;
        
        unsigned char checksum = 0;
        for(int i = 0; i < 10; i += 2){
            if(!valid_char) continue;
            unsigned char byte = 0;
            upper = char_to_hex(input_ID[i]);
            lower = char_to_hex(input_ID[i + 1]);
            if(upper == -1 || lower == -1)
                 valid_char = 0;
            else checksum ^= (upper * 16 + lower);
        }
        
        if(checksum != check_byte || checksum == 0 || valid_char == 0){
            strcpy(display_info, "The checksum is invalid! Please scan again");
            info_len = (int)strlen(display_info);
            screen_display();
            for(int i = 0; i < MAX_ID_LEN; ++i)
                input_ID[i] = '\0';
            input_len = 0;
            return 2;
        }
            
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
        for(int i = 0; i < MAX_ID_LEN; ++i)
            input_ID[i] = '\0';
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
        end_idx ++;
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
        
        end_idx = end_idx - 1;
        for(int i = idx; i < end_idx; i++){
            strcpy(database[i], database[i + 1]);
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
    for(int i = 0; i < end_idx; i ++){
        if(strcmp(database[i], input_ID) == 0)
            return i;
    }
    return -1;
}

int char_to_hex(unsigned char ch){
    
    if('a' <= ch && ch <= 'f')
        return ch - 'a' + 10;
    else if('A' <= ch && ch <= 'F')
        return ch - 'A' + 10;
    else if('0' <= ch && ch <= '9')
        return ch - '0';
    return -1;
}