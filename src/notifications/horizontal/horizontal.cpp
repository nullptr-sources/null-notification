#include <notifications/horizontal/horizontal.h>

namespace null::notification {
	void c_horizontal::update() {
		i_default_notification::update();

		size = vec2_t{ prefix_size.x + body_size.x, std::max(prefix_size.y, body_size.y) } + style_t::text_offset * 2 + vec2_t{ style_t::body_offset * 2.f + style_t::remaining_time_line_size, 0.f };

		i_notification::update();
	}

	void c_horizontal::handle_animation(i_notification* prev, const std::chrono::steady_clock::time_point& current_time) {
		i_default_animation::handle_animation(prev, current_time);

		body.transform([&](std::string&, color_t<int>& color) { color.a() = animation.alpha * 255; });
		prefix.transform([&](std::string&, color_t<int>& color) { color.a() = animation.alpha * 255; });
	}

	void c_horizontal::draw() {
		render::foreground.draw_rect_filled(pos, pos + size, { style_t::background_color, style_t::background_color.a() * animation.alpha }, style_t::rounding);

		vec2_t remaining_time_line_offset{ style_t::text_offset + vec2_t{ i_default_notification::prefix_size.x + style_t::body_offset, 0.f } };
		rect_t remaining_time_line_rect{ pos + remaining_time_line_offset, vec2_t{ style_t::remaining_time_line_size, size.y - style_t::text_offset.y * 2.f }, rect_t::top | rect_t::left };
		render::foreground.draw_rect_filled(remaining_time_line_rect, { color_t<int>::palette_t::white, animation.alpha * 0.2f });
		render::foreground.draw_rect_filled(remaining_time_line_rect + rect_t{ vec2_t{ 0.f, remaining_time_line_rect.size().y * std::min(1.f - animation.life_time_pocent, 1.f) }, vec2_t{ } }, { color_t<int>::palette_t::white, animation.alpha });

		render::foreground.draw_text(prefix, pos + style_t::text_offset);
		render::foreground.draw_text(body, vec2_t{ remaining_time_line_rect.max.x + style_t::body_offset, pos.y + style_t::text_offset.y });
	}
}