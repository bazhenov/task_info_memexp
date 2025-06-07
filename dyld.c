#include <stdio.h>
#include <stdint.h>

extern uint64_t dyld_shared_cache_get_base_address(uint64_t *result);

extern int64_t _dyld_shared_cache_real_path(char *);

int main(void) {
    uint64_t result = 0;
    printf("%lld\n", dyld_shared_cache_get_base_address(&result));
    printf("%lld\n", result);
}
