#include "oclFiltro.hpp"

oclFiltro::oclFiltro(cl_uint gpu = 1, int width = 640, int height = 480) {

    global = height * width;
    /*Buscamos plataformas disponibles*/
    errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
    checkErr((errNum != CL_SUCCESS) ? errNum : (numPlatforms <= 0 ? -1 : CL_SUCCESS),
            "clGetPlatformIDs");

    /*Reservamos memoria según el numero de plataformas disponibles*/
    platformIDs = (cl_platform_id *)malloc(sizeof(cl_platform_id) * numPlatforms);

    /*Obtenemos los datos de todas las plataformas*/
    errNum = clGetPlatformIDs(numPlatforms, platformIDs, NULL);
    checkErr((errNum != CL_SUCCESS) ? errNum : (numPlatforms <= 0 ? -1 : CL_SUCCESS),
           "clGetPlatformIDs");

    /*
     * De todas las plataformas que obtuvimos iteramos y buscamos una donde
     * este disponible un GPU o una CPU
    */
    deviceIDs = nullptr;
    cl_uint i;
    for (i = 0; i < numPlatforms; i++) {
        errNum = clGetDeviceIDs(platformIDs[i], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU,
                                0, NULL, &numDevices);
        if (errNum != CL_SUCCESS && errNum != CL_DEVICE_NOT_FOUND) {
                    checkErr(errNum, "clGetDeviceIDs");
        }else if(numDevices > 0) {
            deviceIDs = (cl_device_id *)malloc(sizeof(cl_device_id) * numDevices);
            errNum = clGetDeviceIDs(platformIDs[i], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, numDevices, &deviceIDs[0], NULL);
            checkErr(errNum, "clGetDeviceIDs");
            break;
        }
    }

    /*Comprueba si encontramos almenos un dispositivo*/
    if (deviceIDs == NULL) {
        cout << "No se contro ningun dispositivo" << endl;
        exit(EXIT_FAILURE);
    }

    /*Creamos el contexto con la plataforma seleccionada*/
    /*Definimos las propuedades del contexto*/
    cl_context_properties contextProperties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platformIDs[i],
        0
    };

    context = clCreateContext(contextProperties, numDevices, deviceIDs, NULL, NULL, &errNum);
    checkErr(errNum, "clCreateContext");

    /*Se crea la cola de comandos*/
    queue = clCreateCommandQueue(context, deviceIDs[0], 0, &errNum);
    checkErr(errNum, "clCreateCommandQueue");

    /*Cargamos el archivo donde se encuentra el programa de OpenCL*/
    ifstream srcFile("convolution.cl");
    checkErr(srcFile.is_open() ? CL_SUCCESS : -1, "reading convolution.cl");

    /*Se carga el archivo en un string de c++ */
    string srcProg(istreambuf_iterator<char>(srcFile),(std::istreambuf_iterator<char>()));


    /*Como OpenCL trabaja con C hacemos el respectivo cast mediante un apuntador*/
    const char *src = srcProg.c_str();
    size_t length = srcProg.length();

    /*Creamos el programa de OpenCL utilizando el apuntador*/
    program = clCreateProgramWithSource(context, 1, &src, &length, &errNum);
    checkErr(errNum, "clCreateProgramWithSource");

    /*Compilamos el programa de OpenCL*/
    errNum = clBuildProgram(program, numDevices, deviceIDs, NULL, NULL, NULL);
    /*Aqui podemos sabes con mas detalle si la compilación llega a dar un error*/
    checkErr(errNum, "clBuildProgram");

    /*Creamos el kernel*/
    kernel = clCreateKernel(program, "filtro", &errNum);
    checkErr(errNum, "clCreateKernel");

    inputSignalBuffer = clCreateBuffer(
            context,
            CL_MEM_READ_ONLY,
            sizeof(uchar) * global,
            NULL,
            &errNum);
    checkErr(errNum, "clCreateBuffer(inputSignal)");

    inputSignalFilter = clCreateBuffer(
                context,
                CL_MEM_READ_ONLY,
                sizeof(float) * 9,
                NULL,
                &errNum);
    checkErr(errNum, "clCreateFilter(inputSignal)");

    outputSignalBuffer = clCreateBuffer(
            context,
            CL_MEM_WRITE_ONLY,
            sizeof(uchar) * global,
            NULL,
            &errNum);
    checkErr(errNum, "clCreateBuffer(outputSignal)");
}

void oclFiltro::compute(uchar *inputData, uchar *data2, float *filter)
{
    global_size = global;
    errNum = clEnqueueWriteBuffer(queue, inputSignalBuffer,CL_TRUE, 0,
                                  sizeof(uchar) * global, inputData, 0, NULL, NULL);
    checkErr(errNum, "clCreateBuffer(inputSignal)");

    errNum = clEnqueueWriteBuffer(queue, inputSignalFilter,CL_TRUE, 0,
                                  sizeof(float) * 9, filter, 0, NULL, NULL);
    checkErr(errNum, "clCreateBuffer(inputSignal)");

    errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputSignalBuffer);
    errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &outputSignalBuffer);
    errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &inputSignalFilter);
    checkErr(errNum, "clSetKernelArg");

    errNum = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
    checkErr(errNum, "clEnqueueNDRangeKernel");

    errNum = clEnqueueReadBuffer(queue, outputSignalBuffer, CL_TRUE, 0, sizeof(uchar) * global, data2,
            0, NULL, NULL);
    checkErr(errNum, "clEnqueueReadBuffer");
}

inline void oclFiltro::checkErr(cl_int err, const char *name) {
    if (err != CL_SUCCESS) {
        cerr << "ERROR: " <<  name << " (" << err << ")" << endl;
        exit(EXIT_FAILURE);
    }
}
