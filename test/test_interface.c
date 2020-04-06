
#include "thulac_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%s 'SENTENCE'\n", argv[0]);
        return 0;
    }

    void *ctx = thulac_init(NULL, NULL, 0, 0, 0);
    if (ctx)
    {
        int count = thulac_seg(ctx, argv[1]);
        for (int i = 0; i < count; i++)
        {
            thulac_word_tag_t *wt = thulac_fetch(ctx, i);
            printf("%s %s\n", wt->word, wt->tag);
        }
        thulac_deinit(ctx);
    }
    else
    {
        printf("failed to init\n");
    }

    return 0;
}
