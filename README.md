# FindPattern
I have decided to publish this purely because of boredom. As far as I have seen, this is the fastest FindPattern routine(s) I have ever encountered and or written. OpenMP is not used; its implementation is left as an exercise for the reader

Arguments
```
[in]   BaseAddress: The base address of the data to scan
[in]  SearchLength: The length of the data to scan
[in]   BytePattern: The pattern to search for.
                    This pattern must be formatted as a raw byte array
[in] PatternLength: The length of the pattern
[in]    SearchMask: The search mask.
                    This must be formatted as an 8-bit aligned binary mask.

                        Example Pattern 1.) \x48\x00\x00\x00\xC3\xCC\xCC
                           Example Mask 1.) 0b100011'00 <--- Make sure to trail zeroes to align it to the size of a byte
```
