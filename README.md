# About
WinApi Dump SAM - SYSTEM - SECURITY registry keys for offline parsing and hash extraction.

## Instructions

Compile and run `samdump.exe`:

```
samdump\x64\release> samdump.exe
```

By default the output will be saved in the following files:

```
C:\ProgramData\sam.save - SAM
C:\ProgramData\system.save - SYSTEM
C:\ProgramData\security.save - SECURITY
```

You can modify the file names by changing `samdump.cpp`.
