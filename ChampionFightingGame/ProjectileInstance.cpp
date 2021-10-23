#include "ProjectileInstance.h"

void ProjectileInstance::load_anim_list(SDL_Renderer* renderer)
{
	ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail())
	{
		cerr << "Could not open anim_list!" << endl;
		exit(1);
	}

	string line_1;
	anim_list >> line_1;
	int num_anims = ymlChopInt(line_1);

	for (int i = 0; i < num_anims; i++) {
		string name;
		string path;
		string frame_count;
		anim_list >> name >> path >> frame_count;
		animation_table[i].name = ymlChopString(name);
		animation_table[i].path = (resource_dir + "/anims/" + ymlChopString(path));
		animation_table[i].length = ymlChopInt(frame_count) - 1;
		loadAnimation(&animation_table[i], renderer);
	}
	anim_list.close();
}