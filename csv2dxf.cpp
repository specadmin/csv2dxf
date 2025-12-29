//-------------------------------------------------------------------------------------------------
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "dxflib/dl_dxf.h"
#include "csv.h"                // https://github.com/rgamble/libcsv
#include "cvalue.h"
#include "cdxfwriter.h"
#include "options.h"
//-------------------------------------------------------------------------------------------------
#define COLS_LIMIT      5
#define MAX_FIELD_SIZE  20
#define LAYER_POINTS    0
#define LAYER_LABELS    1
//-------------------------------------------------------------------------------------------------
struct Meta
{
    CValue *fields[COLS_LIMIT];
    unsigned int fields_count = 0;
};
//-------------------------------------------------------------------------------------------------
static int endline_detector(unsigned char c)
{
    switch(c)
    {
        case CSV_CR:
        case CSV_LF:
            return 1;
    }
    return 0;
}
//-------------------------------------------------------------------------------------------------
void process_line(size_t line_number, CValue *fields[])
{
    if(line_number < options.skip_first_lines)
    {
        // skip first lines
        return;
    }
    // ========================================================================================= //
    static char nocode[] = "(none)";
    char fmt[10];
    char label[100];
    char layer[100];
    double x = fields[2]->to_double(options.round_digits);
    double y = fields[1]->to_double(options.round_digits);
    double z = fields[3]->to_double(options.round_digits);
    char *code = fields[4]->text();

    // create a point
    if(options.split_by_code)
    {
        if(*code)
        {
            dxf.use_layer(dxf.upsert_layer(code));
        }
        else
        {
            dxf.use_layer(dxf.upsert_layer(nocode));
        }

    }
    else
    {
        dxf.use_layer(LAYER_POINTS);
    }
    dxf.add_point(x, y, z);

    // create a label
    sprintf(fmt, "%%.%df", options.round_digits);
    if(*code && !options.split_by_code)
    {
        sprintf(label, "%s-%s-%s", code, fields[3]->format(fmt, options.round_digits), fields[0]->text());
    }
    else
    {
        sprintf(label, "%s-%s", fields[3]->format(fmt, options.round_digits), fields[0]->text());
    }
    printf("%s\n", label);
    if(options.split_labels)
    {
        if(options.split_by_code)
        {
            if(*code)
            {
                sprintf(layer, "%s-labels", code);
            }
            else
            {
                sprintf(layer, "%s-labels", nocode);
            }
            dxf.use_layer(dxf.upsert_layer(layer));
        }
        else
        {
            dxf.use_layer(LAYER_LABELS);
        }
    }
    dxf.add_label(x + options.labels_height / 2,
                  y + options.labels_height / 4,
                  z,
                  label);
}
//-------------------------------------------------------------------------------------------------
void csv_line_handler(int c, void *data)
{
    static unsigned int line_pos = 0;
    Meta* meta = (Meta*) data;
    process_line(line_pos++, meta->fields);

    for(size_t i = 0; i < COLS_LIMIT; i++)
    {
        delete meta->fields[i];
    }
    meta->fields_count = 0;
}
//-------------------------------------------------------------------------------------------------
void csv_field_handler(void *s, size_t len, void *data)
{
    struct Meta* meta = (Meta*) data;
    if(meta->fields_count < COLS_LIMIT)
    {
        meta->fields[meta->fields_count] = new CValue((char*)s, len);
    }
    meta->fields_count++;
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    csv_parser csv_parser;
    FILE *csv_file = NULL;
    struct Meta meta;
    size_t bytes_read = 0;
    char buf[4096];
    printf("CSV to DXF points converter\n");

    // parse cmd options
    if(cmd_options(argc, argv))
    {
        exit(EXIT_FAILURE);
    };

    // open a CSV
    csv_file = fopen(options.csv_file_name, "rb");
    if (!csv_file)
    {
        fprintf(stderr, "Failed to open source file '%s': %s\n", options.csv_file_name, strerror(errno));
        exit(errno);
    }

    // prepare CSV parser
    if(csv_init(&csv_parser, options.csv_options) != 0)
    {
        fprintf(stderr, "Failed to initialize csv parser\n");
        exit(EXIT_FAILURE);
    }
    csv_set_term_func(&csv_parser, endline_detector);
    csv_set_delim(&csv_parser, ',');

    // prepare default layers
    if(options.split_by_code == false)
    {
        dxf.add_layer("points");
        if(options.split_labels)
        {
            dxf.add_layer("labels");
        }
    }

    // main loop
    while((bytes_read = fread(buf, 1, sizeof(buf), csv_file)) > 0)
    {
        if(csv_parse(&csv_parser, buf, bytes_read, csv_field_handler, csv_line_handler, &meta) != bytes_read)
        {
            fprintf(stderr, "Error while parsing source file '%s': %s\n", options.csv_file_name, csv_strerror(csv_error(&csv_parser)));
        }
    }

    // close CSV
    csv_fini(&csv_parser, csv_field_handler, csv_line_handler, &meta);
    csv_free(&csv_parser);
    if(csv_file) { fclose(csv_file); }

    // write DXF
    if(int error = dxf.write(options.dxf_file_name))
    {
        fprintf(stderr, "Failed to create destination file '%s': %s\n", options.dxf_file_name, strerror(error));
        exit(errno);
    }
    return 0;
}
//-------------------------------------------------------------------------------------------------
