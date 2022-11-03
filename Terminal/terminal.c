#define _CRT_SECURE_NO_WARNINGS
#include "terminal.h"

uint16_t sum_digits(uint16_t num)
{
	uint16_t sum = 0;
	while (num) {
		sum += num % 10;
		num /= 10;
	}
	return sum;
}
uint16_t double_num(uint16_t num)
{
	return num * 2;
}

uint8_t  validate_luhn(char str[])
{
	uint8_t length = strlen(str);
	uint16_t luhn = 0;
	uint8_t flag = 0;
	for (int i = length - 1; i >= 0; i--)
	{
		if (flag)
			luhn += sum_digits(double_num(str[i] - '0'));  // every second digit from the end
		else
			luhn += sum_digits(str[i] - '0');			//every first digit from the end
		flag = !flag;
	}
	if (luhn % 10 == 0)
		return TRUE;
	else
		return FALSE;
}

uint8_t to_decimal(uint8_t c)
{
	return c - '0';
}

uint8_t check_format(char str[])
{
	uint8_t length = strlen(str);
	uint32_t day, month, year;
	if (length == DATE_LENGTH)
	{
		//dd/mm/yyy
		// convert date from string to decimal
		day = to_decimal(str[0]) * 10 + to_decimal(str[1]);
		month = to_decimal(str[3]) * 10 + to_decimal(str[4]);
		year = to_decimal(str[6]) * 1000 + to_decimal(str[7]) * 100 + to_decimal(str[8]) * 10 + to_decimal(str[9]);

		if ((day>0 && day<=31) && (month>0 && month<=12) && (year<=2022))
		{
			return TRUE;
		}
	}
	return FALSE;	
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("please enter the transaction date : \n");
	gets(termData->transactionDate);

	if (check_format(termData->transactionDate))    // CHECKS FORMAT && SIZE
		return OK_terminalError;
	else
		return WRONG_DATE;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	uint8_t expiry_month, expiry_year, transaction_month, transaction_year;

	// convert expiry date to decimal
	expiry_month = (cardData.cardExpirationDate[0] - '0') * 10 + cardData.cardExpirationDate[1] - '0';
	expiry_year  = (cardData.cardExpirationDate[3] - '0') * 10 + cardData.cardExpirationDate[4] - '0' + 2000;

	// convert transaction date to decimal
	transaction_month = to_decimal(termData.transactionDate[3]) * 10 + to_decimal(termData.transactionDate[4]);
	transaction_year = to_decimal(termData.transactionDate[6]) * 1000 + to_decimal(termData.transactionDate[7]) * 100 + to_decimal(termData.transactionDate[8]) * 10 + to_decimal(termData.transactionDate[9]);

	// validate transaction date
	if (transaction_year < expiry_year)
		return OK_terminalError;
	else if (transaction_year == expiry_year && transaction_month <= expiry_month)
		return OK_terminalError;
	else
		return EXPIRED_CARD;
}


EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	if (validate_luhn(cardData->primaryAccountNumber))
		return OK_terminalError;
	return INVALID_CARD;
}



EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("please enter the transaction amount : ");

	scanf_s("%f", &termData->transAmount);

	if (termData->transAmount <= 0)
		return INVALID_AMOUNT;
	else
	{
		return OK_terminalError;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount <= termData->maxTransAmount)
		return OK_terminalError;
	else
		return EXCEED_MAX_AMOUNT;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	printf("Enter MAXIMUM amount for a transaction: ");
	scanf("%f", &termData->maxTransAmount);

	if (termData->maxTransAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	return OK_terminalError;
}