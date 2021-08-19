#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
#if defined(OPT)
    entry *pHead[hashnum], **e;
    e = malloc(sizeof(entry *)*hashnum);
    for(int i=0; i<hashnum; i++) {
        pHead[i] = (entry *) malloc(sizeof(entry));
        printf("size of entry : %lu bytes\n", sizeof(entry));
        e[i] = pHead[i];
        pHead[i]->pNext = NULL;
    }
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

#if defined(OPT)
    /* remove blank node */
    entry *tmp;
    for(int i=0; i<hashnum; i++) {
        tmp = pHead[i];
        pHead[i] = pHead[i]->pNext;
        free(tmp);
        e[i] = pHead[i];
    }
#else
    /* remove blank node */
    entry *tmp = pHead;
    pHead = pHead->pNext;
    free(tmp);
    e = pHead;
#endif

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    entry *ans = findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("result: %s\n", ans->lastName);
    cpu_time2 = diff_in_second(start, end);

    FILE *output;
#if defined(OPT)
    output = fopen("opt.txt", "a");
#else
    output = fopen("orig.txt", "a");
#endif
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

#if defined(OPT)
    for(int i=0; i<hashnum; i++) {
        while (pHead[i]->pNext) {
            tmp = pHead[i];
            pHead[i] = pHead[i]->pNext;
            free(tmp);
        }
        free(pHead[i]);
    }
#else
    while (pHead->pNext) {
        tmp = pHead;
        pHead = pHead->pNext;
        free(tmp);
    }
    free(pHead);
#endif

    return 0;
}
