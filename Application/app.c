#include "app.h"

void appStart(void)
{
	getAcountDB();

	ST_cardData_t userCardData;
	EN_cardError_t card_error;
	ST_terminalData_t userTerminalData;
	EN_cardError_t terminal_error;
	ST_transaction_t userTransactionData;

	card_error = getCardHolderName(&userCardData);
	while (card_error == WRONG_NAME)
	{
		printf("\nxX WRONG NAME Xx\n");
		printf("Re enter: ");
		card_error = getCardHolderName(&userCardData);
	}

	card_error = getCardExpiryDate(&userCardData);
	while (card_error == WRONG_EXP_DATE)
	{
		printf("\nxX WRONG EXPIRY DATE Xx\n");
		printf("Re enter: ");
		card_error = getCardExpiryDate(&userCardData);
	}

	card_error = getCardPAN(&userCardData);
	while (card_error == WRONG_NAME)
	{
		printf("\nxX WRONG PAN Xx\n");
		printf("Re enter: ");
		card_error = getCardPAN(&userCardData);
	}
	if (isValidCardPAN(&userCardData) == INVALID_CARD) // luhn number or not
	{
		printf("\nERROR: INVALID CARD \n");
		return;
	}

	terminal_error = getTransactionDate(&userTerminalData);
	while (terminal_error == WRONG_DATE)
	{
		printf("\nxX WRONG DATE FORMAT Xx\n");
		printf("Re enter in the form of dd/mm/yyyy: ");
		terminal_error = getTransactionDate(&userTerminalData);
	}
	if (isCardExpired(userCardData, userTerminalData) == EXPIRED_CARD)
	{
		printf("\nxX DECLINED EXPIRED CARD Xx\n");
		return;
	}

	terminal_error = getTransactionAmount(&userTerminalData);
	while (terminal_error == INVALID_AMOUNT)
	{
		printf("\nxX INVALID AMOUNT Xx\n");
		printf("Re enter : ");
		terminal_error = getTransactionAmount(&userTerminalData);
	}

	terminal_error = setMaxAmount(&userTerminalData);
	while (terminal_error == INVALID_AMOUNT)
	{
		printf("\nxX INVALID AMOUNT Xx\n");
		printf("Re enter : ");
		terminal_error = setMaxAmount(&userTerminalData);
	}
	if (isBelowMaxAmount(&userTerminalData) == EXCEED_MAX_AMOUNT)
	{
		printf("\n xX DECLINED AMOUNT EXCEEDING LIMIT Xx");
		return;
	}
	userTransactionData.cardHolderData = userCardData;
	userTransactionData.terminalData = userTerminalData;
	EN_transState_t transactionState;
	transactionState = recieveTransactionData(&userTransactionData);
	if (transactionState == DECLINED_STOLEN_CARD)
	{
		printf("\n xX ALERT: THIS CARD IS STOLEN Xx \n");
		return;
	}
	else if (transactionState == DECLINED_INSUFFECIENT_FUND)
	{
		printf("\n INSUFFICIENT FUND AVAILABLE \n");
		return;
	}
	else if (transactionState == APPROVED)
	{
		if (saveTransaction(&userTransactionData) == SAVING_FAILED) {
			printf("Could not save the transaction, Soryy for the inconvenience");
		}
		printf("\nTransaction APPROVED\n");
		printf("UPDATING Database...\n");
		updateAccountDB();
		printf("Transaction COMPLETED\n\n\n\n");
	}

	return;
}
