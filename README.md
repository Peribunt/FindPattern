# FindPattern
I have decided to publish this purely because of boredom. As far as I have seen, this is the fastest FindPattern routine I have ever encountered and or written

Arguments
* ` BaseAddress: The base address of the region`
* `SearchLength: The base address of the region`
* `  BytePatten: The pattern to search for, this pattern must be formatted as follows: "\xE8\x00\x00\x00\x00\xC3"`
* `  SearchMask: The mask for the battern in bits, this mask must be formatted as follows: 0b100001'00`
