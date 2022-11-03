#pragma warning( disable : 4996 )

#include "server.h"
#include <string.h>

ST_accountsDB_t AccountDB[255];

uint8_t DBindx = -1;   // updated in isvalidaccount function

uint8_t comparePAN(uint8_t *arr1, uint8_t *arr2)
{

	for (int i = 0; i < PAN_LENGTH; i++)
	{
		if (arr1[i] != arr2[i])
			return FALSE;
	}
	return TRUE;

}
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	for (int i = 0; i < 255; i++)
	{
		if (comparePAN(cardData->primaryAccountNumber, AccountDB[i].primaryAccountNumber))
		{
			DBindx = i;
			return OK_ServerError;
		}	
	}
	return DECLINED_STOLEN_CARD;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	printf("index = %d\n", DBindx);
	if (AccountDB[DBindx].balance >= termData->transAmount)
	{
		return OK_ServerError;
	}
	return LOW_BALANCE;
}
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	// change Transaction sequence number which is saved in an external file to keep track of number even if the application is closed.
	FILE* fptr;
	fptr = fopen("./TransactionSequenceNumber.txt", "r");
	if (fptr==NULL) {
		return SAVING_FAILED;
	}
	uint32_t transaction_number;
	fscanf(fptr, "%d", &transaction_number);
	fclose(fptr);
	transaction_number++;      // increment transaction number
	fptr = fopen("./TransactionSequenceNumber.txt", "w");
	fprintf(fptr, "%d", transaction_number);
	fclose(fptr);
	transData->transactionSequenceNumber = transaction_number;
	/////////////////////////////////////////////////////////////

	FILE* file_ptr = fopen("./Transactions.txt", "a");
	if (!file_ptr) {
		return SAVING_FAILED;
	}
	fprintf(file_ptr, "** Transaction Receipt  **\n");
	fprintf(file_ptr, "\t-> Name: %s\n", transData->cardHolderData.cardHolderName);
	fprintf(file_ptr, "\t-> Transaction Amount: %f\n", transData->terminalData.transAmount);
	fprintf(file_ptr, "\t-> Remaining Amount: %f\n", AccountDB[DBindx].balance);

	fprintf(file_ptr, "\t-> Transaction State: ");

	if (transData->transState == APPROVED) {
		fprintf(file_ptr, "APPROVED.\n");
	}
	else if (transData->transState == DECLINED_STOLEN_CARD) {
		fprintf(file_ptr, "X ERROR: DECLINED STOLEN CARD.\n");
	}
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND) {
		fprintf(file_ptr, "X ERROR: DECLINED INSUFFECIENT FUND.\n");
	}

	fprintf(file_ptr, "\t-> Transaction Sequence Number: %d \n\n", transData->transactionSequenceNumber);
	fclose(file_ptr);
	return OK_ServerError;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return OK_ServerError;
}
void getAcountDB()
{
	FILE* ptr;
	ptr = fopen("./AccountsDB.txt", "r");
	if (!ptr) {
		printf("'AccountsDB.txt' File Corrupted/Not Found.\n");
		return;
	}
	float amount;
	int i = 0;
	for (int i = 0; i < 255; i++)
	{
		fscanf(ptr, "%f ", &amount);
		fgets(AccountDB[i].primaryAccountNumber, 21, ptr);
		AccountDB[i].balance = amount;
	}
	fclose(ptr);
}
void updateAccountDB()
{
	FILE* ptr;
	ptr = fopen("./AccountsDB.txt", "w");
	if (ptr == NULL)
	{
		printf("ERROR: CAN'T FIND AccountsDB FILE. \n");
		return;
	}
	int i;
	for (i = 0; i < 255; i++)
	{
		fprintf(ptr, "%f %s\n", AccountDB[i].balance, AccountDB[i].primaryAccountNumber);
	}
}
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData) == DECLINED_STOLEN_CARD) {
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	transData->transState = APPROVED;

	if(DBindx != -1)
		AccountDB[DBindx].balance -= transData->terminalData.transAmount;


	return APPROVED;
}