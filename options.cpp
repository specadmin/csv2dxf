#include <stdio.h>
#include <unistd.h>
#include "options.h"
//-------------------------------------------------------------------------------------------------
COptions options;
//-------------------------------------------------------------------------------------------------
static inline void print_usage()
{
    fprintf(stderr, "Usage: csv2dxf [-s skip_lines] [-r round_to_digits] source_file destination_file\n");
}
//-------------------------------------------------------------------------------------------------
int cmd_options(int argc, char *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "ERROR! Not enough parameters\n");
        print_usage();
        return 1;
    }
    options.csv_file_name = argv[argc - 2];
    options.dxf_file_name = argv[argc - 1];
    options.dxf_version = DL_Codes::AC1009;
    int opt;
    char *endptr = NULL;
    while((opt = getopt(argc, argv, "KLs:r:")) != -1)
    {
        switch(opt)
        {
            case 'K':
                options.split_by_code = true;
                break;
            case 'L':
                options.split_labels = true;
                break;
            case 's':
                options.skip_first_lines = strtol(optarg, &endptr, 10);
                if(*endptr)
                {
                    fprintf(stderr, "Invalid value for parameter -s\n");
                    print_usage();
                    return 1;
                }
                break;
            case 'r':
                options.round_digits = strtol(optarg, &endptr, 10);
                if(*endptr)
                {
                    fprintf(stderr, "Invalid value for parameter -r\n");
                    print_usage();
                    return 1;
                }
                break;
        }
    }
    return 0;
}
//-------------------------------------------------------------------------------------------------
