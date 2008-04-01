// Program name and description
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------


#ifndef _VERTEX_ARRAY_TRANSFORMER_H_
#define _VERTEX_ARRAY_TRANSFORMER_H_

#include <Scene/ISceneNodeVisitor.h>

namespace OpenEngine {
namespace Scene {

class VertexArrayTransformer : public ISceneNodeVisitor{
public:
    VertexArrayTransformer();
    ~VertexArrayTransformer();

    void Transform(ISceneNode& node);
    void VisitGeometryNode(GeometryNode* node);

};

} // NS Scene
} // NS OpenEngine


#endif //_VERTEX_ARRAY_TRANSFORMER_H_
