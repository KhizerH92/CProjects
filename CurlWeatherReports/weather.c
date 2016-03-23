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
#include <curl/curl.h>
#include "mycurl.c"
#include <string.h>
#include <ctype.h>		
#include "ezxml.h"
#include "mycurl.h"


/* ... */

int main(int argc, char **argv)
{
    /* local variables */
	
	/* check args for every type of anomally that could occur */

	if(argc!=2){
		printf("give one location to find the weather report of");
		exit(0);
	}

	
	if (strlen(argv[1])!=4) {
		printf("the location code is supposed to be four letters\n");
		exit(0);
	}

	//store the argv into a string
	char str[50] = {0};
	sprintf(str,"%s", argv[1]);

	for(int c; c < strlen(argv[1]) ; c++){
		if(!isupper(str[c])){
			printf("Code should be letters and in allcaps"); //This is a cool little trick to check
                                                             //both that the code is letters and all caps
            exit(0);
		}
	}

    /* curl stuff */
    CURL *curl;
    CURLcode res; //result of the whole process
    struct curlResponse page;
    /* Here is the format string for the URL we will request */
    char *APIurl  = "http://w1.weather.gov/xml/current_obs/%s.xml";

    //Stuff I am adding is going here:
    char newUrl[60]={0};
    snprintf(newUrl, 1000 , APIurl, argv[1]);
   
    //printf("%s\n", newUrl );
    //printf("%s\n", argv[1] );

    /* allocate and intiialize the output area */
    init_curlResponse(&page);

    /* create the URL from the format string */
    /* ... */

    /* initialize curl */
    curl = curl_easy_init();

    if(curl) {

        /* set the URL we want to visit */
        curl_easy_setopt(curl, CURLOPT_URL, newUrl );

		/* set the URL we want to visit */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, newUrl );


        /* set the function curl should call with the result */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

        /* set the buffer into which curl should place the data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &page);

        //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0); //To see what is happening really

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* make sure it worked */

        //printf("%d\n",res);

        if (res==0){
        	
        	//checl that data pointer starts of with xml assignment
        	if(page.ptr[0]=='<' && page.ptr[1]=='?' && page.ptr[2]=='x'){
	        	FILE *fp;
    	    	fp = fopen("Weather_Conditions.xml","w");
                //printf("%c\n", page.ptr);
        		fprintf(fp, "%s", page.ptr );
                fclose(fp);
        		

	        	ezxml_t f1 = ezxml_parse_file("Weather_Conditions.xml");

    	    	printf("Please find the Weather conditions of your location below\n");


        		printf("%s\n", ezxml_get(f1,"credit", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"location", -1)->txt);
        		printf("%s\n" , ezxml_get(f1,"station_id", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"observation_time", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"weather", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"temperature_string", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"relative_humidity", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"wind_string", -1)->txt);
        		printf("%s\n", ezxml_get(f1,"visibility_mi", -1)->txt);

        		ezxml_free(f1); //not sure about this!
                
        	}
        	else{
        		printf("It gave an html page. Please try another code");
        	}
        }

        else{
        	printf("Your request was denied by the server. Please try again");
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    
    return 0 ;
}
