/*	file.c	one-line description

	Copyright 2013 (if any)

	License (if any)

	Project name:
	Component name:

	This file contains ...
	
	Primary Author:	
	Date Created:	

	Special considerations:  
		(e.g., special compilation options, platform limitations, etc.) 
	
======================================================================*/
// do not remove any of these sections, even if they are empty
//
// ---------------- Open Issues 

// ---------------- System includes e.g., <stdio.h>

// ---------------- Local includes  e.g., "file.h"

// ---------------- Constant definitions 

// ---------------- Macro definitions

// ---------------- Structures/Types 

// ---------------- Private variables 

// ---------------- Private prototypes 

/*====================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

float windChill( float temp, float V ) {
	float specialFloat= powf(V,0.16);
	return (35.74 + 0.6215*temp - 35.75*(specialFloat) + 0.4275*temp*(specialFloat));
}

int main( int argc, char *argv[] ) {
	/* Check for invalid arguments */
	
	for(int c=1; c<argc ; c++){
		char str[50] = {0};
		sprintf(str,"%s", argv[c]);
		for(int j=0; j<strlen(str); j++){
			if(!isdigit(str[j]) && !(str[j]==46) && !(str[j]==45)){
				printf("Temperature and Wind Velocity should be nummbers");
				return(0);
			}
		}
	}




	int x = argc;
	/* printf("%d", x ); */
	printf("Temp	Wind	Chill\n");
	printf("----	----	-----\n");
	
	if(x>3){
		printf("only pass two arguments at max");
		return(0);
	}

	switch(x) {
		case 1 : /* print the whole table */
			for(int j=-1;j<=4;j++){


				for(int i=1;i<=3;i++){
					float t = 10*j;
					float V = 5*i;
					printf("%.1f\t%.1f\t%.1f\n", t , V , windChill(t,V) );
				}

				printf(" \n");
			}
			break;
	
		case 2 : ;/* print the different winds for one temp only */
			float onlyArg = atof(argv[1]);
		
			if(onlyArg<50){
				for(int i=1;i<=3;i++){
					float t = onlyArg;
					float V = 5*i;
					printf("%.1f\t%.1f\t%.1f\n", t , V , windChill(t,V) );
				}

			}
			else{
				printf("Enter a temperature less than 50");
			}
			break;
		case 3 : ;/* print the whole table */
			float t = atof(argv[1]);
			float V = atof(argv[2]);
		
			if(t<50 && V>0.5){
				printf("%.1f\t%.1f\t%.1f", t , V , windChill(t,V) );
		
			}
		
			else{
				printf("Temperature should be less than 50 and Wind Veloity greater than 0.5 MPH");
			}
			break;
		
		default : 
			printf("caught nothing");
	}
	return(0);
}