#ifndef oclFiltro_h
#define oclFiltro_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <opencv2/core/core.hpp>

#include <CL/cl.h>

using namespace std;

class oclFiltro
{
public:
    oclFiltro(cl_uint gpu, int width, int height);
    ~oclFiltro();

    void compute(uchar *inputData, uchar *data2, float *filter);

private:
    int global;
    size_t global_size;
    cl_int errNum;
    cl_uint numPlatforms;
    cl_uint numDevices;
    cl_platform_id * platformIDs;
    cl_device_id * deviceIDs;
    cl_context context = NULL;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem inputSignalBuffer;
    cl_mem outputSignalBuffer;
    cl_mem inputSignalFilter;

    inline void checkErr(cl_int err, const char* name);
};

#endif
