#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

struct Line {
    unsigned long long tag;
    int lru;
    int valid;
};

struct Set {
    struct Line* line;
    int m;
}*sets;

int transToInt(char* s) {
    int ans = 0;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
        ans = ans * 10 + s[i] - '0';
    }
    return ans;
}

int lineNum = 0;

int search(struct Line* line, int m, unsigned long long tag, int lru)
{
    for(int i = 0; i < m; i++) 
        if (line[i].valid && line[i].tag == tag) {
            line[i].lru = lru;
            return i;
        }
    return -1;
}

int try_insert(struct Line* line, int m, unsigned tag, int lru)
{
    if (m < lineNum) {
        line[m].valid = 1;
        line[m].tag = tag;
        line[m].lru = lru;
        return -1;
    }
    int min = line[0].lru, pos = 0;
    for(int i = 1; i < m; i++)
        if (min > line[i].lru) {
            min = line[i].lru;
            pos = i;
        }
    return pos;
}

void evict(struct Line* line, int pos, unsigned tag, int lru)
{
    line[pos].tag = tag;
    line[pos].lru = lru;
}

int main(int argc, char** argv)
{
    int hits = 0, misses = 0, evicts = 0;
    int setNum = 0, blockSize = 0;
    char ch;
    opterr = 0;
    char file[20];
    while ((ch = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch(ch) {
            case 's':
                setNum = transToInt(optarg);
                break;
            case 'E':
                lineNum = transToInt(optarg);
                break;
            case 'b':
                blockSize = transToInt(optarg);
                break;
            case 't':
                strcpy(file, optarg);
                break;
        }
    }
    if (!setNum || !lineNum || !blockSize) return 0;
    FILE* fs = fopen(file, "r");
    if (fs == NULL) return 0;
    
    int maxSet = 1 << setNum;
    sets = (struct Set*)calloc(maxSet, sizeof(struct Set));
    for(int i = 0; i < maxSet; i++)
        sets[i].line = (struct Line*) calloc(lineNum, sizeof(struct Line));
    
    unsigned long long address;
    unsigned long long tag;
    int set, size, lru = 0;
    char is[2];
    while(fscanf(fs, "%s %llx,%d", is, &address, &size) != EOF) {
        if (is[0] == 'I') continue;
        address >>= blockSize;
        set = address % maxSet;
        address >>= setNum;
        tag = address;
        struct Line* line = sets[set].line;
        int m = sets[set].m;
        if (search(line, m, tag, lru) != -1) hits++;
        else {
            misses++;
            int pos = try_insert(line, m, tag, lru);
            if (pos != -1) {
                evicts++;
                evict(line, pos, tag, lru);
            } else sets[set].m++;
        }
        if (is[0] == 'M') hits++;
        lru++;
        //printf("Set=%d Tag=%d\n", set, tag);
        //printf("%d %d %d\n", hits, misses, evicts);
    }
    fclose(fs);
    printSummary(hits, misses, evicts);
    return 0;
}
