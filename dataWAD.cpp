// ****************************************************************************
// * Includes & Namespaces
// ****************************************************************************

#include "dataWAD.h"

#include <cstring>
#include <iomanip>
#include <iostream>

// ****************************************************************************
// * Class Methods (DataWAD)
// ****************************************************************************

void DataWAD::setAll(fstream *fileReference)
{
    setAttributes(fileReference);
    setDirectory(fileReference);
    setMapData(fileReference);
}

void DataWAD::setAttributes(fstream *fileReference)
{
    fileReference->read(type,4);
    type[4] = '\0';
    fileReference->read(reinterpret_cast<char*>(&cntLumps), 4);
    fileReference->read(reinterpret_cast<char*>(&dirOffset), 4);
}

void DataWAD::setDirectory(fstream *fileReference)
{
    lump    = new LumpData[cntLumps];
    cntMaps = 0;

    fileReference->seekg(dirOffset, ios::beg);

    for (int i = 0; i < cntLumps; i++)
    {
        fileReference->read(reinterpret_cast<char*>(&lump[i].offset), 4);
        fileReference->read(reinterpret_cast<char*>(&lump[i].size), 4);
        fileReference->read(lump[i].name,8);
        lump[i].name[8] = '\0';

        if ( isMapName(lump[i].name) )
            cntMaps++;
    }
}

void DataWAD::setMapData(fstream *fileReference)
{
    int mapIndex = 0;

    map = new MapData[cntMaps];

    for (int i = 0; i < cntLumps; i++)
        if ( isMapName( lump[i].name ) )
            map[mapIndex++].setAll( fileReference, &lump[i] );
}

bool DataWAD::isMapName(char* lmpName)
{
    if ( lmpName[0] == 'E' )                    // "ExMy" (Episode x, Mission y)
        if ( isdigit(lmpName[1]) )
            if ( lmpName[2] == 'M' )
                if ( isdigit(lmpName[3]) )
                    return 1;

    return 0;

}

void DataWAD::printAttributes()
{
    if ( strcmp( "IWAD", type ) == 0 )
        cout << "Internal WAD";
    else if ( strcmp( "PWAD", type ) == 0 )
        cout << "Patch WAD" << endl;
    else
        cout << "Unknown WAD";

    cout << " (" << cntLumps << " Lumps)" << endl;
    cout << "Directory List > " << dirOffset  << endl;
}

void DataWAD::printDirectory() {
    cout << setw(4)  << "#";
    cout << setw(11) << "Lump";
    cout << setw(15) << "Size";
    cout << setw(15) << "Offset";
    cout << endl;

    for ( int i=0; i < cntLumps; i++ )
    {
        cout << setw(4) << i;
        cout << setw(6);

        for ( int j = 0; j < 8; j++ )
            cout << lump[i].name[j];

        cout << setw(10) << lump[i].size << " bytes";
        cout << setw(13) << lump[i].offset << endl;
    }
}

void DataWAD::printMapList()
{
    for (int i=0; i < cntMaps; i++)
    {
        cout << setw(4) << i << " ";
        map[i].printName();
    }
}

void DataWAD::printMapLineData(int index)
{
    map[index].printLineData();
}

void DataWAD::printMapSideData(int index)
{
    map[index].printSideData();
}

void DataWAD::printMapVrtxData(int index)
{
    map[index].printVrtxData();
}

void DataWAD::printMapSctrData(int index)
{
    map[index].printSctrData();
}

// ****************************************************************************
// * Class Methods (MapData)
// ****************************************************************************

void MapData::setAll(fstream *fileReference, LumpData *lumpReference)
{
    setPointers(lumpReference);
    setDataSize();
    setLineData(fileReference);
    setSideData(fileReference);
    setVrtxData(fileReference);
    setSctrData(fileReference);
}

void MapData::setPointers(LumpData* lumpReference)
{
    lmpOrigin   = lumpReference;
    hdrLines    = lumpReference + 2;
    hdrSides    = lumpReference + 3;
    hdrVrtcs    = lumpReference + 4;
    hdrSctrs    = lumpReference + 8;
}

void MapData::setDataSize()
{
    cntLines = hdrLines->size / 14;
    cntSides = hdrSides->size / 30;
    cntVrtcs = hdrVrtcs->size / 4;
    cntSctrs = hdrSctrs->size / 26;
}

void MapData::setLineData(fstream* fileReference)
{
    lineDef = new LineData[cntLines];

    fileReference->seekg(hdrLines->offset, ios::beg);

    for (int i = 0; i < cntLines; i++)
    {
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].start), 2);
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].end), 2);
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].flags), 2);
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].type), 2);
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].tag), 2);
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].front), 2);
        fileReference->read(reinterpret_cast<char*>(&lineDef[i].back), 2);
    }
}

void MapData::setSideData(fstream* fileReference)
{
    sideDef = new SideData[cntSides];

    fileReference->seekg(hdrSides->offset, ios::beg);

    for (int i = 0; i < cntSides; i++)
    {
        fileReference->read(reinterpret_cast<char*>(&sideDef[i].xOffset), 2);
        fileReference->read(reinterpret_cast<char*>(&sideDef[i].yOffset), 2);
        fileReference->read(sideDef[i].upper, 8);
        sideDef[i].upper[8] = '\0';
        fileReference->read(sideDef[i].lower, 8);
        sideDef[i].lower[8] = '\0';
        fileReference->read(sideDef[i].middle, 8);
        sideDef[i].middle[8] = '\0';
        fileReference->read(reinterpret_cast<char*>(&sideDef[i].sector), 2);
    }
}

void MapData::setVrtxData(fstream* fileReference)
{
    vertex = new VrtxData[cntVrtcs];

    fileReference->seekg(hdrVrtcs->offset, ios::beg);

    for (int j = 0; j < cntVrtcs; j++)
    {
        fileReference->read(reinterpret_cast<char*>(&vertex[j].xPosition), 2);
        fileReference->read(reinterpret_cast<char*>(&vertex[j].yPosition), 2);
    }
}

void MapData::setSctrData(fstream* file)
{
    sector = new SctrData[cntSctrs];

    file->seekg(hdrSctrs->offset, ios::beg);

    for (int i = 0; i < cntSctrs; i++)
    {
        file->read(reinterpret_cast<char*>(&sector[i].hghtFloor), 2);
        file->read(reinterpret_cast<char*>(&sector[i].hghtCeil), 2);
        file->read(sector[i].txtrFloor, 8);
        sector[i].txtrFloor[8] = '\0';
        file->read(sector[i].txtrCeil, 8);
        sector[i].txtrCeil[8] = '\0';
        file->read(reinterpret_cast<char*>(&sector[i].light), 2);
        file->read(reinterpret_cast<char*>(&sector[i].type), 2);
        file->read(reinterpret_cast<char*>(&sector[i].tag), 2);
    }
}

void MapData::printName()
{
    cout << lmpOrigin->name << endl;
}

void MapData::printLineData()
{
    cout << setw(4)  << "#"
         << setw(24) << "Starting Coordinate"
         << setw(22) << "Ending Coordinate"
         << setw(10)  << "Flags"
         << setw(9)  << "Type"
         << setw(8)  << "Tag"
         << setw(20)  << "SideDef (Front)"
         << setw(19)  << "SideDef (Back)"
         << endl;

    for ( int i=0; i < cntLines; i++ )
        cout << setw(4)  << i
             << setw(17) << lineDef[i].start
             << setw(23) << lineDef[i].end
             << setw(16) << lineDef[i].flags
             << setw(9)  << lineDef[i].type
             << setw(8)  << lineDef[i].tag
             << setw(15) << lineDef[i].front
             << setw(19) << lineDef[i].back
             << endl;
}

void MapData::printSideData()
{
    cout << setw(4)  << "#"
         << setw(13) << "x Offset"
         << setw(13) << "y Offset"
         << setw(18) << "Upper Texture"
         << setw(18)  << "Lower Texture"
         << setw(19)  << "Middle Texture"
         << setw(11) << "Sector"
         << endl;

    for ( int i=0; i < cntSides; i++ )
        cout << setw(4)  << i
             << setw(11) << sideDef[i].xOffset
             << setw(13) << sideDef[i].yOffset
             << setw(18) << sideDef[i].upper
             << setw(18) << sideDef[i].lower
             << setw(18) << sideDef[i].middle
             << setw(14) << sideDef[i].sector
             << endl;
}

void MapData::printVrtxData()
{
    cout << setw(4)  << "#"
         << setw(15) << "x Position"
         << setw(15) << "y Position"
         << endl;

    for ( int i=0; i < cntVrtcs; i++ )
        cout << setw(4)  << i
             << setw(12) << vertex[i].xPosition
             << setw(15) << vertex[i].yPosition
             << endl;
}

void MapData::printSctrData()
{
    cout << setw(4)  << "#"
         << setw(17) << "Floor Height"
         << setw(19) << "Ceiling Height"
         << setw(18) << "Floor Texture"
         << setw(20) << "Ceiling Texture"
         << setw(15) << "Brightness"
         << setw(9)  << "Type"
         << setw(8)  << "Tag"
         << endl;

    for ( int i=0; i < cntSctrs; i++ )
        cout << setw(4)  << i
             << setw(13) << sector[i].hghtFloor
             << setw(18) << sector[i].hghtCeil
             << setw(21) << sector[i].txtrFloor
             << setw(19) << sector[i].txtrCeil
             << setw(15) << sector[i].light
             << setw(12) << sector[i].type
             << setw(8)  << sector[i].tag
             << endl;
}
