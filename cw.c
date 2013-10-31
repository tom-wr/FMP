#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TODO 
// string to int for ref and age
// input checking
// scanner cohesion


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
	fprintf(stderr, "A memory error has occured\n");
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
char* memory_allocate_string(char *string)
{
	char *new_string;

	new_string = (char*)malloc((unsigned)(strlen(string) + 1 ));
	if(new_string == NULL)
		memory_error();

	strcpy(new_string, string);
	return (new_string);
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
		/*(*node) = (struct node*)malloc(sizeof(struct node)); // create memory for node
		if((!*node))
			memory_error(); // throw error if node has not initialized properly*/

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

void read(char *name)
{
	char word[100];
	int index;
	int count = 0;
	int ch;
	char* line[4];
	FILE *file;

	file = fopen(name, "r");
	if(file == NULL)
	{
		fprintf(stderr, "Error: Unable to open %s\n", name);
		exit(8);
	}
	while(1)
	{

		while(1)
		{
			ch = fgetc(file);
			if(!isspace(ch) || !isblank(ch) || (ch == EOF))
				break;
		}

		if(ch == EOF)
			break;

		word[0] = ch;
		for(index = 1; (unsigned)index < sizeof(word); index++)
		{
			ch = fgetc(file);
			if(!isalpha(ch) && !isdigit(ch))
				break;
			word[index] = ch;
		}
		word[index] = '\0';

		line[count] = memory_allocate_string(word);//(char*)malloc(strlen(word));
		strcpy(line[count], word);
		count++;

		if(count == 4)
		{
			insert(&root, line);
			count = 0;
		}
	}
	fclose(file);
}

int main(int argc, char *argv[])
{
	int i;

	for(i=1; i < argc; i++)
	{
		read(argv[i]);
	}
	print_tree(root);
	return (0);
}