#include "thulac.h"
#include "thulac_lib.h"
#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
void showhelp(){
    std::cerr<<"Command line usage:"<<std::endl;
    std::cerr<<"./thulac [-t2s] [-seg_only] [-filter] [-deli delimeter] [-user userword.txt] [-model_dir dir]"<<std::endl;
    std::cerr<<"or"<<std::endl;
    std::cerr<<"./thulac [-t2s] [-seg_only] [-filter] [-deli delimeter] [-user userword.txt] [-intput inputfile] [-output outputfile]"<<std::endl;
    std::cerr<<"\t-t2s\t\t\ttransfer traditional Chinese text to Simplifed Chinese text"<<std::endl;
    std::cerr<<"\t-seg_only\t\tsegment text without Part-of-Speech"<<std::endl;
    std::cerr<<"\t-filter\t\t\tuse filter to remove the words that have no much sense, like \"could\""<<std::endl;
    std::cerr<<"\t-deli delimeter\t\tagsign delimeter between words and POS tags. Default is _"<<std::endl;
    std::cerr<<"\t-user userword.txt\tUse the words in the userword.txt as a dictionary and the words will labled as \"uw\""<<std::endl;
    std::cerr<<"\t-model_dir dir\t\tdir is the directory that containts all the model file. Default is \"models/\""<<std::endl;
    std::cerr<<"\t-input inputfile\t\tinputfile is the text that needs to be segmented. Default is stdin"<<std::endl;
    std::cerr<<"\t-output outputfile\t\toutputfile is the result of the output. Default is stdout"<<std::endl;
}

void print(const THULAC_result& result, bool seg_only, char separator) {
    for(int i = 0; i < result.size() - 1; i++) {
        if(i != 0) cout << " ";
        if(seg_only) {
            cout << result[i].first;
        }
        else {
            cout << result[i].first << separator << result[i].second;
        }
    }
    cout << endl;
}
//#define THULAC_SHARED_LIB
#ifndef THULAC_SHARED_LIB
int main (int argc,char **argv) {

    char* user_specified_dict_name=NULL;
    char* model_path_char = NULL;
    char* input_path = NULL;
    char* output_path = NULL;


    char separator = '_';

    bool useT2S = false;
    bool seg_only = false;
    bool useFilter = false;
    bool multi_thread = false;

    int c = 1;
    while(c < argc){
        std::string arg = argv[c];
        if(arg == "-t2s"){
            useT2S = true;
        }else if(arg == "-user"){
            user_specified_dict_name = argv[++c];
        }else if(arg == "-deli"){
            separator = argv[++c][0];
        }else if(arg == "-seg_only"){
            seg_only = true;
        }else if(arg == "-filter"){
            useFilter = true;
        }else if(arg == "-model_dir"){
            model_path_char = argv[++c];
        }else if(arg == "-input") {
            input_path = argv[++c];
        }else if(arg == "-output") {
            output_path = argv[++c];
        }else if(arg == "-multi-thread") {
            multi_thread = true;
        }else{
            showhelp();
            return 1;
        }
        c ++;
    }

    THULAC lac;
    lac.init(model_path_char, user_specified_dict_name, seg_only, useT2S, useFilter);
    std::ifstream finput;
    if(input_path) {
        finput.open(input_path);
        cin.rdbuf(finput.rdbuf());
    }
    std::ofstream foutput;
    if(output_path) {
        foutput.open(output_path);
        cout.rdbuf(foutput.rdbuf());
    }
    std::string raw;
    THULAC_result result;
    clock_t start = clock();
    while(getline(cin, raw)) {
        lac.cut(raw, result);
        print(result, seg_only, separator);
    }
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    std::cerr<<duration<<" seconds"<<std::endl;
    finput.close();
    foutput.close();
    return 0;
}

#else

//extern "C" {    
void* 
thulac_init(const char * model_path, const char* user_path, int just_seg, int t2s, int ufilter) {
    THULAC *lac = new THULAC();
    lac->init(model_path, user_path, just_seg, t2s, ufilter);
    return static_cast<void *>(lac);
}

void 
thulac_deinit(void *lac_c) {
    if (lac_c) {
        THULAC *lac = static_cast<THULAC *>(lac_c);
        lac->deinit();
        delete lac;
    }
}

struct thulac_tag* 
_thulac_next_tag(struct thulac_tag *tag) {
    tag->next = (struct thulac_tag*)calloc(sizeof(struct thulac_tag), 1);
    return tag->next;
}

// return tag list
struct thulac_tag* 
thulac_seg(void *lac_c, const char *in) {
    if (lac_c == NULL || in == NULL) {
        return NULL;
    }
    THULAC *lac = static_cast<THULAC *>(lac_c);
    THULAC_result result;
    std::string raw(in);
    lac->cut(raw, result);
    struct thulac_tag head;
    struct thulac_tag *tag = &head;
    for (int i=0; i<result.size() - 1; i++) {
        tag = _thulac_next_tag(tag);
        tag->word = strndup(result[i].first.c_str(), result[i].first.length());
        if (!lac->segOnly()) {
            tag->tag = strndup(result[i].second.c_str(), result[i].second.length());
        }
    }
    return head.next;
}

void
thulac_clean(void *lac_c, struct thulac_tag *list) {
    if (list == NULL) {
        return;
    }
    while (list) {
        struct thulac_tag *next = list->next;
        if (list->word) {
            free(list->word);
        }
        if (list->tag) {
            free(list->tag);
        }
        free(list);
        list = next;
    }
}
//}
#endif