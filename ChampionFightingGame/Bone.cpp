#include "Bone.h"

void offset_base_bone(Bone* base, Bone* offset) {
	offset->pos -= base->pos;
	offset->rot -= base->rot;
	offset->scale -= base->scale;
}