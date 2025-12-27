#ifndef CDXFWRITER_H
#define CDXFWRITER_H
#include <stdlib.h>
#include "dxflib/dl_dxf.h"
//-------------------------------------------------------------------------------------------------
// textGenerationFlags
#define TEXT_DIR_DEFAULT        0
#define TEXT_DIR_BACKWARDS      2
#define TEXT_DIR_UPSIDEDOWN     4
// Horizontal alignment
#define TEXT_ALIGN_LEFT         0
#define TEXT_ALIGN_CENTER       1
#define TEXT_ALIGN_RIGHT        2
#define TEXT_ALIGN_TOTAL        3
#define TEXT_ALIGN_MIDDLE       4
#define TEXT_ALIGN_FIT          5
// Vertical alignment
#define TEXT_VALIGN_BASELINE    0
#define TEXT_VALIGN_BOTTOM      1
#define TEXT_VALIGN_MIDDLE      2
#define TEXT_VALIGN_TOP         3
//-------------------------------------------------------------------------------------------------
struct Point
{
    Point()
    {
        x = 0;
        y = 0;
        z = 0;
    };
    Point(double x_coord, double y_coord, double z_coord = 0)
    {
        x = x_coord;
        y = y_coord;
        z = z_coord;
    };
    Point(const Point &src)
    {
        x = src.x;
        y = src.y;
        z = src.z;
    };
    double x;
    double y;
    double z;
};
//-------------------------------------------------------------------------------------------------
struct Line
{
    Line(Point &start_point, Point &end_point)
    {
        P1 = start_point;
        P2 = end_point;
    };
    Point P1;
    Point P2;
};
//-------------------------------------------------------------------------------------------------
struct Label
{
    Point position;
    char text[100];
};
//-------------------------------------------------------------------------------------------------
class CDXFwriter
{
    public:
        CDXFwriter();
        ~CDXFwriter();
        int add_layer();
        int use_layer();
        int add_point(double x_coord, double y_coord, double z_coord);
        int add_label(double x_coord, double y_coord, double z_coord, char *lable_text);
        //int add_line();
        int write(const char *file_name);
    private:
        DL_Dxf libdxf;
        DL_WriterA *dw = NULL;
        Point *points = NULL;
        Label *labels = NULL;
        Line *lines = NULL;
        size_t points_count = 0;
        size_t lines_count = 0;
        size_t labels_count = 0;
};
//-------------------------------------------------------------------------------------------------
extern CDXFwriter dxf;
//-------------------------------------------------------------------------------------------------
#endif
