#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int64_t ledger(int32_t arg1, int64_t pid, void *buffer, int64_t *size);

#define LEDGER_NAME_MAX 32

#define LEDGER_INFO             0
#define LEDGER_ENTRY_INFO       1
#define LEDGER_TEMPLATE_INFO    2

typedef struct {
	char    li_name[LEDGER_NAME_MAX];
	int64_t li_id;
	int64_t li_entries;
} ledger_info;

typedef struct {
	int64_t         lei_balance;
	int64_t         lei_credit;
	int64_t         lei_debit;
	uint64_t        lei_limit;
	uint64_t        lei_refill_period;      /* In nanoseconds */
	uint64_t        lei_last_refill;        /* Time since last refill */
} ledger_entry_info;

typedef struct {
	char            lti_name[LEDGER_NAME_MAX];
	char            lti_group[LEDGER_NAME_MAX];
	char            lti_units[LEDGER_NAME_MAX];
} ledger_template_info;

int main(int argc, char **argv) {
    int32_t pid;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(1);
    }
    pid = atoi(argv[1]);
    char *ledger_name = NULL;
    if (argc >= 3) {
        ledger_name = argv[2];
    }

    ledger_info *info = (ledger_info *)malloc(sizeof(ledger_info));
    if (ledger(LEDGER_INFO, pid, info, 0) < 0) {
        perror("ledger() failed");
    }
    int64_t ledgers_cnt = info->li_entries;

    ledger_entry_info *entries = (ledger_entry_info *)malloc(sizeof(ledger_entry_info) * ledgers_cnt);
    if (ledger(LEDGER_ENTRY_INFO, pid, entries, &ledgers_cnt) < 0) {
        perror("ledger() failed");
    }

    ledger_template_info *templates = (ledger_template_info *)malloc(sizeof(ledger_template_info) * ledgers_cnt);
    if (ledger(LEDGER_TEMPLATE_INFO, (int64_t)templates, &pid, 0) < 0) {
        perror("ledger() failed");
    }

    for(size_t i = 0; i< ledgers_cnt; i++) {
        ledger_entry_info *entry = &entries[i];
        ledger_template_info *template = &templates[i];
        if ( ledger_name == NULL || strstr(template->lti_name, ledger_name) != NULL ) {
            printf(" %10s %-35s %12lld %12lld %12lld %s\n", template->lti_group, template->lti_name,
                entry->lei_balance, entry->lei_credit, entry->lei_debit, template->lti_units);
        }
    }
}
