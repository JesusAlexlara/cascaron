#include <CL/cl.h>
#include <iostream>

using namespace std;

void platforms();
void devices();


int main( int argc, char** argv )
{
    //platforms();
    devices();
    return EXIT_SUCCESS;
}

void devices() {
    int i, j;
    char* value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_uint maxComputeUnits;

    // Obtenemos todas las plataformas
    clGetPlatformIDs(0, nullptr, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, nullptr);

    for (i = 0; i < platformCount; i++) {

        // Obtenemos todos los dispositivos
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, nullptr);

        // Para cada dispositivo se imprimen sus atributos.
        for (j = 0; j < deviceCount; j++) {

            // Imprime el nombre del dispositivo
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, nullptr, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, nullptr);
            printf("%d. Dispositivo: %s\n", j+1, value);
            free(value);

            // Imprime la version del hardware dispositivo
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, nullptr, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, nullptr);
            printf(" %d.%d Versión del Hardware: %s\n", j+1, 1, value);
            free(value);

            // Imprime la versión del driver
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, nullptr, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, nullptr);
            printf(" %d.%d Versión del driver: %s\n", j+1, 2, value);
            free(value);

            // Imprime la version del OpenCL
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, nullptr, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, nullptr);
            printf(" %d.%d versión de OpenCL C : %s\n", j+1, 3, value);
            free(value);

            // Imprime las unidades de procesamiento en paralelo disponibles.
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                    sizeof(maxComputeUnits), &maxComputeUnits, nullptr);
            printf(" %d.%d Parallel compute units: %d\n", j+1, 4, maxComputeUnits);

        }
        free(devices);
        }
    free(platforms);
}

void platforms() {
    int i, j;
    char* info;
    size_t infoSize;
    cl_uint platformCount;
    cl_platform_id *platforms;

    const char* attributeNames[5] = {
        "Nombre", "Vendor", "Versión", "Perfiles", "Extenciones"
    };

    const cl_platform_info attributeTypes[5] = {
        CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION,
        CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS
    };

    const int attributeCount = sizeof(attributeNames) / sizeof(char*);

    // Obtenemos el numero las plataformas disponibles.
    clGetPlatformIDs(5, nullptr, &platformCount);

    // Obtenemos todas las plataformas
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, nullptr);

    for (i = 0; i < platformCount; i++) {
        printf("\n %d. Platforma \n", i+1);
        for (j = 0; j < attributeCount; j++) {

            // Obtiene el tamaño de atributos que contiene la plataforma
            clGetPlatformInfo(platforms[i], attributeTypes[j], 0, nullptr, &infoSize);
            info = (char*) malloc(infoSize);

            // Obtiene los atributos
            clGetPlatformInfo(platforms[i], attributeTypes[j], infoSize, info, nullptr);

            printf("  %d.%d %-11s: %s\n", i+1, j+1, attributeNames[j], info);
            free(info);
        }
        printf("\n");
    }

}




