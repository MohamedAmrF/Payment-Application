#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <string.h>
#include "../Card/card.h"

#define DATE_LENGTH	10

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;


typedef enum EN_terminalError_t
{
	OK_terminalError, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;


// prototypes to check for date format
uint8_t to_decimal(uint8_t c);
uint8_t check_format(char str[]);

// Prototypes for luhn's algorithm
uint16_t sum_digits(uint16_t num);
uint16_t double_num(uint16_t num);
uint8_t  validate_luhn(char str[]);
////////////////////////////////////

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);


#endif