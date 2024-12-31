#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char** split(char* str, const char d){
    char** result    = 0;
    int  count       = 0;
    char* tmp        = str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = d;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp){
        if (d == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (str + strlen(str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**) malloc(sizeof(char*) * count);

    if (result){
        size_t idx  = 0;
        char* token = strtok(str, delim);

        while (token){
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int main(){
    char str[] = "Hello this is another test.";
    char** output;
    output = split(str, ' ');
    for (int i = 0; output[i] != NULL; i++){
        printf( "%s \n", output[i]);
    }
}