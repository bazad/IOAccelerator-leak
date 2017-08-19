## IOAccelerator-leak

IOAccelerator-leak is a proof-of-concept exploit for a kernel heap pointer disclosure in
IOGraphicsFamily that was fixed in macOS Sierra 10.12.4. I reported this bug to Apple but do not
know whether it was assigned a CVE. This vulnerability can be triggered to leak the address of an
element in the `kalloc.48` zone.

### The vulnerability

The `_CreateID` function in `IOAccelerator.cpp` uses the lower 32 bits of a kernel heap pointer as
an object ID that eventually gets passed to user space. This function is accessible via the
`IOAccelerationUserClient::extCreate` external method. By retrieving the ID parameter in user space
and supplying the upper 32 bits (which are highly predictable), it is possible to recover a kernel
pointer into the `kalloc.48` zone.

### License

The IOAccelerator-leak code is released into the public domain. As a courtesy I ask that if you
reference or use any of this code you attribute it to me.
