// Vertex Array
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <list>
#include <Math/Math.h>
#include <Geometry/FaceSet.h>
#include <Logging/Logger.h>
#include <Resources/ITextureResource.h>

namespace OpenEngine {
namespace Scene {

using namespace OpenEngine::Logging;
using OpenEngine::Geometry::FaceSet;
    //using namespace OpenEngine::Resources;

class VertexArray {
private:
    float* pVertices; // Pointer to vertex array
    float* pNormals; // Pointer to normal array
    float* pColors; // Pointer to color array
    float* pTexCoords; // Pointer to texture coordinate array

    int numFaces;

    void Init();
public:
    VertexArray();
    VertexArray(FaceSet& faces);
    virtual ~VertexArray();

   OpenEngine::Resources::ITextureResourcePtr texr; //!< texture resource

    float* GetVertices();
    float* GetNormals();
    float* GetColors();
    float* GetTexCoords();

    int GetNumFaces();
};

} // NS Geometry
} // NS OpenEngine


#endif // _VERTEX_ARRAY_H_
