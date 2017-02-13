# Requirements
- Boost (at least: filesystem, system, program_options, graph)

# Compilation
**Use the following commands for cmake generation**
- Profiling: 
```
cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc-6 -DCMAKE_CXX_COMPILER=g++-6 ../../

```
- Debug: 
```
cmake -DCMAKE_CXX_FLAGS=-g -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc-6 -DCMAKE_CXX_COMPILER=g++-6 ../../
```

- Release: 
```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc-6 -DCMAKE_CXX_COMPILER=g++-6 ../../
```

**For profiling use the following command**
```
call ./WikiTalkPageParser 
```
```
gprof WikiTalkPageParser | gprof2dot --strip -n0 -e0 --wrap > ~/Schreibtisch/profiling.dot
```
