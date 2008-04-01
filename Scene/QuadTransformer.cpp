// Quad transformer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "QuadTransformer.h"

namespace OpenEngine {
namespace Scene {

    /**
     * Construct a quad transformor, that transforms geometry nodes to
     * quad nodes.
     */
    QuadTransformer::QuadTransformer() 
        : mCount(500), mHSize(100){
    }

    /**
     * Destructor.
     */
    QuadTransformer::~QuadTransformer(){

    }
    
    /**
     * Transforms the geometry nodes of a tree into quad trees.
     *
     * @pre The root of the scene to transform may not be of type GeometryNode.
     * @param node Root node of a scene to build from.
     */
    void QuadTransformer::Transform(ISceneNode& node){
        node.Accept(*this);
    }

    /**
     * Set the maximum amount of faces to be contained in a single quad
     * node.
     * The default is 100.
     *
     * @param count Maximum count.
     */
    void QuadTransformer::SetMaxFaceCount(const int count) {
        mCount = count;
    }

    /**
     * Maximum size the bounding square of a quad node may be.
     * The default is 10.
     *
     * @param size Maximum size of the quad box 
     */
    void QuadTransformer::SetMaxQuadSize(const float size) {
        mHSize = size / 2;
    }

    /**
     * Transform the encountered geometry node into a quad node.
     *
     * @param node Geometry node.
     */
    void QuadTransformer::VisitGeometryNode(GeometryNode *node){
        FaceSet *faces = node->GetFaceSet();
        if (faces->Size() != 0){
            QuadNode *quad = new QuadNode(faces, mCount, mHSize);
            node->GetParent()->ReplaceNode(node, quad);
        } else {
            node->GetParent()->DeleteNode(node);
        }
    }

} // NS Scene
} // NS OpenEngine
