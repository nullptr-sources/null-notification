#pragma once
#include <interfaces/notification.h>

namespace null::notification::interfaces {
	class i_default_animation : private virtual i_notification {
	public:
		struct style_t {
			static inline const float hide_animation_speed{ 0.1f };
			static inline const float show_animation_speed{ 0.1f };
			static inline const float lerp_speed{ 0.2f };
		};

		struct animation_t {
		public:
			float alpha{ };
			float life_time_pocent{ };
		} animation{ };

	public:
		virtual void handle_animation(i_notification* prev, const std::chrono::steady_clock::time_point& current_time) override {
			if(i_notification::can_remove(current_time)) animation.alpha = std::max(animation.alpha - style_t::hide_animation_speed, 0.f);
			else animation.alpha = std::min(animation.alpha + style_t::show_animation_speed, 1.f);
			animation.life_time_pocent = std::chrono::duration<float>{ remove_time - current_time }.count() / std::chrono::duration<float>{ show_duration }.count();

			vec2_t start_pos{ layouts[layout].get_start_position(size) };
			vec2_t target_pos{ start_pos.x, prev ? prev->pos.y + (prev->size.y * (1.f - layouts[layout].size_offset.y) + i_notification::style_t::spacing + size.y * layouts[layout].size_offset.y) * layouts[layout].direction().y : start_pos.y };
			pos = math::round(vec2_t{ target_pos.x, math::lerp(pos.y, target_pos.y, layouts[layout].direction().y * (target_pos.y - pos.y) > 0.f ? 1.f : style_t::lerp_speed) });
		}

		virtual bool can_remove(const std::chrono::steady_clock::time_point& current_time) override { return i_notification::can_remove(current_time) && animation.alpha == 0.f; }
	};
}