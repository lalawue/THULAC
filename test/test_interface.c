
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

    void *lac = thulac_init(NULL, NULL, 0, 0, 0);

    if (lac)
    {
        struct thulac_tag *list = thulac_seg(lac, argv[1]);
        struct thulac_tag *tag = list;
        while (tag) {
            printf("%s %s\n", tag->word, tag->tag);
            tag = tag->next;
        }
        thulac_clean(lac, list);
        thulac_deinit(lac);
    }

    return 0;
}
