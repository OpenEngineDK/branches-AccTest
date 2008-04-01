// Vertex Array node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _VERTEX_ARRAY_NODE_H_
#define _VERTEX_ARRAY_NODE_H_

#include <Scene/SceneNode.h>

namespace OpenEngine {
namespace Scene {

class VertexArray;

/**
 * Vertex Array node.
 * Acts as a simple wrapper around a list of vertex arrays.
 *
 * @class VertexArrayNode VertexArrayNode.h Scene/VertexArrayNode.h
 */
class VertexArrayNode : public SceneNode {
private:
    list<VertexArray*> vaList;

public:
    /**
     * Default constructor.
     */
    VertexArrayNode();

    /**
     * Set constructor.
     *
     * @param faces content of this Vertex Array Node.
     */
    VertexArrayNode(VertexArray& vertexArray);

    /**
     * Destructor.
     */
    ~VertexArrayNode();

    /**
     * Get faces this Vertex Array Node contains.
     *
     * @return FaceSet pointer.
     */
    list<VertexArray*> GetVertexArrays();

    /**
     * Set FaceSet for this Vertex Array Node.
     *
     * @param faces FaceSet pointer.
     */
    void AddVertexArray(VertexArray& va);

    /**
     * Accept a visitor.
     *
     * @see ISceneNode::Accept
     */
    void Accept(ISceneNodeVisitor& visitor);

};

} // NS Scene
} // NS OpenEngine

#endif // _VERTEX_ARRAY_NODE_H_
