#pragma once
#include <null-render.h>

namespace null::notification {
	enum class e_layout {
		top_left,
		top_center,
		top_right,
		bottom_left,
		bottom_center,
		bottom_right
	};

	struct layout_t {
	public:
		vec2_t screen_offset{ }, size_offset{ };

	public:
		layout_t() { }
		layout_t(const vec2_t& _screen_offset, const vec2_t& _size_offset, const vec2_t& border = 0.02f) : screen_offset{ math::abs(border - _size_offset) }, size_offset{ _size_offset } {}

	public:
		vec2_t get_start_position(const vec2_t& size) { return renderer::draw_data_t::screen_size * screen_offset - size * size_offset; }
		vec2_t direction() { return vec2_t{ 1.f } - size_offset * 2.f; }
	};

	static inline std::map<e_layout, layout_t> layouts {
		{ e_layout::top_left,		{ 0.f, 0.f }},
		{ e_layout::top_center,		{ { 0.5f, 0.f }, { 0.5f, 0.f }}},
		{ e_layout::top_right,		{ { 1.f, 0.0f }, { 1.f, 0.f }}},

		{ e_layout::bottom_left,	{ { 0.f, 1.f }, { 0.f, 1.f }}},
		{ e_layout::bottom_center,	{ { 0.5f, 1.f }, { 0.5f, 1.f }}},
		{ e_layout::bottom_right,	{ 1.f, 1.f, }},
	};

	namespace interfaces {
		class i_notification {
		public:
			static inline std::map<e_layout, std::vector<std::shared_ptr<i_notification>>> notifications{ };

			static i_notification* find(std::string_view name, e_layout layout) {
				if(name.empty()) return nullptr;
				if(auto finded{ std::ranges::find_if(notifications[layout], [&](std::shared_ptr<i_notification>& notification) { return notification->name == name; }) }; finded != notifications[layout].end())
					return (*finded).get();
				return nullptr;
			}

		public:
			struct style_t {
				static inline const float spacing{ 10.f };
			};

		public:
			std::string name{ };

			std::chrono::steady_clock::time_point created_time{ }, remove_time{ };
			std::chrono::milliseconds show_duration{ };

			vec2_t pos{ }, size{ };

			e_layout layout{ };

		public:
			i_notification() { }
			i_notification(std::string_view _name, e_layout _layout, const std::chrono::milliseconds& _show_duration) : name{ _name }, layout{ _layout }, show_duration{ _show_duration }, created_time{ std::chrono::steady_clock::now() } { }

		public:
			template <typename self_t>
			void add(this self_t&& self) {
				self.update();
				notifications[self.layout].push_back(std::make_shared<std::remove_cvref_t<self_t>>(self));
			}

			template <typename self_t, typename self_ptr_t = std::remove_cvref_t<self_t>*>
			void add_or_update(this self_t&& self) {
				if(self_ptr_t finded{ dynamic_cast<self_ptr_t>(find(self.name, self.layout)) }) {
					finded->copy_from(self);
				} else self.add();
			}

			virtual void update() { remove_time = std::chrono::steady_clock::now() + show_duration; pos = layouts[layout].get_start_position(size); }
			virtual void draw() { }
			virtual void handle_animation(i_notification* prev, const std::chrono::steady_clock::time_point& current_time) { }

			virtual bool can_remove(const std::chrono::steady_clock::time_point& current_time) { return current_time >= remove_time; }

			void copy_from(const i_notification& notification) { update(); }

		public: //@note: builder functions
			template <typename self_t> auto set_name(this self_t&& self, std::string_view _name) { self.name = _name; return self; }
			template <typename self_t> auto set_layout(this self_t&& self, e_layout _layout) { self.layout = _layout; return self; }
			template <typename self_t> auto set_show_duration(this self_t&& self, const std::chrono::seconds& _show_duration) { self.show_duration = _show_duration; return self; }
		};
	}
}