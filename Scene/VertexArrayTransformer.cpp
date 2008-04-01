// Program name and description
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Scene/VertexArrayTransformer.h>
#include <Scene/VertexArray.h>
#include <Scene/VertexArrayNode.h>
#include <Scene/GeometryNode.h>
#include <Geometry/FaceSet.h>
#include <Resources/ITextureResource.h>

namespace OpenEngine {
namespace Scene {

using OpenEngine::Geometry::FaceList;
using OpenEngine::Geometry::FacePtr;
using OpenEngine::Resources::ITextureResourcePtr;

VertexArrayTransformer::VertexArrayTransformer() {
}

VertexArrayTransformer::~VertexArrayTransformer() {
}

void VertexArrayTransformer::Transform(ISceneNode& node) {
    node.Accept(*this);
}

void VertexArrayTransformer::VisitGeometryNode(GeometryNode* node) {    

    // Create VertexArrayNode
    VertexArrayNode* vaNode = new VertexArrayNode();
    // Clone subNode pointers
    list<ISceneNode*>::iterator itr;
    for(itr = node->subNodes.begin(); itr!=node->subNodes.end(); itr++){
        vaNode->AddNode(*itr);
    }

    // Get the faceset
    FaceSet* faces = node->GetFaceSet();
    if( faces == NULL ) return;

    // Run through all faces and sort them by texture id   
    map<int, FaceSet*> facesetMap;
    for (FaceList::iterator itr = faces->begin(); itr != faces->end(); itr++) {
        // Get the face
        FacePtr f = (*itr);
        // Get the texture resource associated with the face
        ITextureResourcePtr texr = f->texr;
        // Cause the textures are not loaded at this point we compare texture 
        // resources by the pointer address. If the texture resource of two faces
        // points to the same texture resource in memory they must be the same even
        // though the texture is not loaded yet nor giving a texture id.
        unsigned int resPtrAddress = (unsigned int)&(*texr);
          
        FaceSet* curFaceSet;
        // Check if a faceset for faces with this texture resource already exists
        map<int, FaceSet*>::iterator texSetIter = facesetMap.find(resPtrAddress);
        if( texSetIter == facesetMap.end() ){
            // FaceSet for current texture resource is not found, so we create it.
            curFaceSet = new FaceSet();
            facesetMap[resPtrAddress] = curFaceSet; 
        }
        else {
            // A FaceSet with same texture resource already exists so set this as current.
            curFaceSet = texSetIter->second;
        }
        // Add the face to the correct faceset
        curFaceSet->Add(f);
    }

    logger.info << "Number of faceset created: " << facesetMap.size() << logger.end;
    

    // Now all faces has been sorted into FaceSets with same texture resource. 
    // Creating a vertex array for each face set and add the Vertex Array to this Vertex Array Node.
    for(map<int,FaceSet*>::iterator itr = facesetMap.begin(); itr != facesetMap.end(); itr++) {
        logger.info << "ResAdd: " << (*itr).first << logger.end;
        FaceSet* fset = (*itr).second;
        VertexArray* va = new VertexArray(*fset);
        // Add the VertexArray to the VertexArrayNode
        vaNode->AddVertexArray(*va);
    }
    // Replace existing node with newly created Vertex Array Node
    node->GetParent()->ReplaceNode(node,vaNode);
}
    

}
}

