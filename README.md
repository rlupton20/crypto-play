Implementations of ciphers. Mostly for my education. No planning or engineering involved, just brain to code. Suffice to say, not to be depended on, or used anywhere seriously. I'm just keeping them here for reference.

Currently implemented:

* Trivium: simple lightweight stream cipher with 80-bit key. Optimized for hardware. Runs, haven't given it a thorough check yet. main.c is currently a bunch of unit tests, and a little encryption test that lets me catch bugs with valgrind.