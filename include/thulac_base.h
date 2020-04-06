#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "thulac_raw.h"

namespace thulac{


typedef Raw Word;
typedef Raw RawSentence;
typedef std::vector<Word> SegmentedSentence;

enum POC{
    kPOC_B='0',
    kPOC_M='1',
    kPOC_E='2',
    kPOC_S='3'
};

struct thulac_tag_base {
    char *word; // word in sentence
    char *tag; // word tag, NULL for seg_only 
    struct thulac_tag_base *next; // NULL for 
};

thulac_tag_base* _thulac_next_tag(struct thulac_tag_base *tag) {
    tag->next = (struct thulac_tag_base *)calloc(1, sizeof(struct thulac_tag_base));
    return tag->next;
}

typedef std::vector<POC> POCSequence;
typedef std::vector<int> POCGraph;

struct WordWithTag{
    Word word;
    std::string tag;
	char separator;
	WordWithTag(char separator){
		this->separator = separator;
	}
    friend std::ostream& operator<< (std::ostream& os,WordWithTag& wwt){
        os<<wwt.word<<wwt.separator<<wwt.tag;
        return os;
    }
    static thulac_tag_base* tags_out(WordWithTag& wwt) {
        thulac_tag_base *tag = new thulac_tag_base;
        std::ostringstream w;
        w << wwt.word;
        tag->word = strndup(w.str().c_str(), w.str().length() + 1);
        tag->tag = strndup(wwt.tag.c_str(), wwt.tag.length() + 1);
        tag->next = NULL;
        return tag;
    }
};

//typedef std::vector<WordWithTag> TaggedSentence;
class TaggedSentence : public std::vector<WordWithTag>{
    friend std::ostream& operator<< (std::ostream& os,TaggedSentence& sentence){
        for(size_t i=0;i<sentence.size();i++){
            if(i!=0)os<<" ";
            os<<sentence[i];
        }
        return os;    
    };
public:
    static thulac_tag_base* tags_out(TaggedSentence& sentence) {
        thulac_tag_base head;
        thulac_tag_base *tag = &head;
        for(size_t i=0;i<sentence.size();i++){
            tag->next = WordWithTag::tags_out(sentence[i]);
        }
        return head.next;    
    };
};

void get_label_info(const char* filename, char** label_info, int** pocs_to_tags){
        std::list<int> poc_tags[16];
    char* str=new char[16];
    FILE *fp;
    fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "[ERROR] cws_label.txt file not find ", filename);
        return;
    }
    
    int ind=0;
    while( fscanf(fp, "%s", str)==1){
        label_info[ind]=str;
        int seg_ind=str[0]-'0';
        for(int j=0;j<16;j++){
            if((1<<seg_ind)&(j)){
                poc_tags[j].push_back(ind);
            }
        }
        str=new char[16];
        ind++;
    }
    delete[]str;
    fclose(fp);

    /*pocs_to_tags*/
    for(int j=1;j<16;j++){
        pocs_to_tags[j]=new int[(int)poc_tags[j].size()+1];
        int k=0;
        for(std::list<int>::iterator plist = poc_tags[j].begin();
                plist != poc_tags[j].end(); plist++){
            pocs_to_tags[j][k++]=*plist;
        };
        pocs_to_tags[j][k]=-1;
    }

}

template<class KEY>
class Indexer{
public:
    std::map<KEY,int> dict;
    std::vector<KEY> list;
    Indexer(){dict.clear();};
    void save_as(const char* filename);
    int get_index(const KEY& key){
        typename std::map<KEY,int>::iterator it;
        it=dict.find(key);
        if(it==dict.end()){
            int id=(int)dict.size();
            dict[key]=id;
            list.push_back(key);
            return id;
        }else{
            return it->second;
        }
    };
    KEY* get_object(int ind){
        if(ind<0||ind>=dict.size())return NULL;
        return &list[ind];
    }
};

template<class KEY>
class Counter:public std::map<KEY,int>{
public:
    void update(const KEY& key){
        typename std::map<KEY,int>::iterator it;
        it=this->find(key);
        if(it==this->end()){
            (*this)[key]=0;
        }
        (*this)[key]++;
    }
};

}//end of thulac
