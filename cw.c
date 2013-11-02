#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**************
 * Structures *
 **************/

struct node
{
	char* age;
	char* ref;
	char* firstname;
	char* lastname; 

	struct node *left, *right;
};

static struct node *root = NULL; // initialize root node for the tree

/**********
 * Output *
 **********/

/*
 * prints error message to console
 *
 * params:
 * char * error_string - string to be included in the error message
 * returns:
 * void
 */
void print_error(char * error_string)
{
	fprintf(stderr, "!!!\t\n!!!\tERROR: %s\n!!!\n", error_string);
}

/*
 * prints error information for an array of input
 *
 * params:
 * char * entry[4] - array of input elements to be printed
 * returns:
 * void
 */
void print_entry_error(char * entry[4])
{
	printf("!!!\t%s\t%s\t%s\t%s\n!!!\n", entry[0], entry[1], entry[2], entry[3]);
}

/*
 * recursively prints the information in the tree
 *
 * params:
 * struct node * tree_node - current node to be printed
 * returns:
 * void
 */
void print_tree(struct node * tree_node)
{
	if(tree_node == NULL)
		return;
	print_tree(tree_node->left);
	printf("\t%s\t%s\t%s, %s\n", tree_node->ref, tree_node->age, tree_node->lastname, tree_node->firstname);
	print_tree(tree_node->right);
}

/*
 * print output of the program
 *
 * params:
 * struct node * tree_node - tree node to start printing from
 * returns:
 * void
 */
void print_output(struct node * tree_node)
{
	printf("\t-------------------------------\n");
	printf("\t%s\t%s\t%s\t\n", "Ref", "Age", "Name");
	printf("\t-------------------------------\n");
	print_tree(tree_node);
	printf("\t-------------------------------\n\n");
}

/*******************
 * Memory Handling *
 *******************/

/*
 * prints message to inform of memory error to the console
 * and exits the program.
 */
void memory_error(void)
{
	fprintf(stderr, "!!!\tA memory error has occurred\n");
	exit(8);
}

/*
 * allocates memory for a string
 *
 * params:
 * char* word - string to be allocated memory
 * returns:
 * char* - copy of string with memory allocated
 */
char* memory_allocate_string(char * word)
{
	char *new_word;

	new_word = (char*)malloc((unsigned)(strlen(word) + 1 ));
	if(new_word == NULL)
		memory_error();

	strcpy(new_word, word);
	return (new_word);
}

/*
 * allocates memory for a node struct
 *
 * returns:
 * struct node* - pointer to newly created memory for node struct
 */

struct node* memory_allocate_node()
{
	struct node *new_node;

	new_node = (struct node*)malloc(sizeof(struct node));
	if(new_node == NULL)
		memory_error();

	return new_node;
}

/*
 * free_entry_line
 *
 * frees memory for each element in an entry line
 *
 * params:
 * char* arr[4] - pointer to array of four strings
 * returns:
 * void
 */
void free_entry_line(char * arr[4])
{
	int i;
	for (i = 0; i < 4; ++i)
	{
		free(arr[i]);
	}
}

/*
 * frees memory used by tree recursively
 *
 * params:
 * struct node * n - node struct to be freed
 * returns:
 * void
 */
void free_tree(struct node * n)
{
	if(n == NULL)
		return;
	free_tree(n->left);
	free_tree(n->right);
	free(n->firstname);
	free(n->lastname);
	free(n->age);
	free(n->ref);
	free(n);
}


/***************
 * Binary Tree *
 ***************/

// Insert prototype
void insert(struct node**, char*[]);

/*
 * checks if node struct is null
 *
 * if so, the node is initialized with variables from the supplied file line array and returns true
 * if not, returns false
 *
 * params:
 * struct node ** node - node to be checked (and initialized if null)
 * char * entry_line - line of input elements to be used as variables in the node
 * returns:
 * int  - 1 if a node has been initialised
 * 		- 0 if nothing has been created
 */
int initNode(struct node ** node, char * entry_line[4])
{
	if(!(*node)) // If node is null i.e. does not exist
	{
		(*node) = memory_allocate_node();

		// Initialize left and right nodes to null
		(*node)->left = NULL;
		(*node)->right = NULL;

		// Initialize node variables from line input array
		(*node)->ref = memory_allocate_string( entry_line[ 0 ] );
		(*node)->age = memory_allocate_string( entry_line[ 1 ] );
		(*node)->firstname = memory_allocate_string( entry_line[ 2 ] );
		(*node)->lastname = memory_allocate_string( entry_line[ 3 ] );

		return 1; // return true if node has been created
	}
	return 0; // return false is node has not been created
}


/*
 * runs comparison checks on the entry line array of elements and 
 * performs tree insert on applicable child of the node
 *
 * params:
 * struct node ** node - current node to checked against
 * char * line[4] - input line to be checked
 * returns:
 * void
 */
void compareNode(struct node ** node, char *line[4])
{

	int result; // stores difference between nodes for comparison

	// check to see if ref number is a duplicate
	if(strcmp((*node)->ref, line[0]) == 0)
	{
		print_error("Duplicate ref number not allowed. The following entry was not added:");
		print_entry_error(line);
		return;
	}

	// compare by age ---> last name ---> first name
	result = atoi((*node)->age)- atoi(line[1]); //performs comparison between node age variables
	if(result == 0)
		result = strcmp((*node)->lastname, line[3]); // comparison between last names
	if(result == 0)
		result = strcmp((*node)->firstname, line[2]); // comparison between first names


	// recursive insert on left or right node depending on result
	if(result < 0)
		insert(&(*node)->right, line); // if comparison is smaller perform insert on right node
	else
		insert(&(*node)->left, line); // if the same or larger perform insert on left node
}


/*
 * inserts an array of input elements into the tree.
 * a node is created if the current node being looked at is null
 * else the function contiues through the tree looking for the next empty spot
 *
 * params:
 * struct node ** tree_node - current node being evaluated
 * char * line[4] - input line to potentially be initialized into a new node
 * returns:
 * 
 */
void insert(struct node ** tree_node, char *line[4])
{
	if(initNode(tree_node, line)) // initializes node if null
		return; // returns if node has just been initialized
	compareNode(tree_node, line); // compares new entry variables to tree nodes and inserts accordingly
}

/**************
 * File Input *
 **************/

/*
 * checks to see if file was opened and does not equal null
 *
 * params:
 * FILE * file - file to be checked
 * char * filename - file name used to display error messafe
 * returns:
 * int - 0 if file is null, 1 if otherwise
 */
 int validateFile(FILE *file, char *filename)
 {
 	if(file == NULL)
 	{
 		fprintf(stderr, "!!!\tERROR: Unable to open file '%s'\n", filename);
 		return 0;
 	}
 	return 1;
 }


/*
 * checks the length of the string is larger than 0 and smaller than
 * given maximum length
 *
 * params:
 * char * str - string to be checked
 * int max_length - maximum length of the string
 * returns:
 * int - 0 if 
 */
 int validate_string_length(char * str, int max_length)
 {
 	int len = (int)strlen(str);
 	if( len > max_length || len == 0)
 		return 0;
 	return 1;
 }


/*
 * checks that a string only contains digits
 *
 * params:
 * char * str - string to be checked
 * returns:
 * int - 1 if contains only digits; 0 if contains characters other than digits 		
 */
 int validate_digit_string(char * str)
 {
 	int len = strlen(str);
 	int i;
 	for(i = 0; i < len; i++)
 	{
 		if(!isdigit(str[i]))
 		{
 			return 0;
 		}
 	}
 	return 1;
 }


/*
 * checks that a string only contains digits
 *
 * params:
 * char * str - string to be checked
 * returns:
 * int - 1 if contains only digits; 0 if contains characters other than digits 		
 */
 int validate_alpha_string(char * str)
 {
 	int len = strlen(str);
 	int i;
 	for(i = 0; i < len; i++)
 	{
 		if(!isalpha(str[i]))
 			return 0;
 	}
 	return 1;
 }

/*
 * checks to see if string contains only characters of a type
 * either all digits ('d') or alphabetical ('a')
 *
 * params:
 * char * str - string to be checked
 * char type - type of check to be performed; 'd' digits; 'a' alphabetical
 * returns:
 * int - 0 if validation checks fail; 1 if checks are passed 
 */
 int validate_string_type(char * str, char type)
 {
 	switch(type)
 	{
 		case 'd':
 			if(!validate_digit_string(str))
 				return 0;
 			break;
 		case 'a':
 			if(!validate_alpha_string(str))
 				return 0;
 			break;
 		default:
 			printf("Incompatible type parameter");
 			return 0;
 	}
 	return 1;
 }

/*
 * checks string is of the right length and only contains characters of expected type
 *
 * params:
 * char * str - string to be checked
 * int max_length - maximum length the string can be
 * char expected_type - expected type for characters of string to contain
 * returns:
 * int - 0 if check failed; 1 if check passed
 */
 int validate_entry_string(char * str, int max_length, char expected_type)
 {
	if( (!validate_string_length(str, max_length)) || (!validate_string_type(str, expected_type)) )
		return 0;
	return 1;

 }

/*
 * checks entry line is in expected format:
 * 		REF AGE FIRSTNAME LASTNAME
 * by validating each element of the array with specific parameters
 *
 * params:
 * char * entry[4] - input array of strings to be checked
 * returns:
 * int - 1 if all checks pass; 0 if at least one fails.
 */
 int validate_entry_line(char * entry[4])
 {

 	if(!validate_entry_string(entry[0], 3, 'd'))
 	{
 		print_error("Ref number must be 0 to 3 digits long and consist of only digits:");
 		print_entry_error(entry);
	 	return 0;
 	}

 	if(!validate_entry_string(entry[1], 3, 'd'))
 	{
 		print_error("Age must be 0 to 3 digits long and consist of only digits:");
 		print_entry_error(entry);
 		return 0;
 	}
 	
 	if(!validate_entry_string(entry[2], 100, 'a'))
 	{
 		print_error("First name must be 0 to 100 letters long and contain only letters:");
 		print_entry_error(entry);
 		return 0;
 	}
 	
 	if(!validate_entry_string(entry[3], 100, 'a'))
 	{
 		print_error("Last name must be 0 to 100 letters long and contain only letters:");
 		print_entry_error(entry);
 		return 0;
 	}
 	
 	else return 1;
 }

/*
 * read lines from input file and validates them before entering them into tree 
 *
 * params:
 * FILE * pfile - file to be read from
 * returns:
 * void
 */
void readlines(FILE * pfile)
{
	char line[256];
	while(fgets(line, 256, pfile) != NULL)
 	{
 		char ref[256], age[256], firstname[256], lastname[256];
 		sscanf(line, "%s %s %s %s", ref, age, firstname, lastname);

 		char * entry[4] = { 
 			memory_allocate_string(ref),
 			memory_allocate_string(age),
 			memory_allocate_string(firstname),
 			memory_allocate_string(lastname)
 		};

 		if(!validate_entry_line(entry))
 		{
 			free_entry_line(entry);
 			continue; // skip insertion if entry line is not valid
 		}

 		insert(&root, entry);

 		free_entry_line(entry);

 		// reset memory for entry line 
 		memset(ref, 0, 256);
 		memset(age, 0, 256);
 		memset(firstname, 0, 256);
 		memset(lastname, 0, 256);
 	}
}

/*
 * open and read lines from file
 *
 * params:
 * char * filename - name of file to be opened
 * returns:
 * int - 0 if file was not valid; 1 if operation completes
 */
 int read(char *filename)
 {
 	FILE * pfile;
 	
 	pfile = fopen(filename, "r");
 	if(!validateFile(pfile, filename))
		return 0;
	
	readlines(pfile);
 	
 	fclose(pfile);
 	return 1;
 }

int main(int argc, char *argv[])
{
	int i;
	printf("\n");

	for(i=1; i < argc; i++)
	{
		if(!read(argv[i])) // if file is not valid print error
			fprintf(stderr, "!!!\tSkipping file '%s' and continuing\n!!!\n", argv[i]);
	}
	print_output(root);
	free_tree(root);
	return (0);
}