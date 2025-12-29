#include <string.h>
#include <algorithm>
#include "cdxfwriter.h"
#include "options.h"
//-------------------------------------------------------------------------------------------------
CDXFwriter dxf;
//-------------------------------------------------------------------------------------------------
template<typename T>
static inline void safe_free(T *&ptr)
{
    if(ptr)
    {
        free(ptr);
        ptr = NULL;
    }
}
//-------------------------------------------------------------------------------------------------
CDXFwriter::CDXFwriter()
{

}
//-------------------------------------------------------------------------------------------------
CDXFwriter::~CDXFwriter()
{

}
//-------------------------------------------------------------------------------------------------
int CDXFwriter::write(const char *file_name)
{
    dw = libdxf.out(file_name, DL_Codes::AC1009);
    if(!dw)
    {
        return errno;
    }

    // write headers
    libdxf.writeHeader(*dw);
    dw->sectionEnd();

    // write layers
    if(layers_count < 1)
    {
        use_layer(add_layer("default"));
    }
    dw->tableLayers(layers_count);
    for(size_t i = 0; i < layers_count; i++)
    {
        libdxf.writeLayer(*dw, DL_LayerData(layers[i].get_name(), 0), DL_Attributes(std::string(""), DL_Codes::red, 100, "CONTINUOUS", 1.0));
    }
    dw->tableEnd();
    dw->sectionEnd();

    // write drawings
    dw->sectionEntities();

    //  points
    for(size_t i = 0; i < points_count; i++)
    {
        DL_Attributes *attrib = new DL_Attributes(layers[points[i].layerID].get_name(), 256, -1, "BYLAYER", 1.0 );
        libdxf.writePoint(*dw, DL_PointData(points[i].x, points[i].y, points[i].z), *attrib);
        delete attrib;
    }
    //  labels
    for(size_t i = 0; i < labels_count; i++)
    {
        DL_Attributes *attrib = new DL_Attributes(layers[labels[i].layerID].get_name(), 256, -1, "BYLAYER", 1.0 );
        libdxf.writeText(*dw, DL_TextData(labels[i].position.x,
                                          labels[i].position.y,
                                          labels[i].position.z,
                                          labels[i].position.x,
                                          labels[i].position.y,
                                          labels[i].position.z,
                                          options.labels_height,
                                          options.labels_width_scale,
                                          TEXT_DIR_DEFAULT,
                                          TEXT_ALIGN_LEFT,
                                          TEXT_VALIGN_MIDDLE,
                                          labels[i].text,
                                          options.labels_font,
                                          0), *attrib);
        delete attrib;
    }
    //  lines
    for(size_t i = 0; i < lines_count; i++)
    {
        // libdxf.writeLine(*dw, DL_LineData(25.0, 30.0, 0.0, 100.0, 120.0, 0.0), *attrib);
    }
    dw->sectionEnd();

    // write EOF
    dw->dxfEOF();

    // close DXF
    dw->close();
    delete dw;
    return 0;
}
//-------------------------------------------------------------------------------------------------
int CDXFwriter::use_layer(size_t layerID)
{
    if(layerID >= layers_count)
    {
        return -1;
    }
    current_layer = layerID;
    return current_layer;
}
//-------------------------------------------------------------------------------------------------
int CDXFwriter::add_layer(const char *layer_name)
{
    Layer *newlayers = new Layer[layers_count + 1];
    std::copy(layers, layers + layers_count, newlayers);
    if(layers)
    {
        delete[] layers;
    }
    layers = newlayers;
    layers[layers_count].set_name(layer_name);
    return layers_count++;
}
//-------------------------------------------------------------------------------------------------
int CDXFwriter::upsert_layer(const char *layer_name)
{
    if(!strlen(layer_name))
    {
        return -1;
    }
    for(size_t i = 0; i < layers_count; i++)
    {
        if(strcmp(layer_name, layers[i].get_name()) == 0)
        {
            return i;
        }
    }
    return add_layer(layer_name);
}
//-------------------------------------------------------------------------------------------------
int CDXFwriter::add_point(double x_coord, double y_coord, double z_coord)
{
    Point *newpoints = new Point[points_count + 1];
    std::copy(points, points + points_count, newpoints);
    if(points)
    {
        delete[] points;
    }
    points = newpoints;
    points[points_count].x = x_coord;
    points[points_count].y = y_coord;
    points[points_count].z = z_coord;
    points[points_count].layerID = current_layer;
    return points_count++;
}//-------------------------------------------------------------------------------------------------
int CDXFwriter::add_label(double x_coord, double y_coord, double z_coord, char *lable_text)
{
    Label *newlabels = new Label[labels_count + 1];
    std::copy(labels, labels + labels_count, newlabels);
    if(labels)
    {
        delete[] labels;
    }
    labels = newlabels;
    labels[labels_count].position.x = x_coord;
    labels[labels_count].position.y = y_coord;
    labels[labels_count].position.z = z_coord;
    labels[labels_count].layerID = current_layer;
    strcpy(labels[labels_count].text, lable_text);
    return labels_count++;
}
//-------------------------------------------------------------------------------------------------
char* Layer::set_name(const char *layer_name)
{
    strcpy(name, layer_name);
    return name;
}
//-------------------------------------------------------------------------------------------------

