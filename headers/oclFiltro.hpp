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
    oclFiltro();
    ~oclFiltro();


private:

    inline void checkErr(cl_int err, const char* name);
};

#endif
