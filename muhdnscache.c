#include <Windows.h>
#include <Windns.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#ifdef _MSC_VER
#pragma comment(lib, "dnsapi.lib")
#endif

#ifdef __cplusplus
extern "C"
#endif
int WINAPI DnsGetCacheDataTable(PDNS_RECORD*);

static int countEntries(PDNS_RECORD ptr)
{
    int ret = 0;
    while(ptr)
    {
        ++ret;
        ptr = ptr->pNext;
    }
    return ret;
}

static void copyEntries(PDNS_RECORD ptr, const wchar_t ** names)
{
    while(ptr)
    {
        *names++ = (const wchar_t*)ptr->pName;
        ptr = ptr->pNext;
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

static void printA(const wchar_t * domain, PDNS_RECORD p)
{
    while(p)
    {
        if(DNS_TYPE_A == p->wType)
        {
            const int a = (int)((p->Data.A.IpAddress >> 0x00) & 0xff);
            const int b = (int)((p->Data.A.IpAddress >> 0x08) & 0xff);
            const int c = (int)((p->Data.A.IpAddress >> 0x10) & 0xff);
            const int d = (int)((p->Data.A.IpAddress >> 0x18) & 0xff);
            wprintf(L"%ls %u %d.%d.%d.%d\n", domain, p->dwTtl, a, b, c, d);
        }
        p = p->pNext;
    }
}

static void printunique(const wchar_t ** strs)
{
    const wchar_t * last = NULL;
    while(*strs)
    {
        if(!samestr(*strs, last))
        {
            PDNS_RECORD r = NULL;

            /* 0x8010 was dug out of ipconfig disasm, it seems to be binary OR
            of DNS_QUERY_NO_WIRE_QUERY and some undocumented one (0x00008000 and
            few others are missing from DNS_QUERY* defines, there are other
            defines of that value but they seem unrelated) */
            if(DNS_RCODE_NOERROR == DnsQuery_W(*strs, DNS_TYPE_A, 0x8010, NULL, &r, NULL))
            {
                printA(*strs, r);
                DnsRecordListFree(r, DnsFreeRecordList);
            }
        }
        last = *strs;
        ++strs;
    }
}

int main(int argc, char ** argv)
{
    PDNS_RECORD entries = NULL;
    const wchar_t ** names = NULL;
    int ecount = 0;

    if(!DnsGetCacheDataTable(&entries))
        return 1;

    ecount = countEntries(entries);
    names = (const wchar_t**)calloc(ecount + 1, sizeof(wchar_t*));
    if(!names)
    {
        DnsRecordListFree(entries, DnsFreeRecordList);
        return 2;
    }

    copyEntries(entries, names);
    qsort(names, ecount, sizeof(wchar_t*), &myqwcmp);
    printunique(names);
    free(names);
    DnsRecordListFree(entries, DnsFreeRecordList);
    return 0;
}
