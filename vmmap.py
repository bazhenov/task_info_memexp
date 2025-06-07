#!/usr/bin/env python3
import re
import sys

PAGE_SIZE = 2**14
SHARED_REGION_START = 0x180000000
SHARED_REGION_END = SHARED_REGION_START + 0x180000000

# Usage: vmmap -pages $$ | python3 vmmap.py

class Mapping:
    name: str
    description: str
    start_address: int
    end_address: int
    vsize: float
    rsize: float
    dirty_size: float
    swap_size: float

    def __str__(self) -> str:
        return f"{self.name:30} [{self.vsize * PAGE_SIZE / 1024 / 1024: 5.1f}M {self.rsize * PAGE_SIZE / 1024 / 1024: 5.1f}M {self.dirty_size * PAGE_SIZE / 1024 / 1024: 5.1f}M {self.swap_size * PAGE_SIZE / 1024 / 1024: 5.1f}M] ({self.start_address:09x}-{self.end_address:09x}) {self.description}"

def parse(line) -> Mapping | None:
    addr_match = re.search(r'([0-9a-f]{9,}|kernel)-([0-9a-f]{9,}|kernel)', line)

    mem_re = '[0-9]+(.[0-9]+)?'
    mem_match = re.search(fr'\[\s*({mem_re})\s+({mem_re})\s+({mem_re})\s+({mem_re})\s*]', line)
    if addr_match and mem_match:
        mapping = Mapping()
        mapping.name = line[:addr_match.start()].strip()
        mapping.description = line[mem_match.end():].strip()
        mapping.start_address = 0 if addr_match[1] == 'kernel' else int(addr_match[1], 16)
        mapping.end_address = 0 if addr_match[2] == 'kernel' else int(addr_match[2], 16)
        mapping.vsize = float(mem_match[1])
        mapping.rsize = float(mem_match[3])
        mapping.dirty_size = float(mem_match[5])
        mapping.swap_size = float(mem_match[7])
        return mapping
    return None

if __name__ == "__main__":
    vsize = 0
    rsize = 0
    dirty = 0
    swapped = 0

    mappings = []

    for line in sys.stdin:
        mapping = parse(line)
        if mapping != None and (mapping.vsize > 0 or mapping.rsize > 0 or mapping.dirty_size > 0 or mapping.swap_size > 0):
            mappings.append(mapping)


    mappings.sort(key=lambda x: x.rsize, reverse=True)
    for mapping in mappings:
        if SHARED_REGION_START <= mapping.start_address and mapping.end_address < SHARED_REGION_END \
            and mapping.dirty_size == 0 \
            and mapping.swap_size == 0 \
            and (mapping.name in ['__TEXT', '__OBJC_RO', '__LINKEDIT', '__AUTH_CONST', '__DATA_CONST', '__TPRO_CONST']):
            # dyld cache
            continue

        print(mapping)

        # if mapping.name == 'mapped file' \
        #     and 'SM=COW' in mapping.description:
        #    rsize += mapping.dirty_size
        # else:
        #    rsize += mapping.rsize


        vsize += mapping.vsize
        # if shared_region_start <= mapping.start_address and mapping.end_address < shared_region_end:
        #     rsize += mapping.dirty_size
        # else:
        #     rsize += mapping.rsize


        if mapping.name.startswith("__") and SHARED_REGION_START <= mapping.start_address and mapping.end_address < SHARED_REGION_END and mapping.name not in ['__OBJC_RW', '__DATA_DIRTY', '__DATA_CONST', '__AUTH']:
            rsize += mapping.dirty_size
        else:
            rsize += mapping.rsize

        # rsize += mapping.rsize
        dirty += mapping.dirty_size
        swapped += mapping.swap_size

    print(f"Vsize: {vsize * PAGE_SIZE / 1024 / 1024:5.2f}M")
    print(f"Real Mem: {rsize * PAGE_SIZE / 1024 / 1024:5.2f}M")
    print(f"Memory: {(dirty + swapped) * PAGE_SIZE / 1024 / 1024:5.2f}M")
    print(f"Compressed: {swapped * PAGE_SIZE / 1024 / 1024:5.2f}M")
