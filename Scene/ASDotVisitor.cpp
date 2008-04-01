// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Scene/ASDotVisitor.h>
#include <Scene/QuadNode.h>
#include <Scene/BSPNode.h>
#include <Scene/VertexArrayNode.h>
#include <Scene/VertexArray.h>

namespace OpenEngine {
namespace Scene {

void ASDotVisitor::VisitQuadNode(QuadNode* node) { 
    map<string,string> options;
    options["shape"] = "box";
    options["label"] = "Quad Node";

    // add this node
    int nid = GetId(node);
    dotdata << "{" << nid << " [";
    for (map<string,string>::iterator op = options.begin(); op != options.end(); op++)
        dotdata << op->first << "=\"" << op->second << "\" ";
    dotdata << "]}";

    // bind to sub nodes
    dotdata << " -> { ";
    if (node->GetTopLeft() != NULL)
        dotdata << GetId(node->GetTopLeft()) << "; ";
    if (node->GetTopRight() != NULL)
        dotdata << GetId(node->GetTopRight()) << "; ";
    if (node->GetBottomLeft() != NULL)
        dotdata << GetId(node->GetBottomLeft()) << "; ";
    if (node->GetBottomRight() != NULL)
        dotdata << GetId(node->GetBottomRight()) << "; ";
    for (list<ISceneNode*>::iterator n = node->subNodes.begin(); 
         n != node->subNodes.end(); n++) {
        dotdata << GetId(*n) << "; ";
    }    
    dotdata << "};\n";

    // visit sub nodes
    node->VisitSubNodes(*this);
}

void ASDotVisitor::VisitBSPNode(BSPNode* node) {    
    // statistics collector for bsp nodes.
    class BSPStatsVisitor : public ISceneNodeVisitor {
    public:
        int min;                // minimum observed depth
        int max;                // maximum observed depth
        int count;              // face count
        int weight;             // weight in terms of nodes
        float rel;              // balance relation
        BSPStatsVisitor() : min(0), max(0), count(0), weight(0), rel(1) {}
        void VisitBSPNode(BSPNode* node) {
            int   fmin = 0, bmin = 0;
            int   fmax = 1, bmax = 1;
            int   fwgh = 0, bwgh = 0;
            float frel = 1, brel = 1;
            if (node->GetFront() != NULL) {
                node->GetFront()->Accept(*this);
                fmin = min;
                fmax = max;
                frel = rel;
                fwgh = weight;
            } else fmin = 1;
            if (node->GetBack() != NULL) {
                node->GetBack()->Accept(*this);
                bmin = min;
                bmax = max;
                brel = rel;
                bwgh = weight;
            } else bmin = 1;
            if (fmin == 0 || bmin == 0) min = ((fmin>bmin) ? fmin : bmin) + 1;
            else min = ((fmin<bmin) ? fmin : bmin) + 1;
            max = ((fmax>bmax) ? fmax : bmax) + 1;
            rel = frel * brel + 0.1f;
            if (fwgh != 0 && bwgh != 0)
                rel *= (fwgh<bwgh) ? (float)fwgh/(float)bwgh : (float)bwgh/(float)fwgh;
            if (node->GetSpan() != NULL)
                count += 1 + node->GetSpan()->Size();
            weight = fwgh + bwgh + 1;
        }
    };
    // create sub visitor for statistics gathering.
    BSPStatsVisitor stats;
    stats.VisitBSPNode(node);
    // create the dot node
    ostringstream label;
    label << "BSP Tree\\n"
          << "Face count: "  << stats.count  << "\\n"
          << "Tree weight: " << stats.weight << "\\n"
          << "Relation: "    << stats.rel    << "\\n"
          << "Min depth: "   << stats.min    << "\\n"
          << "Max depth: "   << stats.max;
    map<string,string> options;
    options["shape"] = "triangle";
    options["label"] = label.str();

    //VisitNode((ISceneNode*)node, options);
    
    int nid = GetId(node);
    dotdata << "{" << nid << " [";
    for (map<string,string>::iterator op = options.begin(); op != options.end(); op++)
        dotdata << op->first << "=\"" << op->second << "\" ";
    dotdata << "]}";

//     // code for creating the full bsp tree
//     int name = GetId(node);
//     BSPNode* f = node->GetFront();
//     if (f != NULL) {
//         dotdata << name << " -> " << GetId(f) << ";\n";
//         f->Accept(*this);
//     }
//     BSPNode* b = node->GetBack();
//     if (b != NULL) {
//         dotdata << name << " -> " << GetId(b) << ";\n";
//         b->Accept(*this);
//     }

}

void ASDotVisitor::VisitVertexArrayNode(VertexArrayNode* node) {
    ostringstream label;
    list<VertexArray*> arrays = node->GetVertexArrays();

    label << "VertexArrays: " << arrays.size() << "\\n";
    int i=0;
    for(list<VertexArray*>::iterator itr = arrays.begin(); itr!=arrays.end(); itr++, i++ ){
        VertexArray* va = (*itr);
        label << "VA #" << i << " Faces: " << va->GetNumFaces() << "\\n";
    }
    VisitNode((ISceneNode*)node, label.str());
}

} // NS Scene
} // NS OpenEngine
