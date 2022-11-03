#ifndef CARD_H            // HEADER GUARD
#define CARD_H

#include <stdio.h>
#include <string.h>
#include "../types.h"


////VALID SIZES OF CARD DATA////
#define MAX_NAME_LENGTH		24
#define MIN_NAME_LENGTH		20
#define VALID_DATE_LENGTH	5
#define	MAX_PAN_LENGTH		19
#define	MIN_PAN_LENGTH		16
////////////////////////////////


typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;


typedef enum EN_cardError_t
{
	OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;




EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

#endif