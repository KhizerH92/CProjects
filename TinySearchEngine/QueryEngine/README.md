Description: Lab6 is the third component of the tiny search engine project called the "Query". In this we employ We rebuild a hashtable for lookup from the index file
provided and then search it for word combinations that the user provides.

Author: Khizar Hussain

File Components:

	Query.c 		Includes the main Hashtable Builder from the data file provided along with document list maker programs

	ANDORList.[hc]	These files contain the singly linked list proto types and list addition functions to help keep track of the items being queried

	hashtable.[hc]	These files contain the re-factored hashtable from crawler
					to allow us to create the word index. Basically reusing what we have for the indexer

	funcFile.[hc]	Provided files containing utility methods such as IsDir, IsFile and GetFileNamesInDir that are helper functions in our main code

	
	common.hc		Provided and edited file containing common data structures to be used in the Query Engine to keep track of the words as 
					well as the pages containing the words.



Instructions:

		To build and run the Query:
				1) Go to lab6 root folder and run 'make Query' or 'make'
				2) then you can issue the command to run the indexer manually as follows:
						./QueryEngine/Query [index.dat] [target directory of crawler files]

		To run and test the Query Automatically:
				1) run the shell script called QEBATS.sh in the Query Engine folder
				2) you can call 'make test' in the root directory
				This script:
						1) Tests the different argumments for the Query and produces a log report which is put in QueryEngine directory
						2) Builds Query and gives a report

		Testing On the word combinations provided to Query is done manually and is documented in 'TESTING' file

		Valgrind report on Query Engine is given in Spec.pdf file

		Query Engine may only be quit using ctrl-C signal for memory clearance.

Assumptions usde in the Query:
		
		1) Index file if it exists always has a 'word' as the first field of thefile and it has some data fields for the look up:
			a) For instance all the fields with not the right DOC ID , Freq Tuples will be discarded.
			b) The fields hold tuples in the order : [DOC ID, Freq]
		2) Index files use single spaces as delimiters. Any other characters besides lowercase letters for words and digits for doc IDs and word occurrences will lead to the files/lines being discarded.
		3) It is assumed that the directory of crawler files provided has the right DOC_ID-HTML correspondence and that the directory is not empty and that the first lines of the files are indeed html addresses.
		