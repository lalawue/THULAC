#ifndef _THULAC_LIB_H
#define _THULAC_LIB_H

#ifdef __cplusplus
extern "C" {
#endif
// default 'NULL, NULL, 16*1024*1024, 0, 0'
int thulac_init(const char* model, const char* dict, int ret_size, int t2s, int just_seg);
void thulac_deinit();

struct thulac_tag {
    char *word; // word in sentence
    char *tag; // word tag, NULL for seg_only 
    struct thulac_tag *next; // NULL for 
};

// return tag list
struct thulac_tag* thulac_seg(const char *in, int bytes_len);
void thulac_clean(struct thulac_tag *list);

#ifdef __cplusplus
}
#endif
#endif
