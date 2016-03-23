/*
 * Author: Ella Dzenitis
 *
 * Created: Tuesday 8 March 2016
 *
 * Script Name: ptest.c
 *
 * Program Description: Tests the p function from avatardaemon2.0.c.
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>
#include <stdio.h>

// ---------------- Local includes
#include "../src/avatardaemon2.0.c"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */


// test p function from avatardaemon2.0.c
int main(int argc, char* argv[])
{
	// 2^5
	printf("%d\n", p(2,5));

	// 5^5
	printf("%d\n", p(5,5));

	// // 3^0
	printf("%d\n", p(3, 0));
	return 0;
}