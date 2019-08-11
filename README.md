# muhdnscache

A simple C program using an (undocumented) function from `dnsapi.lib` to print
out all the A record in the Windows DNS Cache: their domain, TTL and IP.

This is similar to what `ipconfig /displaydns` prints, but in a more friendly
and more grepable format than it (one line per entry, space separated, etc.).

Based on information and link found in this SO answer: [link](https://stackoverflow.com/a/8001616).

**Note:** When testing on a Windows 10 VM in Virtual Box this program
**prints nothing** while `ipconfig /displaydns` still works. I'm not sure why
that is: using just the records from `DnsGetCacheDataTable` instead of passing
domains from it to `DnsQuery_W` with `DNS_QUERY_NO_WIRE_QUERY` does work but
the TTL is wrong (extremely high) and results of that mismatch
`ipconfig /displaydns` too (different domains listed or not).
`ipconfig /displaydns` also uses another function from `dnsapi.dll` (it's not
in the lib file, while the non-Ex one is in lib too) `DnsGetCacheDataTableEx`.

Go to releases to download a Windows exe compiled with Pelles C with no `-O2`
to avoid running into any `-O2` optimizer bug similar to this one that affected
`stb_image`: [Pelles C forum bug report](https://forum.pellesc.de/index.php?topic=7837.0)
