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
    if (!reset_.empty()) host_.CallResetHandler(reset_);
  }

  void FrameBegin(KeyStatus key) override {
    if (!begin_.empty()) host_.CallBeginHandler(begin_, key);
  }

  const ScenePtr &GetScene() const override { return scene_; }

  // setters
  void set_reset_handler(const std::string &name) { reset_ = name; }
  void set_begin_handler(const std::string &name) { begin_ = name; }

 private:
  ScriptHost &host_;
  ScenePtr scene_;
  std::string reset_, begin_;
};

#endif  // ZODIA_GAME_GENERALSCENE_H_
