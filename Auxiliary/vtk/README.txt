VTK Interface Classes
---------------------

These classes are used to interface VTK to and from Insight.

Make sure that VTK is built with ANSI C++ in order to avoid
conflicts with the iostream library. We use the following
flags when building VTK (in Microsoft C++).

/D "VTK_USE_ANSI_STDLIB" /GX /Zm1000

