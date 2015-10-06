# WinProm

WinProm is a Windows tool for calculating the topographic prominence
of mountains.  It was written by Edward Earl, a pioneer in prominence
research and calculation.

## Building the code

WinProm compiles with Microsoft Visual Studio 2015 Community Edition.
You can download it for free from Microsoft's Web site.  Be sure to
install the Visual C++ compiler and the MFC libraries; these are not
installed by default.

Each program has Debug, Release, and Distribute targets.  The
Distribute target has some important functionality disabled,
therefore, it's recommended that you generally use the Release target.

Release binaries of WinProm and winelev are checked in at the top
level.

## File formats

WinProm takes as input .elv raster files.  The winelev program, also
included here, can convert from several common file formats to .elv. 

Here is an example of how to find prominences with WinProm:

- Download some terrain data from http://dds.cr.usgs.gov/srtm/version2_1/SRTM1/

- Run winelev and convert the terrain data to a .elv file

- Run WinProm, open the .elv file via File | Open | Elevation Map,
  then find prominences via Operation | Analyze | Peaks.

## About prominence

There are many good articles about topographic prominence on the Web.
For an introduction, see

https://en.wikipedia.org/wiki/Topographic_prominence
http://www.surgent.net/highpoints/prominence.html

## About Edward Earl

For more about Edward, see http://peakbagger.com/climber/climber.aspx?cid=601.