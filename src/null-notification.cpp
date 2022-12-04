#include <null-notification.h>

namespace null::notification {
	void end_frame() {
		std::chrono::steady_clock::time_point current_time{ std::chrono::steady_clock::now() };

		for(auto& [layout, notifications] : interfaces::i_notification::notifications) {
			auto remove{ std::ranges::remove_if(notifications, [&](std::shared_ptr<interfaces::i_notification>& notification) { return notification->can_remove(current_time); }) };
			notifications.erase(remove.begin(), remove.end());

			for(auto iterator : std::views::iota(notifications.begin(), notifications.end())) {
				std::shared_ptr<interfaces::i_notification>& notification{ *iterator };
				notification->handle_animation(iterator == notifications.begin() ? nullptr : (*std::prev(iterator)).get(), current_time);
				notification->draw();
			}
		}
	}
}