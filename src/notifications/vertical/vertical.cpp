#include <notifications/vertical/vertical.h>

namespace null::notification {
	void c_vertical::update() {
		i_default_notification::update();

		size = vec2_t{ std::max(prefix_size.x, body_size.x), prefix_size.y + body_size.y } + style_t::text_offset * 2.f + vec2_t{ 0.f, style_t::body_offset * 2.f + style_t::remaining_time_line_size };

		i_notification::update();
	}

	void c_vertical::handle_animation(i_notification* prev, const std::chrono::steady_clock::time_point& current_time) {
		i_default_animation::handle_animation(prev, current_time);

		body.transform([&](std::string&, color_t<int>& color) { color.a() = animation.alpha * 255; });
		prefix.transform([&](std::string&, color_t<int>& color) { color.a() = animation.alpha * 255; });
	}

	void c_vertical::draw() {
		render::foreground.draw_rect_filled(pos, pos + size, { style_t::background_color, style_t::background_color.a() * animation.alpha }, style_t::rounding);

		vec2_t remaining_time_line_offset{ style_t::text_offset + vec2_t{ 0.f, prefix_size.y + style_t::body_offset } };
		rect_t remaining_time_line_rect{ rect_t{ pos + remaining_time_line_offset}.from_min(vec2_t{ size.x - style_t::text_offset.x * 2, style_t::remaining_time_line_size }) };
		render::foreground.draw_rect_filled(remaining_time_line_rect, { style.remaining_time_line_color, 0.2f * animation.alpha });
		render::foreground.draw_rect_filled(remaining_time_line_rect - rect_t{ vec2_t{ }, vec2_t{ remaining_time_line_rect.size().x * std::min(1.f - animation.life_time_pocent, 1.f), 0.f } }, { style.remaining_time_line_color, animation.alpha });

		render::foreground.draw_text(prefix, pos + style_t::text_offset);
		render::foreground.draw_text(body, remaining_time_line_rect.min + vec2_t{ 0.f, style_t::body_offset });
	}
}