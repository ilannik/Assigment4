#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define NUM_LETTERS ((int)27)
#define NORMILIZE 96

typedef struct node
{
    char letter;
    long unsigned int count;
    struct node **children;
} node;

node *create_node(char c)
{
    //printf("before node creation(create node)\n");
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL)
    {
        perror("Node creation has failed! Not enough memory.\n");
    }
    node **word = calloc(NUM_LETTERS, sizeof(node *));
    if (word != NULL)
    {
        new_node->count = 0;
        new_node->letter = c;
        new_node->children = word;
    }
    //printf("after node creation(create node)\n");
    return new_node;
}

typedef struct String
{
    char *char_array;
    int size;
    int capacity;
} String;

String *create_String(int capacity)
{
    String *new_String = (String *)malloc(sizeof(String));
    if (new_String == NULL)
    {
        perror("creating new string failed");
        exit(1);
    }
    new_String->char_array = (char *)calloc(capacity, sizeof(char));
    if (new_String->char_array == NULL)
    {
        perror("creating char array failed");
        exit(1);
    }
    new_String->size = 0;
    new_String->char_array[0] = '\0';
    new_String->capacity = capacity;
    return new_String;
}

void resize_String(String *string_name)
{
    string_name->capacity *= 2;
    string_name->char_array = (char *)realloc(string_name->char_array, string_name->capacity);
    if (string_name->char_array == NULL)
    {
        perror("reallocating the array has failed");
        exit(1);
    }
}

void add_char_to_String(String *string_name, char c)
{
    if (string_name->capacity == string_name->size)
    {
        resize_String(string_name);
    }
    string_name->char_array[string_name->size] = c;
    string_name->size++; // isn't increasing for some reason
    if (string_name->capacity == string_name->size)
    {
        resize_String(string_name);
    }
    string_name->char_array[string_name->size] = '\0';
}

node *frequency()
{
    //printf("before node creation(inside frequency\n");
    node *root = create_node('!');
    node *curr_node = root;
    //printf("after creating node inside frequency\n");
    char curr_char = getchar();
    curr_char = tolower(curr_char);
    //printf("enter while\n");
    while (curr_char != EOF)
    {
        // filling up the tree
        if ((curr_char >= 'a' && curr_char <= 'z') || curr_char == ' ' || curr_char == '\n')
        {
            if ('a' <= curr_char && curr_char <= 'z')
            {
                if (curr_node->children[curr_char - NORMILIZE] == NULL)
                {
                    node *node_to_add = create_node(curr_char);
                    curr_node->children[curr_char - NORMILIZE] = node_to_add;
                    curr_node = node_to_add;
                    //printf("node created (char not existed) the char was: %c\n",curr_node->letter);
                }
                else
                {
                    curr_node = (curr_node->children[curr_char - NORMILIZE]);
                    //printf("char has existed, current node advanced, the char was: %c\n",curr_node->letter);
                }
            }
            else
            {
                if (curr_node != root)
                {
                    if (curr_node->children[0] == NULL)
                    {
                        //printf("the char is: %c, and $ is null\n",curr_node->letter);
                        node *end_of_word = create_node('$');
                        curr_node->children[0] = end_of_word;
                        curr_node = end_of_word;
                        curr_node->count += 1;
                        curr_node = root;
                    }
                    else
                    {
                        // printf("the char is: %c, and $ isn't null\n",curr_node->letter);
                        curr_node = (curr_node->children[0]);
                        curr_node->count += 1;
                        //printf("the current counter is: %lu\n",curr_node->count);

                        curr_node = root;
                    }
                }
            }
        }
        curr_char = getchar();
        curr_char = tolower(curr_char);
        //printf("reached end of while, stating over with the char: %c\n",curr_char);
    }
    //printf("finished frequency\n");

    return root;
}

//TODO make sure to change reversal_traversal to match String construct
void reversal_traversal(node *root, String *word)
{
    for (int i = 26; i >= 0; i--)
    {
        node *runner = root;
        if (runner->children[i] != NULL)
        {
            if (runner->children[i]->letter == '$')
            {
                //printf("inside first if \n");
                printf("%s %lu\n", word->char_array, runner->children[i]->count);
                free(runner->children[i]->children);
                free(runner->children[i]);
            }
            else
            {
                // printf("letter is %c runner is root \n",runner->letter);
                runner = (runner->children[i]);
                //printf("inside else, after moving runner forward\n");
                //printf(" runner->letter is %c",runner->letter);
                add_char_to_String(word, runner->letter);
                reversal_traversal(runner, word);
            }
        }
    }
    word->char_array[word->size] = '\0';
    word->size--;
    free(root->children);
    free(root);
}

void traversal(node *root, String *word)
{
    for (int i = 0; i < 27; i++)
    {
        node *runner = root;
        if (runner->children[i] != NULL)
        {
            if (runner->children[i]->letter == '$')
            {
                //printf("inside first if \n");
                printf("%s %lu\n", word->char_array, runner->children[i]->count);
                free(runner->children[i]->children);
                free(runner->children[i]);
            }
            else
            {
                // printf("letter is %c runner is root \n",runner->letter);
                runner = (runner->children[i]);
                //printf("inside else, after moving runner forward\n");
                //printf(" runner->letter is %c",runner->letter);
                add_char_to_String(word, runner->letter);
                traversal(runner, word);
            }
        }
    }
    word->char_array[word->size] = '\0';
    word->size--;
    free(root->children);
    free(root);
}

int main(int argc, char *argv[])
{
    String *word = create_String(1);
    node *root = frequency();
    if (argc == 1)
    {
        traversal(root, word);
    }
    else if (argc == 2 && *argv[1] == 'r')
    {
        reversal_traversal(root, word);
    }
    free(word->char_array);
    free(word);
}
