#ifndef _THULAC_SHARED_LIB_H
#define _THULAC_SHARED_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *word;
    const char *tag;
} thulac_word_tag_t;

// default 'NULL, NULL, 0, 0, 0'
void* thulac_init(const char * model_path, const char* user_path, int just_seg, int t2s, int ufilter);
void thulac_deinit(void *ctx);

// return seg count
int thulac_seg(void *ctx, const char *in);

// fetch tag
thulac_word_tag_t* thulac_fetch(void *ctx, int index);

// clean last seg result
void thulac_clean_result(void *ctx);

#ifdef __cplusplus
}
#endif
#endif
