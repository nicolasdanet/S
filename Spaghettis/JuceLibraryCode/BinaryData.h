/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   info_bng_xml;
    const int            info_bng_xmlSize = 492;

    extern const char*   icon_add_svg;
    const int            icon_add_svgSize = 163;

    extern const char*   icon_construction_svg;
    const int            icon_construction_svgSize = 723;

    extern const char*   icon_delete_svg;
    const int            icon_delete_svgSize = 489;

    extern const char*   icon_error_outline_svg;
    const int            icon_error_outline_svgSize = 292;

    extern const char*   icon_info_svg;
    const int            icon_info_svgSize = 307;

    extern const char*   icon_lock_svg;
    const int            icon_lock_svgSize = 426;

    extern const char*   icon_mail_svg;
    const int            icon_mail_svgSize = 277;

    extern const char*   icon_place_svg;
    const int            icon_place_svgSize = 386;

    extern const char*   icon_restore_svg;
    const int            icon_restore_svgSize = 372;

    extern const char*   icon_sync_svg;
    const int            icon_sync_svgSize = 386;

    extern const char*   icon_system_update_alt_svg;
    const int            icon_system_update_alt_svgSize = 434;

    extern const char*   icon_text_rotate_up_svg;
    const int            icon_text_rotate_up_svgSize = 305;

    extern const char*   icon_text_rotation_down_svg;
    const int            icon_text_rotation_down_svgSize = 300;

    extern const char*   icon_tune_svg;
    const int            icon_tune_svgSize = 278;

    extern const char*   icon_upload_svg;
    const int            icon_upload_svgSize = 224;

    extern const char*   icon_visibility_svg;
    const int            icon_visibility_svgSize = 493;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 17;

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
