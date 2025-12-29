#!/bin/bash

name="csv2dxf"
arc="amd64"

echo -n "Enter version: "
read version

deb="${name}-${version}_${arc}"
rm -rf $deb
mkdir -p $deb/DEBIAN
mkdir -p $deb/usr/bin

echo "Package: $name" | tee -a $deb/DEBIAN/control
echo "Architecture: $arc" | tee -a $deb/DEBIAN/control
echo "Version: $version" | tee -a $deb/DEBIAN/control
echo "Section: utils" | tee -a $deb/DEBIAN/control
echo "Priority: optional" | tee -a $deb/DEBIAN/control
echo "Maintainer: Anton Rezanov <ton@specadmin.ru>" | tee -a $deb/DEBIAN/control
echo "Depends: libcsv3, libfmt9, libdxflib3" | tee -a $deb/DEBIAN/control
echo "Description: Survey Files Converter" | tee -a $deb/DEBIAN/control
echo " Converts points information from CSV-file to DXF-file, containing points and their labels. Could be useful for surveyors to convert coordinates files created in GNSS or total stations into vector graphics format." | tee -a $deb/DEBIAN/control

install -Dm755 bin/Release/csv2dxf $deb/usr/bin/csv2dxf
dpkg-deb --build $deb
mv ${deb}.deb downloads/
rm -rf $deb