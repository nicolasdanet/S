/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== add_white_24dp.svg ==================
static const unsigned char temp_binary_data_0[] =
"<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"24px\" viewBox=\"0 0 24 24\" width=\"24px\" fill=\"#FFFFFF\"><path d=\"M0 0h24v24H0V0z\" fill=\"none\"/><path d=\"M19 13h-6v6h-2v-6H5v-2h6V5h2v6h6v2z\"/></svg>";

const char* add_white_24dp_svg = (const char*) temp_binary_data_0;

//================== clear_white_24dp.svg ==================
static const unsigned char temp_binary_data_1[] =
"<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"24px\" viewBox=\"0 0 24 24\" width=\"24px\" fill=\"#FFFFFF\"><path d=\"M0 0h24v24H0V0z\" fill=\"none\"/><path d=\"M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19"
" 17.59 13.41 12 19 6.41z\"/></svg>";

const char* clear_white_24dp_svg = (const char*) temp_binary_data_1;

//================== remove_white_24dp.svg ==================
static const unsigned char temp_binary_data_2[] =
"<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"24px\" viewBox=\"0 0 24 24\" width=\"24px\" fill=\"#FFFFFF\"><path d=\"M0 0h24v24H0V0z\" fill=\"none\"/><path d=\"M19 13H5v-2h14v2z\"/></svg>";

const char* remove_white_24dp_svg = (const char*) temp_binary_data_2;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x15f323a7:  numBytes = 194; return add_white_24dp_svg;
        case 0xe3ae587b:  numBytes = 268; return clear_white_24dp_svg;
        case 0xf3f38164:  numBytes = 176; return remove_white_24dp_svg;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "add_white_24dp_svg",
    "clear_white_24dp_svg",
    "remove_white_24dp_svg"
};

const char* originalFilenames[] =
{
    "add_white_24dp.svg",
    "clear_white_24dp.svg",
    "remove_white_24dp.svg"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
