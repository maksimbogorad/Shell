#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "parser.h"
#include "utils.h"

int main()
{
    int c, lastchar = 0, lenbuff = 8, i = 0;
    int flags[2] = {0, 0};
    int inOutPos[3] = {0, 0, 0};
    char * buff = (char*)malloc(lenbuff);
    list * headlist = NULL;

    init(&headlist);
    while ((c = getchar()) != EOF) 
    {
        if (c != '\n') 
        {
            if ((!divide(c) && c != '\"') || (divide(c) && flags[0]))
            {
                if (i >= lenbuff - 1)
                    buff = extendbuff(buff, &lenbuff);
                buff[i++] = c;
            }
            else if (isIOsymbol(c))
                handleIOquote(lastchar, c, inOutPos, headlist, i);
            else if (c == '\"')
                flags[0] = !flags[0];
            else if (c == '&')
                flags[1]++;
            else if (c == '|')
            {
                if(i != 0)
                    headlist = addtolist(headlist, buff, i);
                headlist = addtolist(headlist, buff, -1);
                i = 0;
            }
            if (i != 0 && !flags[0] && spaceTabIOquote(c)) 
            {
                headlist = addtolist(headlist, buff, i);
                i = 0;
            }
            if(!spaceTab(c))
                lastchar = c;
        }
        else{
            processinglast(&i, buff, &headlist);
            iscorrectcmd(flags, lastchar, &headlist, inOutPos);
            reinit(&headlist);
            killbg();
        }
    }
    freememory(headlist, buff);
    return 0;
}
