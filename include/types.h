#ifndef TYPES_H
#define TYPES_H

struct SMAP_entry{
	uint32 BaseL; // base address uint64_t
	uint32 BaseH;
	uint32 LengthL; // length uint64_t
	uint32 LengthH;
	uint32 Type; // entry Type
	uint32 ACPI; // extended
} ;

};
struct mem_entry{
    unsigned int base_address;
    unsigned int length;
};

struct page{
    //mem_entries array
};
