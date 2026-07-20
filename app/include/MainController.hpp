//
// Created by lola on 7/19/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <engine/platform/Input.hpp>
#include <engine/platform/PlatformEventObserver.hpp>

namespace  app {
class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    }
private:
    void initialize() override;
    bool loop() override;
    void draw() override;
    void draw_tree();
    //void update() override;
    //void update_camera();
};
};



#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
