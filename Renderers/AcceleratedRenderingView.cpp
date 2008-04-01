// Accelerated rendering view.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Renderers/AcceleratedRenderingView.h>
#include <Meta/OpenGL.h>
#include <Scene/BSPNode.h>
#include <Scene/QuadNode.h>
#include <Scene/VertexArray.h>
#include <Scene/VertexArrayNode.h>
#include <Scene/TransformationNode.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Math/Math.h>

namespace OpenEngine {
namespace Renderers {

using namespace OpenEngine::Math;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers::OpenGL;

//! Rendering view constructor.
AcceleratedRenderingView::AcceleratedRenderingView(Viewport& viewport)
    : IRenderingView(viewport), renderer(NULL) {
    RenderStateNode* renderStateNode = new RenderStateNode();
    renderStateNode->AddOptions(RenderStateNode::RENDER_TEXTURES);
    renderStateNode->AddOptions(RenderStateNode::RENDER_SHADERS);
    renderStateNode->AddOptions(RenderStateNode::RENDER_BACKFACES);
    stateStack.push_back(renderStateNode);
}

AcceleratedRenderingView::~AcceleratedRenderingView() {
}

void AcceleratedRenderingView::VisitQuadNode(QuadNode* node) {
    if (viewport.GetViewingVolume()->IsVisible(node->GetBoundingBox()))
        node->VisitSubNodes(*this);
}

void AcceleratedRenderingView::VisitBSPNode(BSPNode* node) {
    node->VisitSubNodes(*this);
}

/**
 * Process a transformation node.
 *
 * @param node Transformation node to apply.
 */
void AcceleratedRenderingView::VisitTransformationNode(TransformationNode* node) {
    // push transformation matrix
    Matrix<4,4,float> m = node->GetTransformationMatrix();
    float f[16];
    m.ToArray(f);
    glPushMatrix();
    glMultMatrixf(f);
    // traverse sub nodes
    node->VisitSubNodes(*this);
    // pop transformation matrix
    glPopMatrix();
}

/**
 *   Process a Vertex Array Node which may contain a list of vertex arrays
 *   sorted by texture id.
 */
void AcceleratedRenderingView::VisitVertexArrayNode(VertexArrayNode* node){
    // Toggle wireframed rendering
    if( IsOptionSet(RenderStateNode::RENDER_WIREFRAMED) ) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);
    // Enable all client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    // Get vertex array from the vertex array node
    list<VertexArray*> vaList = node->GetVertexArrays();
    for(list<VertexArray*>::iterator itr = vaList.begin(); itr!=vaList.end(); itr++) {
        VertexArray* va = (*itr);
        glBindTexture(GL_TEXTURE_2D, va->texr->GetID()); 
        
        // Setup pointers to arrays
        glNormalPointer(GL_FLOAT, 0, va->GetNormals());
        glColorPointer(4, GL_FLOAT, 0, va->GetColors());
        glTexCoordPointer(2, GL_FLOAT, 0, va->GetTexCoords());
        glVertexPointer(3, GL_FLOAT, 0, va->GetVertices());
        glDrawArrays(GL_TRIANGLES, 0, va->GetNumFaces()*3);
    }

    // Disable all state changes
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

/**
 * Process a rendering node.
 *
 * @param node Rendering node to apply.
 */
void AcceleratedRenderingView::VisitRenderNode(IRenderNode* node) {
    node->Apply(this);
}

/**
 * Process a render state node.
 *
 * @param node Render state node to apply.
 */
void AcceleratedRenderingView::VisitRenderStateNode(RenderStateNode* node) {
    stateStack.push_back(node);
    node->VisitSubNodes(*this);
    stateStack.pop_back();
}

bool AcceleratedRenderingView::IsOptionSet(RenderStateNode::RenderStateOption o) {
    return stateStack.back()->IsOptionSet(o);
}


/**
 * Renderer the scene.
 *
 * @param renderer a Renderer
 * @param root The scene to be rendered
 */
void AcceleratedRenderingView::Render(IRenderer* renderer, ISceneNode* root) {
    this->renderer = renderer;
    root->Accept(*this);
    this->renderer = NULL;
}

/**
 * Get the renderer that the view is processing for.
 *
 * @return Current renderer, NULL if no renderer processing is active.
 */
IRenderer* AcceleratedRenderingView::GetRenderer() {
    return renderer;
}


} // NS Renderers
} // NS OpenEngine
