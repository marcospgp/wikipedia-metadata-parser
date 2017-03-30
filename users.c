#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#include "users.h"

TAD_istruct onPageUsers(TAD_istruct qs, long id, char *username) {

	qs = onUserContribution(qs, id, username);

	return qs;
}
