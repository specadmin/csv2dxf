#include <stdio.h>
#include "options.h"
//-------------------------------------------------------------------------------------------------
COptions options;
//-------------------------------------------------------------------------------------------------
int cmd_options(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("ERROR! Not enough parameters\n");
        printf("Usage: csv2dxf source destination\n");
        return 1;
    }
    options.csv_file_name = argv[argc - 2];
    options.dxf_file_name = argv[argc - 1];
    options.dxf_version = DL_Codes::AC1009;
    options.round_digits = 2;
    options.skip_first_lines = 1;
    return 0;
}
//-------------------------------------------------------------------------------------------------
