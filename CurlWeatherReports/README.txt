Documentation and how to run:
	So to accomplish the tasks we need to do with the weather program
	I have used an xml file parser called EZXML. This meant that the 
	Ezxml parser library needed to be downloaded and integrated into the 
	whole weather program directory. 
	TO do that:
	1) You download ezxml-0.8.6.tar.gz. 
	2) Unzip it in your weather directory
	3) And then run the make command on the files
	4) then you compile your weather program along with object file ezxml.o created by the make operation
	5) Compilation command:
	mygcc -o weather weather.c -lcurl ezxml.o



Assumptions made:
	1) The remote server that we are fetching xml files from, either returns
		html or xml files and if it does return xml files they are formatted to have all the required information that we are looking for, headed the same way as everything 