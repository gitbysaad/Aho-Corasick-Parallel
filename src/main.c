#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "ahocorasick.h"

void callback_match_total(void *arg, struct aho_match_t* m)
{
    long long int* match_total = (long long int*)arg;
    (*match_total)++;
}

void callback_match_pos(void *arg, struct aho_match_t* m)
{
    char* text = (char*)arg;

    printf("match text:");
    for(unsigned int i = m->pos; i < (m->pos+m->len); i++)
    {
        printf("%c", text[i]);
    }

    printf(" (match id: %d position: %llu length: %d)\n", m->id, m->pos, m->len);
}



void sample_code_bench_file()
{
    struct ahocorasick aho;
    long long int match_total = 0;
    int id[10] = {0};

    FILE *fp = NULL;
    char buf[4096] = {0};

    aho_init(&aho);

   id[0] = aho_add_match_text(&aho, "saad", 3);
   id[1] = aho_add_match_text(&aho, "HOACXEUCVDIASIGXOAUHLAIKBMLVCB", 30);
   id[2] = aho_add_match_text(&aho, "771637149162578326925576559617211246813792391884731631637638291455568882413363777414226733154448565429449556179152484317348349173217246987226449451129523516659693511711192993543637572721628982411817649274661796796518276223342492241914759893735251297323752486619843425636669919271785365511423558539910", 300);

    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_total, &match_total);

    fp = fopen("32MB", "r");
    while (fgets(buf, 4096, fp) != NULL)
    {
        aho_findtext(&aho, buf, strlen(buf));
    }

    printf("%lld\n", match_total);

    aho_destroy(&aho);
    fclose(fp);
}

void* __sample_code_thread(void *t)
{
	struct ahocorasick* aho = (struct ahocorasick*)t;

	printf("try: %s\n", "dabcacdfc");
	printf("total match:%u\n", aho_findtext(aho, "dabcacdfc", strlen("dabcacdfc")));

	pthread_exit((void*) t);
}

int main(int argc, const char *argv[])
{
	//sample_code();
	//sample_code2();
    //sample_code3();
    //sample_code_thread();
    sample_code_bench_file();
    return 0;
}
