//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/Texture.h"

namespace AGE {
  class VulkanTexture2D : public Texture2D {
  public:
    VulkanTexture2D(const age_string_t& filepath);
    VulkanTexture2D(uint32_t width, uint32_t height);
    VulkanTexture2D();

    ~VulkanTexture2D() override = default;
    void SetData(void* data, uint32_t size) override;
    uint32_t Width() const override;
    uint32_t Height() const override;
    uint32_t ID() const override;
    void Bind(uint32_t slot) const override;
    void Unbind() const override;
    uint32_t Slot() const override;
    bool IsCorrect() const override;
    bool operator==(const Texture2D& other) const override;

    static Ref<Texture2D> ErrorTexture();
  };
} // AGE
