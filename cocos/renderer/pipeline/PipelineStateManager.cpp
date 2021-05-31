/****************************************************************************
 Copyright (c) 2020-2021 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#include "PipelineStateManager.h"
#include "gfx-base/GFXDevice.h"
#include "helper/SharedMemory.h"

namespace cc {
namespace pipeline {

unordered_map<uint, gfx::PipelineState *> PipelineStateManager::psoHashMap;

gfx::PipelineState *PipelineStateManager::getOrCreatePipelineState(const scene::Pass *  pass,
                                                                   gfx::Shader *        shader,
                                                                   gfx::InputAssembler *inputAssembler,
                                                                   gfx::RenderPass *    renderPass) {
    const auto passHash       = pass->getHash();
    const auto renderPassHash = renderPass->getHash();
    const auto iaHash         = inputAssembler->getAttributesHash();
    const auto shaderID       = shader->getID();
    const auto hash           = passHash ^ renderPassHash ^ iaHash ^ shaderID;

    auto *pso = psoHashMap[hash];
    if (!pso) {
        auto *pipelineLayout = pass->getPipelineLayout();

        pso = gfx::Device::getInstance()->createPipelineState({
            shader,
            pipelineLayout,
            renderPass,
            {inputAssembler->getAttributes()},
            *(pass->getRasterizerState()),
            *(pass->getDepthStencilState()),
            *(pass->getBlendState()),
            pass->getPrimitive(),
            pass->getDynamicState(),
        });

        psoHashMap[hash] = pso;
    }

    return pso;
}

void PipelineStateManager::destroyAll() {
    for (auto &pair : psoHashMap) {
        CC_SAFE_DESTROY(pair.second);
    }
    psoHashMap.clear();
}

} // namespace pipeline
} // namespace cc
