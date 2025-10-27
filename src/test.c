#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct s_struct{
    *value;
    t_struct *next;

}   t_struct;

typedef enum e_enum{
    word,       //0
    pipe,       //1
    append,     //2
    heredoc,    //3
    file,       //4
    operator,   //5
    redirection,//6
}   type_of_token;

void    lexing(char *prompt)
{
    if (!prompt)
        return ;
    int i = 0;
    while (condition)
    {
        /* code */
    }
    
}