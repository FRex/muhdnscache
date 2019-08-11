# muhdnscache

A simple C program using an (undocumented) function and constant from `dnsapi.lib`
to print out all the A record in the Windows DNS Cache: their domain, TTL and IP.

This is exactly what `ipconfig /displaydns` prints, but in a more friendly
and more grepable format than it (one line per entry, space separated, etc.).

Based on:
1. This SO answer: [link](https://stackoverflow.com/a/8001616) - existence of `DnsGetCacheDataTable`
2. MSDN website and `WinDNS.h` - fields, function (`DnsQuery_W`) and flags (`DNS_QUERY_NO_WIRE_QUERY`) to use.
3. Disassembling `ipconfig.exe` - the `0x8010` constant which seems to be `0x8000` (undocumented) + `DNS_QUERY_NO_WIRE_QUERY`.

Go to releases to download a Windows exe compiled with Pelles C with no `-O2`
to avoid running into any `-O2` optimizer bug similar to this one that affected
`stb_image`: [Pelles C forum bug report](https://forum.pellesc.de/index.php?topic=7837.0)

**Note**: Earlier versions of this program had used just the `0x10` aka
`DNS_QUERY_NO_WIRE_QUERY` flag, without the undocumented `0x8000` part, which
somehow made all the `DnsQuery_W` calls fail on a VirtualBox VM of Windows 10.

`ipconfig /displaydns` still worked on that VM which led me to disassemble it
and find the `0x8000`/`0x8010` constant and another undocumented function in
`dnsapi.dll` called `DnsGetCacheDataTableEx` (this one is missing from both the
`WinDNS.h` header and `dnsapi.lib` file, unlike `DnsGetCacheDataTable` which is
only missing from the header) but replacing `DnsGetCacheDataTable` with
`DnsGetCacheDataTableEx` didn't fix my program and disassembling `dnsapi.dll` I saw
that `DnsGetCacheDataTable` is just a small wrapper around `DnsGetCacheDataTableEx`.
