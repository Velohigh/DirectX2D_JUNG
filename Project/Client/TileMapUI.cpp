#include "pch.h"
#include "TileMapUI.h"


TileMapUI::TileMapUI()
    : ComponentUI("TileMap", COMPONENT_TYPE::TILEMAP)
{
}

TileMapUI::~TileMapUI()
{
}

int TileMapUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;




    return TRUE;
}