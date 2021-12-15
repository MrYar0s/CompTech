#include "rwlock.h"
#include <stdio.h>

rwlock_t account_lock;

float balance_to = 100.0;
float balance_from = 100.0;

float get_balance(float balance)
{
	rwlock_readlock(&account_lock);
	printf("balance = %.3f\n", balance);
	rwlock_readunlock(&account_lock);
}

void transfer_balance(float amount)
{
	rwlock_writelock(&account_lock);
	balance_to = balance_to + amount;
	balance_from = balance_from - amount;
	rwlock_writeunlock(&account_lock);
}

int main()
{
	rwlock_init(&account_lock);

	get_balance(balance_from);
	get_balance(balance_to);

	transfer_balance(15.244);

	get_balance(balance_from);
	get_balance(balance_to);

	rwlock_destroy(&account_lock);
}
