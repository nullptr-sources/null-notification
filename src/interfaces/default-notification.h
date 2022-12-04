#pragma once
#include <interfaces/notification.h>

namespace null::notification::interfaces {
	class i_default_notification : private virtual i_notification {
	public:
		struct style_t {
			color_t<int> remaining_time_line_color{ color_t<int>::palette_t::white };
		} style{ };

	public:
		vec2_t prefix_size{ }, body_size{ };
		render::multicolor_text_t<std::string> prefix{ }, body{ };

	public:
		i_default_notification() { }
		i_default_notification(const color_t<int>& remaining_time_line_color, const render::multicolor_text_t<std::string>& _prefix, const render::multicolor_text_t<std::string>& _body) : style{ remaining_time_line_color }, prefix { _prefix }, body{ body } { }

	public:
		virtual void update() override {
			prefix_size = render::c_font::get_current_font()->calc_text_size(prefix);
			body_size = render::c_font::get_current_font()->calc_text_size(body);
		}

		void copy_from(const i_default_notification& src) { style = src.style; prefix = src.prefix; body = src.body; }

	public: //@note: builder functions
		template <typename self_t> auto set_prefix(this self_t&& self, const std::string& _prefix) { return self.set_prefix({ _prefix, color_t<int>::palette_t::white }); }
		template <typename self_t> auto set_prefix(this self_t&& self, const render::multicolor_text_t<std::string>& _prefix) { self.prefix = _prefix; return self; }

		template <typename self_t> auto set_body(this self_t&& self, const std::string& _body) { return self.set_body({ _body, color_t<int>::palette_t::white }); }
		template <typename self_t> auto set_body(this self_t&& self, const render::multicolor_text_t<std::string>& _body) { self.body = _body; return self; }

		template <typename self_t> auto set_remaining_time_line_color(this self_t&& self, const color_t<int>& remaining_time_line_color) { self.style.remaining_time_line_color = remaining_time_line_color; return self; }
	};
}