#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TODO 
// string to int for ref and age
// input checking
// scanner cohesion
// duplicate entry


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
 * char *string - string to be allocated memory
 * returns:
 * char* - copy of string with memory allocated
 */
char* memory_allocate_string(char *word)
{
	char *new_word;

	new_word = (char*)malloc((unsigned)(strlen(word) + 1 ));
	if(new_word == NULL)
		memory_error();

	strcpy(new_word, word);
	return (new_word);
}

void free_entry_line(char * arr[4])
{
	int i;
	for (i = 0; i < 4; ++i)
	{
		free(arr[i]);
	}
}

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
 */
int initNode(struct node **node, char * line[4])
{
	if(!(*node)) // If node is null i.e. does not exist
	{
		(*node) = memory_allocate_node();

		// Initialize left and right nodes to null
		(*node)->left = NULL;
		(*node)->right = NULL;

		// Initialize node variables from line input array
		(*node)->ref = memory_allocate_string( line[ 0 ] );
		(*node)->age = memory_allocate_string( line[ 1 ] );
		(*node)->firstname = memory_allocate_string( line[ 2 ] );
		(*node)->lastname = memory_allocate_string( line[ 3 ] );

		return 1; // return true if node has been created
	}
	return 0; // return false is node has not been created
}

void compareNode(struct node **node, char *line[4])
{

	int result; // stores difference between nodes for comparison

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

void insert(struct node **node, char *line[4])
{
	if(initNode(node, line)) // initializes node if null
		return; // returns if node has just been initialized
	compareNode(node, line); // compares new entry variables to tree nodes and inserts accordingly
}

void print_tree(struct node *top)
{
	if(top == NULL)
		return;
	print_tree(top->left);
	printf("\t%s\t%s\t%s, %s\n", top->ref, top->age, top->lastname, top->firstname);
	print_tree(top->right);
}


/**************
 * File Input *
 **************/

 int validateFile(FILE *file, char *filename)
 {
 	if(file == NULL)
 	{
 		fprintf(stderr, "!!!\tError unable to open file '%s'\n", filename);
 		return 0;
 	}
 	return 1;
 }

 char continueToNextCharacter(FILE * file)
 {
 	char ch;
 	while(1)
	{
		ch = fgetc(file);
		if(!isspace(ch) || !isblank(ch) || (ch == EOF))
			break;
	}
	return ch;
 }

 char* buildWord(FILE * file, char * word)
 {
 	char ch;
 	int index;
 	for(index = 1; (unsigned)index < sizeof(word); index++)
		{
			ch = fgetc(file);
			if(!isalpha(ch) && !isdigit(ch))
				break;
			word[index] = ch;
		}
		word[index] = '\0';

 	return word;
 }

 int readlines(FILE * pfile)
{
	char line[256], ref[30], age[30], firstname[30], lastname[30];

	while(fgets(line, 256, pfile) != NULL)
 	{
 		sscanf(line, "%s %s %s %s", ref, age, firstname, lastname);

 		char * entry[4] = { 
 			memory_allocate_string(ref),
 			memory_allocate_string(age),
 			memory_allocate_string(firstname),
 			memory_allocate_string(lastname)
 		};

 		insert(&root, entry);

 		free_entry_line(entry);

 	}

 	return 1;
}

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

/*int read(char *name)
{
	FILE *file;
	char word[100];
	char* line[4];
	char ch;
	int count = 0;

	file = fopen(name, "r");
	if(!validateFile(file, name))
		return 0;

	while(1)
	{
		ch = continueToNextCharacter(file); // proceed file read to next non white space character

		if(ch == EOF) // if character is End Of File finish the file reading
			break;

		// build next word
		word[0] = ch; // assign character to first letter in the word
		strcpy(word, buildWord(file, word)); // build word by reading through following characters until whitespace is reached

		// add word to line array
		line[count] = memory_allocate_string(word);
		strcpy(line[count], word);

		count++;

		if(count == 4)
		{
			insert(&root, line);
			count = 0;
			int i;
			for(i = 0; i < 4; ++i)
			{
				free(line[i]);
			}
			// while(ch != '\n' && ch != EOF)
			// {
			// 		ch = fgetc(file);
			// }
		}
	}

	fclose(file);

	return 1;
}*/

int main(int argc, char *argv[])
{
	int i;

	for(i=1; i < argc; i++)
	{
		if(!read(argv[i]))
			fprintf(stderr, "!!!\tSkipping file '%s' and continuing\n", argv[i]);
	}
	print_tree(root);
	free_tree(root);
	return (0);
}