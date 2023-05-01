/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   swSlide0000_png;
    const int            swSlide0000_pngSize = 2126;

    extern const char*   swSlide0001_png;
    const int            swSlide0001_pngSize = 2162;

    extern const char*   FuzzboyBG_png;
    const int            FuzzboyBG_pngSize = 1051036;

    extern const char*   MiniBrute_png;
    const int            MiniBrute_pngSize = 837305;

    extern const char*   Toggle_png;
    const int            Toggle_pngSize = 47714;

    extern const char*   Toggle1_png;
    const int            Toggle1_pngSize = 47457;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
