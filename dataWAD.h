#ifndef DATAWAD_H
#define DATAWAD_H

// ****************************************************************************
// * Includes & Namespaces
// ****************************************************************************

#include <cstdint>                          // Contains fixed-width integer types.
#include <fstream>

using namespace std;

// ****************************************************************************
// * Structs
// ****************************************************************************

struct LumpData
{
    char        name[9];                    // Lump Name
    int32_t     offset;                     // Pointer to Referenced Data
    int32_t     size;                       // Lump Size (Bytes)
};

struct LineData
{
    int16_t     start;                      // Starting (X,Y) Coordinate (Vertex)
    int16_t     end;                        // Ending (X,Y) Coordinate (Vertex)
    int16_t     flags;
    int16_t     type;
    int16_t     tag;                        // Referenced LineDef/Sector
    int16_t     front;                      // "Front" SideDef
    int16_t     back;                       // "Back" SideDef
};

struct SideData
{
    int16_t     xOffset;                    // Texture Offset
    int16_t     yOffset;
    char        upper[9];                   // Name of Upper Texture
    char        lower[9];                   // Name of Lower Texture
    char        middle[9];                  // Name of Middle Texture
    int16_t     sector;                     // Sector ID that SideDef "Faces"
};

struct VrtxData
{
    int16_t     xPosition;
    int16_t     yPosition;
};

struct SctrData
{
    int16_t     hghtFloor;                  // Floor Height
    int16_t     hghtCeil;                   // Ceiling Height
    char        txtrFloor[9];               // Name of Floor Texture (Flat)
    char        txtrCeil[9];                // Name of Ceiling Texture (Flat)
    int16_t     light;                      // Light Level / Brightness
    int16_t     type;
    int16_t     tag;                        // Referenced LineDef
};
/*
struct MapEntity
{
    int16_t     xPosition;
    int16_t     yPosition;
    int16_t     heading;                    // E (0), NE (45), N (90), NW (135), W (180), SW (225), S (270), SE (315)
    int16_t     type;                       // Monster, Item, etc.
    int16_t     flags;                      // Existence per Skill Level or Multiplayer
};
*/
// ****************************************************************************
// * Classes
// ****************************************************************************

class MapData
{
    private:
        LumpData*   lmpOrigin;              // Points to (Current Map's) Origin Lump
        LumpData*   hdrLines;               // Points to (Current Map's) Line Definitions Header (in Lump Directory)
        LumpData*   hdrSides;               // Points to (Current Map's) Side Definitions Header (in Lump Directory)
        LumpData*   hdrVrtcs;               // Points to (Current Map's) Vertices Header (in Lump Directory)
        LumpData*   hdrSctrs;               // Points to (Current Map's) Sectors Header (in Lump Directory)

        int         cntLines;               // (Current Map's) Number of Line Definitions
        int         cntSides;               // (Current Map's) Number of Side Definitions
        int         cntVrtcs;               // (Current Map's) Number of Vertices
        int         cntSctrs;               // (Current Map's) Number of Sectors

        LineData*   lineDef;                // Points to (Current Map's) Line Definition Data Array
        SideData*   sideDef;                // Points to (Current Map's) Side Definition Data Array
        VrtxData*   vertex;                 // Points to (Current Map's) Vertex Data Array
        SctrData*   sector;                 // Points to (Current Map's) Sector Data Array

    public:
        void setAll(fstream*, LumpData*);
        void setPointers(LumpData*);
        void setDataSize();
        void setLineData(fstream*);
        void setSideData(fstream*);
        void setVrtxData(fstream*);
        void setSctrData(fstream*);

        void printName();
        void printLineData();
        void printSideData();
        void printVrtxData();
        void printSctrData();

        ~MapData()
        {
            delete [] lineDef;
            delete [] sideDef;
            delete [] vertex;
            delete [] sector;
        }
};

class DataWAD
{
    private:
        char        type[5];                // Internal WAD / Patch WAD
        int         cntMaps;
        uint32_t    cntLumps;               // Number of Lumps
        uint32_t    dirOffset;              // Pointer to Lump Directory
        LumpData*   lump;
        MapData*    map;

    public:
        void setAll(fstream*);
        void setAttributes(fstream*);
        void setDirectory(fstream*);
        void setMapData(fstream*);

        bool isMapName(char*);

        void printAttributes();
        void printDirectory();
        void printMapList();
        void printMapLineData(int);
        void printMapSideData(int);
        void printMapVrtxData(int);
        void printMapSctrData(int);

        ~DataWAD()
        {
            delete [] lump;
            delete [] map;
        }
};

#endif // DATAWAD_H
