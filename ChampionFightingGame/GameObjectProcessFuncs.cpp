#include "GameObject.h"
#include "WindowConstants.h"

void GameObject::process_render_pos() {
	render_pos = pos / scale_vec;
}