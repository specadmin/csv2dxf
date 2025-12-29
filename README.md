CSV to DXF converter
====================

Converts points information from CSV-file to DXF-file, containing points and their labels.
Could be useful for surveyors to convert coordinates files created in GNSS or total stations into
vector graphics format.

Supports only the comma as a fields-separator and the following source file data structure:
```
[Point name],[N],[E],[Z],[code]
```
N, E and Z values must be in meters. All the following fields are beeing ignored.

```
Usage: csv2dxf [-KL] [-s skip_lines] [-r round_to_digits] {source_file} {destination_file}

-K
    Split points by layers due to their codes.
    Points with the same code will be placed in separate layer, named {code}. A distinct layer will
    be created for every unique point's code, found in the source file. Without -K all point are
    placed at the same layer, named "points". Default label's text does not contains code in this
    mode.

-L
    Place labels in additional layers(s).
    By default, labels are placed at the same layer(s) with points. Adding this option forces labels
    to be placed in additional layer(s). When -K option is used, the layer for labels will be named
    {code}-labels.

-s skip_lines
    Skip (do not process) first skip_lines lines. Could be usefull for skipping header line(s).
    By default all lines are being processed from the first one.

-r round_to_digits
    Round coordinates and labels to round_to_digits decimal places.
    By default rounds to 3 decimal places (millimeters).
```
