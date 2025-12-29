#ifndef OPTIONS_H
#define OPTIONS_H
//-------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include "dxflib/dl_dxf.h"
//-------------------------------------------------------------------------------------------------
struct COptions
{
    char *csv_file_name = NULL;
    char *dxf_file_name = NULL;
    char *custom_label = NULL;
    unsigned char csv_options = 0;
    int round_digits = 3;
    size_t skip_first_lines = 0;
    DL_Codes::version dxf_version;
    double labels_height = 0.5;
    double labels_width_scale = 0.9;
    bool split_by_code = false;
    bool split_labels = false;
    char labels_font[10] = "Arial";
};
//-------------------------------------------------------------------------------------------------
extern COptions options;
int cmd_options(int argc, char *argv[]);
//-------------------------------------------------------------------------------------------------
#endif
