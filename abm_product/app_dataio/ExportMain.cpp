#include "DataExportor.h"
#include "ThreeLogGroup.h"
#include "TimesTenAccess.h"

ThreeLogGroup logg;


int main(int argc, char **argv)
{
    try
    {

        DataExportor exporter(argc, argv);

        exporter.run();
    }
    catch(...)
    {        
        logg.log(MBC_MEM_EXPORT_FAIL,LOG_LEVEL_FATAL,LOG_TYPE_SYSTEM,"�ڴ浼���쳣����������ļ�Ȩ�޻��Ǳ�");
        return -1;
    }

    return 0;
}
