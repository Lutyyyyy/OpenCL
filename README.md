# Implementing bitonic sorting using OpenCL

## Installing 
Prerequisites (should be installed on your system):
1) OpenCL 
2) cmake 
3) gtest 
4) boost 

## Running
1. Create an empty folder where you want to build your project (e.g. it is called build)
2. run these commands:

```
cd build
cmake .. 
make (or cmake --build)
```

### Run options
While executing `./Bsort_run` you can use these options
* --help - Show help menu
* --devinfo - Info about device used while computing
* --check - Mode to check sorting correctness
* --profiling - Show computing time 
* --cpucmp - Compare performance with std::sort(parallel policy)