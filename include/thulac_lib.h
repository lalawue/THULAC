#ifndef _THULAC_SHARED_LIB_H
#define _THULAC_SHARED_LIB_H

#ifdef __cplusplus
extern "C" {
#endif
// default 'NULL, NULL, 16*1024*1024, 0, 0'
void* thulac_init(const char * model_path, const char* user_path, int just_seg, int t2s, int ufilter);
void thulac_deinit(void *);

struct thulac_tag {
    char *word; // word in sentence
    char *tag; // word tag, NULL for seg_only 
    struct thulac_tag *next; // NULL
};

// return tag list
struct thulac_tag* thulac_seg(void*, const char *in);
void thulac_clean(void*, struct thulac_tag *list);

#ifdef __cplusplus
}
#endif
#endif
