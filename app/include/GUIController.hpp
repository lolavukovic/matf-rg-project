//
// Created by lola on 7/21/26.
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_HPP
#define MATF_RG_PROJECT_GUICONTROLLER_HPP
#include "engine/core/Controller.hpp"

namespace app {
class GUIController : public engine::core::Controller{
    void initialize() override;
public:
    std::string_view name() const override {
        return "GUIController";
    }

private:
    void draw() override;

    void poll_events() override;

};
}


#endif//MATF_RG_PROJECT_GUICONTROLLER_HPP
