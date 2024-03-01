# RaceNet Dissector

A Wireshark dissector for FUEL's RaceNet protocol

## Information
More information is available in the [FMTK Wiki Networking entry](https://github.com/widberg/fmtk/wiki/Networking). The compression algorithm used comes from [Using PPMD for compression](https://www.codeproject.com/Articles/1180/Using-PPMD-for-compression) which itself is adapted from, but incompatible with, [Dmitry Shkarin's PPMdE](https://compression.ru/ds/). I have cleaned up the PPMD code to be less platform dependent and compile on modern C++ as well as other small changes to bring it more in-line with the FUEL decompilation.
