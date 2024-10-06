/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   bng_pdinfo;
    const int            bng_pdinfoSize = 492;

    extern const char*   icon_construction_svg;
    const int            icon_construction_svgSize = 441;

    extern const char*   icon_delete_svg;
    const int            icon_delete_svgSize = 489;

    extern const char*   icon_error_svg;
    const int            icon_error_svgSize = 518;

    extern const char*   icon_history_svg;
    const int            icon_history_svgSize = 425;

    extern const char*   icon_info_svg;
    const int            icon_info_svgSize = 516;

    extern const char*   icon_location_on_svg;
    const int            icon_location_on_svgSize = 426;

    extern const char*   icon_lock_svg;
    const int            icon_lock_svgSize = 480;

    extern const char*   icon_mail_svg;
    const int            icon_mail_svgSize = 309;

    extern const char*   icon_tune_svg;
    const int            icon_tune_svgSize = 293;

    extern const char*   icon_upload_svg;
    const int            icon_upload_svgSize = 224;

    extern const char*   icon_vertical_align_bottom_svg;
    const int            icon_vertical_align_bottom_svgSize = 195;

    extern const char*   icon_visibility_svg;
    const int            icon_visibility_svgSize = 535;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 13;

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
