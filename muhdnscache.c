#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

struct DnsCacheEntry
{
    struct DnsCacheEntry * next;
    wchar_t * name;
    unsigned short type;
    unsigned short len;
    unsigned long flags;
};

typedef int(WINAPI*DnsGetCacheDataTableFunction)(struct DnsCacheEntry*);

static int getCacheEntries(struct DnsCacheEntry * ptr)
{
    HINSTANCE dnsapidll;
    DnsGetCacheDataTableFunction DnsGetCacheDataTable;

    dnsapidll = LoadLibraryA("dnsapi.dll");
    if(!dnsapidll)
        return 0;

    DnsGetCacheDataTable = (DnsGetCacheDataTableFunction)GetProcAddress(dnsapidll, "DnsGetCacheDataTable");
    if(!DnsGetCacheDataTable)
        return 0;

    return DnsGetCacheDataTable(ptr);
}

static int countEntries(struct DnsCacheEntry * ptr)
{
    int ret = 0;
    while(ptr)
    {
        ++ret;
        ptr = ptr->next;
    }
    return ret;
}

static void copyEntries(struct DnsCacheEntry * ptr, const wchar_t ** names)
{
    while(ptr)
    {
        *names++ = ptr->name;
        ptr = ptr->next;
    }
}

static int myqwcmp(const void * a, const void * b)
{
    return wcscmp(*(const wchar_t**)a, *(const wchar_t**)b);
}

static int samestr(const wchar_t * a, const wchar_t * b)
{
    if(a == b)
        return 1;

    if(a == NULL || b == NULL)
        return 0;

    return 0 == wcscmp(a, b);
}

static void printunique(const wchar_t ** strs)
{
    const wchar_t * last = NULL;
    while(*strs)
    {
        if(!samestr(*strs, last))
            printf("%ls\n", *strs);

        last = *strs;
        ++strs;
    }
}

int main(int argc, char ** argv)
{
    struct DnsCacheEntry entry;
    const wchar_t ** names = NULL;
    int ecount = 0;

    if(!getCacheEntries(&entry))
        return 1;

    ecount = countEntries(entry.next);
    names = (const wchar_t**)calloc(ecount + 1, sizeof(wchar_t*));
    if(!names)
        return 2;

    copyEntries(entry.next, names);
    qsort(names, ecount, sizeof(wchar_t*), &myqwcmp);
    printunique(names);
    free(names);
    /* free dns cache entries? */
    return 0;
}
