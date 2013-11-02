
Formal Methods and Programming


*Assignment 1*

The program reads in records of people from files given on the command line and stores them in a tree structure in ascending age order. The tree is then printed to the console.

The program, after being compiled, is called from the command line in the following manner:

./program file1 file2 file3


* Error Checking and Restrictions *

The records of people are read in line by line and must contain only one record per line in the following format:

REF AGE FIRSTNAME LASTNAME

For example:

001 9 Jim Jones
002 17 Mary Poppins
003 16 John Jenkins

Only the first four elements of the line are read and so it is not valid to include middle names in the records. The program will cut off any trailing names after the fourth element most likely resulting in the middle name being read as the last name.

Checks are made for each element in the line to make sure they are valid entries. The reference number and age must be between 0 and 3 digits. The First and last names must be between 0 and 100 alphabetical characters. Entries that do not pass these checks are rejected and not added to the system. The user is warned via console output.

New entries are allowed to have names and ages equal to existing entries but only one of each reference number is allowed. An attempt to add an entry with a conflicting reference number will result in it not being added to the tree. The user is warned by console output.

Entries to the tree are ordered by age in ascending order. If ages are the same they are order by last name and then first name. If they match on all accounts they are simply placed next to each other.

Files specified on the command line are read in one by one. If the file is not valid it will be skipped and the program will continue to the next file. The user is warned via console output.


* Testing *

Testing was done using a combination of the Google Test framework and simple printing to console. The final test output of gtest can be seen in the included test_output.txt file.

Valgrind was used to check that there were no memory leaks in the program.

The program was tested on the following platforms and compilers:

linux - clang and gcc
windows - gcc