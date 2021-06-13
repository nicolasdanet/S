/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   add_svg;
    const int            add_svgSize = 194;

    extern const char*   delete_forever_svg;
    const int            delete_forever_svgSize = 368;

    extern const char*   mail_svg;
    const int            mail_svgSize = 277;

    extern const char*   place_svg;
    const int            place_svgSize = 386;

    extern const char*   remove_svg;
    const int            remove_svgSize = 176;

    extern const char*   sync_svg;
    const int            sync_svgSize = 386;

    extern const char*   system_update_alt_svg;
    const int            system_update_alt_svgSize = 434;

    extern const char*   text_rotate_up_svg;
    const int            text_rotate_up_svgSize = 305;

    extern const char*   text_rotation_down_svg;
    const int            text_rotation_down_svgSize = 300;

    extern const char*   warning_amber_svg;
    const int            warning_amber_svgSize = 200;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 10;

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
