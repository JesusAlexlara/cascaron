#include "oclFiltro.hpp"

oclFiltro::oclFiltro() {


}

void oclFiltro::compute()
{

}

inline void oclFiltro::checkErr(cl_int err, const char *name) {
    if (err != CL_SUCCESS) {
        cerr << "ERROR: " <<  name << " (" << err << ")" << endl;
        exit(EXIT_FAILURE);
    }
}
