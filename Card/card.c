#define _CRT_SECURE_NO_WARNINGS

#include "card.h"



EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	printf("Please enter your card holder name : \n");
	gets(cardData->cardHolderName);
	
	uint8_t length = strlen(cardData->cardHolderName);

	if (length > MAX_NAME_LENGTH || length < MIN_NAME_LENGTH)
		return WRONG_NAME;
	else
		return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	printf("Please enter your card expiry date in the format MM/YY : \n");
	gets(cardData->cardExpirationDate);

	// change data from string to decimal
	uint8_t month, year;
	month = (cardData->cardExpirationDate[0]-'0') * 10 + cardData->cardExpirationDate[1] - '0';
	year  = (cardData->cardExpirationDate[3]-'0') * 10 + cardData->cardExpirationDate[4] - '0';

	uint8_t length = strlen(cardData->cardExpirationDate);

	// validate input
	if (month < 1 || month>12 || cardData->cardExpirationDate[2] != '/' || (year > 99 || year < 0)) {
		return WRONG_EXP_DATE;
	}
	if (length != VALID_DATE_LENGTH)
		return WRONG_EXP_DATE;
	else
		return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Please enter your card PAN : \n");
	gets( cardData->primaryAccountNumber);

	uint8_t length = strlen(cardData->primaryAccountNumber);
	
	if (length > MAX_PAN_LENGTH || length < MIN_PAN_LENGTH)
		return WRONG_PAN;
	else
		return OK;
}