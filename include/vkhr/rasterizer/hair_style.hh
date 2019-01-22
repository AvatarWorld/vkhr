#ifndef VKHR_VULKAN_HAIR_STYLE_HH
#define VKHR_VULKAN_HAIR_STYLE_HH

#include <vkhr/scene_graph/hair_style.hh>

#include <vkhr/rasterizer/pipeline.hh>
#include <vkhr/rasterizer/drawable.hh>

#include <vkhr/scene_graph/camera.hh>

#include <vkpp/buffer.hh>
#include <vkpp/command_buffer.hh>
#include <vkpp/descriptor_set.hh>
#include <vkpp/pipeline.hh>

namespace vk = vkpp;

namespace vkhr {
    class Rasterizer;
    namespace vulkan {
        class HairStyle final : public Drawable {
        public:
            HairStyle(const vkhr::HairStyle& hair_style,
                      vkhr::Rasterizer& vulkan_renderer);

            HairStyle() = default;

            void load(const vkhr::HairStyle& hair_style,
                      vkhr::Rasterizer& scene_renderer);

            void voxelize_vertices(Pipeline& pipeline, vk::DescriptorSet& descriptor_set, vk::CommandBuffer& command_buffer);

            void draw(Pipeline& vulkan_strand_rasterizer_pipeline,
                      vk::DescriptorSet& descriptor_set,
                      vk::CommandBuffer& command_buffer) override;

            static void build_pipeline(Pipeline& pipeline_reference, Rasterizer& vulkan_renderer);
            static void depth_pipeline(Pipeline& pipeline_reference, Rasterizer& vulkan_renderer);
            static void voxel_pipeline(Pipeline& pipeline_reference, Rasterizer& vulkan_renderer);

            vk::UniformBuffer& get_parameter();

            vk::DeviceImage& get_volume();
            vk::Sampler&     get_volume_sampler();
            vk::ImageView&   get_volume_view();

        private:
            vk::IndexBuffer  segments;
            vk::VertexBuffer vertices;
            vk::VertexBuffer tangents;

            struct Parameters {
                AABB volume_bounds;
                glm::vec3 volume_resolution;
                float strand_radius;
                glm::vec3 hair_color;
                float hair_shininess;
            } parameters;

            vk::UniformBuffer parameter_buffer;

            vk::ImageView density_view;
            vk::DeviceImage density_volume;
            vk::Sampler density_sampler;

            static int id;
        };
    }
}

#endif