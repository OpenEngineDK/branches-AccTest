
ADD_LIBRARY(Extensions_AccelerationStructures
  # quad stuff
  Scene/QuadNode.cpp
  Scene/QuadTransformer.cpp
  # bsp stuff
  Scene/BSPNode.cpp
  Scene/BSPTransformer.cpp
  # vertex array stuff
  Scene/VertexArray.cpp
  Scene/VertexArrayNode.cpp
  Scene/VertexArrayTransformer.cpp
  # other things
  Scene/ASDotVisitor.cpp
  Renderers/AcceleratedRenderingView.cpp
  Renderers/TextureLoader.cpp
  Renderers/Renderer.cpp
)

TARGET_LINK_LIBRARIES(Extensions_AccelerationStructures
  OpenEngine_Renderers
  OpenEngine_Scene
)
