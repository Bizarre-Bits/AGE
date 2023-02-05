//
// Created by alex on 2/5/23.
//

#ifndef AGE_EDITORLAYER_H
#define AGE_EDITORLAYER_H

#include <Age/Core/Layer.h>

namespace AGE {

  class EditorLayer : public Layer {
  public:
    virtual void OnUpdate(Timestep ts) override;
  };

} // AGE

#endif //AGE_EDITORLAYER_H
