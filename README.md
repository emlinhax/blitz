# blitz
a header-only library to dynamically resolve modules and exports while also being able to call them directly

# why would i use this?
1. this library makes it easy to dynamically import functions that are not provided in the standard windows sdk thanks to the macros.
2. it is a great way to make a reverse engineer's life a little more difficult. examples of that can be seen below.

# features
1. absolutely no strings are stored in the resulting binary, which makes static analysis much more difficult.
2. it generates pretty messy and confusing assembly/pseudocode, which is a good thing because this library's focus is obfuscation, not speed.
3. even dynamic analysis is difficult because this library does not use any winapi functions (or anything that could be hooked or monitored).

it is recommended to use this library in combination with a real obfuscator.
the hashing algorithm can be replaced with any other algorithm.
if you do replace the algorithm, make sure to use inlining (or a macro).

# anti reverse engineering
wondering what i meant by "make life for reverse engineer a little more difficult" ?

original function:
```cpp
void run()
{
    BLITZ_FUNC("kernel32.dll", Beep, void(*)(DWORD, DWORD));
    Beep(1000, 1000);
}
```
\

IDA's generated pseudocode:
```cpp
void run(void)
{
  void (__fastcall *v0)(__int64); // r14
  _LIST_ENTRY *p_InMemoryOrderModuleList; // rbx
  _LIST_ENTRY *Flink; // r11
  _LIST_ENTRY *v3; // rdx
  unsigned __int64 v4; // rax
  unsigned __int16 v5; // cx
  char v6; // al
  char *v7; // rdx
  char *v8; // r9
  unsigned int v9; // r8d
  __int64 v10; // rax
  __int64 v11; // r10
  __int64 v12; // rcx
  _LIST_ENTRY *v13; // rbx
  int v14; // r11d
  _DWORD *v15; // rbp
  unsigned int v16; // edi
  unsigned int v17; // r8d
  __int64 v18; // rax
  char *v19; // r9
  __int64 v20; // r10
  __int64 v21; // rcx
  char v22[128]; // [rsp+110h] [rbp-A8h] BYREF

  v0 = 0i64;
  p_InMemoryOrderModuleList = &NtCurrentPeb()->Ldr->InMemoryOrderModuleList;
  Flink = p_InMemoryOrderModuleList->Flink;
  if ( p_InMemoryOrderModuleList->Flink == p_InMemoryOrderModuleList )
  {
LABEL_20:
    v13 = 0i64;
  }
  else
  {
    while ( 1 )
    {
      v3 = Flink[5].Flink;
      v4 = 0i64;
      while ( 1 )
      {
        v5 = *((_WORD *)&v3->Flink + v4);
        if ( !v5 )
          break;
        if ( v5 <= 0x7Fu )
        {
          v22[v4++] = v5;
          if ( v4 < 0x80 )
            continue;
        }
        goto LABEL_8;
      }
      v22[v4] = 0;
LABEL_8:
      v6 = v22[0];
      if ( v22[0] )
      {
        v7 = v22;
        do
        {
          if ( (unsigned __int8)(v6 - 65) <= 0x19u )
            *v7 = v6 + 32;
          v6 = *++v7;
        }
        while ( v6 );
      }
      v8 = v22;
      v9 = -1;
      v10 = -1i64;
      do
        ++v10;
      while ( v22[v10] );
      if ( (_DWORD)v10 )
      {
        v11 = (unsigned int)v10;
        do
        {
          v12 = (unsigned int)*v8++;
          v9 = crc::table[v12 ^ (unsigned __int8)v9] ^ (v9 >> 8);
          --v11;
        }
        while ( v11 );
        if ( ~v9 == 1793498882 )
          break;
      }
      Flink = Flink->Flink;
      if ( Flink == p_InMemoryOrderModuleList )
        goto LABEL_20;
    }
    v13 = Flink[2].Flink;
  }
  v14 = 0;
  v15 = (_DWORD *)((char *)v13 + (unsigned int)*(_OWORD *)((char *)&v13[8].Blink + SHIDWORD(v13[3].Blink)));
  v16 = v15[5];
  if ( v16 )
  {
    while ( 1 )
    {
      v17 = -1;
      v18 = -1i64;
      v19 = (char *)v13 + *(unsigned int *)((char *)&v13->Flink + (unsigned int)(v15[8] + 4 * v14));
      do
        ++v18;
      while ( v19[v18] );
      if ( (_DWORD)v18 )
      {
        v20 = (unsigned int)v18;
        do
        {
          v21 = (unsigned int)*v19++;
          v17 = crc::table[v21 ^ (unsigned __int8)v17] ^ (v17 >> 8);
          --v20;
        }
        while ( v20 );
        if ( ~v17 == 382380223 )
          break;
      }
      if ( ++v14 >= v16 )
        goto LABEL_32;
    }
    v0 = (void (__fastcall *)(__int64))((char *)v13
                                      + *(unsigned int *)((char *)&v13->Flink
                                                        + 4
                                                        * *(unsigned __int16 *)((char *)&v13->Flink
                                                                              + 2 * v14
                                                                              + (unsigned int)v15[9])
                                                        + (unsigned int)v15[7]));
  }
LABEL_32:
  v0(1000i64);
}
```



Inspiration: https://github.com/JustasMasiulis/lazy_importer
