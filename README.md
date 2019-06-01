# muhdnscache

A simple C program using an (undocumented) function from `dnsapi.dll` to print
out all the A record in the Windows DNS Cache: their domain, TTL and IP.

This is similar to what `ipconfig /displaydns` prints, but in a more friendly
and more grepable format than it (one line per entry, space separated, etc.).

Based on information and link found in this SO answer: [link](https://stackoverflow.com/a/8001616).

Go to releases to download a Windows exe compiled with Pelles C with no `-O2`
to avoid running into any `-O2` optimizer bug similar to this one that affected
`stb_image`: [Pelles C forum bug report](https://forum.pellesc.de/index.php?topic=7837.0)
