#ifndef ZODIA_GAME_GENERALSCENE_H_
#define ZODIA_GAME_GENERALSCENE_H_

#include <string>

#include "scene/gamescene.h"
#include "scene/scene.h"
#include "script/host.h"

class GeneralScene : public GameSceneInterface {
 public:
  GeneralScene(ScriptHost &host, int layer_count) : host_(host) {
    scene_ = std::make_shared<Scene>(layer_count);
  }

  // event handlers
  void Reset() override {
    if (!reset_handler_.empty()) host_.CallFunction(reset_handler_);
  }

  void FrameBegin(KeyStatus status) override {
    if (!begin_handler_.empty()) host_.CallFunction(begin_handler_);
  }

  const ScenePtr &GetScene() const override { return scene_; }

  // setters
  void set_reset_handler(const std::string &name) {
    reset_handler_ = name;
  }
  void set_begin_handler(const std::string &name) {
    begin_handler_ = name;
  }

 private:
  ScriptHost &host_;
  ScenePtr scene_;
  std::string reset_handler_, begin_handler_;
};

#endif  // ZODIA_GAME_GENERALSCENE_H_
