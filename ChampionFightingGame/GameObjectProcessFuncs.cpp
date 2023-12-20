#include "GameObject.h"
#include "WindowConstants.h"

void GameObject::process_render_pos() {
	render_pos = pos / glm::vec3(
		WINDOW_WIDTH / (100 * scale.x),
		WINDOW_HEIGHT / (100 * scale.y),
		WINDOW_DEPTH / (100 * scale.z)
	);
}