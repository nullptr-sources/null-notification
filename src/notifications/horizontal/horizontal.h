#pragma once
#include <interfaces/default-animation.h>
#include <interfaces/default-notification.h>

namespace null::notification {
	class c_horizontal : public virtual interfaces::i_notification, public interfaces::i_default_animation, public interfaces::i_default_notification {
	public:
		struct style_t {
			static inline const color_t<float> background_color{ 0.1f, 0.1f, 0.1f, 0.4f };

			static inline const vec2_t text_offset{ 5.f, 4.f };
			static inline const float body_offset{ 7.f };

			static inline const float rounding{ 5.f };

			static inline const float remaining_time_line_size{ 2.f };
		};

	public:
		c_horizontal() { }
		c_horizontal(std::string_view _name, e_layout _layout, const std::chrono::seconds& show_duration, const color_t<int>& remaining_time_line_color, const render::multicolor_text_t<std::string>& _prefix, const render::multicolor_text_t<std::string>& _body) : i_notification{ _name, _layout, show_duration }, i_default_notification{ remaining_time_line_color, _prefix, _body } { }

	public:
		virtual void update() override;

		virtual void handle_animation(i_notification* prev, const std::chrono::steady_clock::time_point& current_time) override;
		virtual void draw() override;

		void copy_from(const c_horizontal& src) { i_default_notification::copy_from(src); i_notification::copy_from(src); }
	};
}