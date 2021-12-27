#include "3DRenderingExample.h"
#include "RenderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
using namespace glm;

/// <summary>
/// Environment for learning and testing 3D rendering with OpenGL. Not to actually be used in the finished product.
/// </summary>
/// <param name="game_manager">: The GameManager instance that gets passed around everywhere.</param>
void three_d_rendering_main(GameManager* game_manager) {
	PlayerInfo* player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	bool three_deeing = true;

	//Get rid of our SDL stuff for now

	SDL_RenderClear(g_renderer);
	SDL_RenderPresent(g_renderer);

	//The shader files are just code that tells the GPU what to do with the vertices. I wrote a bunch of these but the most versatile ones are
	//the ones used in the shader constructor. You need one vertex shader and one fragment shader, and the constructor will look for whatever you 
	//give it within resource/shaders/
	Shader shader("vertex_model_col.glsl", "fragment_tex_col_blend.glsl");

	//Each of these represents the face of a cube. We're gonna pull this info directly from model files in the future.

	//The shader files can tell what they're looking at, there IS a Vertex struct that organizes these better but I copy/pasted this vertex
	//info and didn't feel like adjusting the format

	float vertices[] = {
		//Positions				//Colors (RGB)		//Tex Coords
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f
	};

	//Creating multiple cubes, these are just offsets for each one.
	vec3 cubePositions[] = {
	vec3(0.0f,  0.0f,  0.0f),
	vec3(2.0f,  5.0f, -15.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f, -0.4f, -3.5f),
	vec3(-1.7f,  3.0f, -7.5f),
	vec3(1.3f, -2.0f, -2.5f),
	vec3(1.5f,  2.0f, -2.5f),
	vec3(1.5f,  0.2f, -1.5f),
	vec3(-1.3f,  1.0f, -1.5f)
	};

	//Basically a rendering context for vertices. This is what the vertices get rendered to.
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Also a context for vertices but this is what keeps track of them.
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Helps the GPU understand which part of that vertices array corresponds to what.

	//I can't add comments to the shader code because it is interpreted as raw text, call me if you can't figure that part out.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //The part of each vertex that corresponds to position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //The part of each vertex that corresponds to color
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //The part of each vertex that corresponds to tex coords
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //Make sure none of the vertex arrays are currently bound

	//A bunch of textures, I believe loaded into the current VBO

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);	
	//Some rules to apply to the textures like what to do when they wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Side
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Top
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Whether or not to use mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width;
	int height;
	int num_channels;
	stbi_set_flip_vertically_on_load(true); //STBI loads textures upside down, this just fixes that
	unsigned char* data = stbi_load("resource/chara/roy/sprite/container.jpg", &width, &height, &num_channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //Important to specify RGB for jpgs, RGBA for pngs
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load OpenGL texture!" << endl;
	}
	stbi_image_free(data); //Basically just the equivalent of SDL textures, idt I need to explain this part

	//Repeat with another texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("resource/chara/roy/sprite/sprite.png", &width, &height, &num_channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load OpenGL texture!" << endl;
	}
	stbi_image_free(data);

	shader.use(); //Set that shader from earlier to the active one
	shader.set_int("texture1", 0); //"texture1" and "texture2" are variable names in the fragment shader, and the 0 and 1 specify which actual textures
	shader.set_int("texture2", 1); //that will be bound in a sec to use

	Camera camera; //No constructor needed

	while (three_deeing) {
		frameTimeDelay();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0); //Set texture0 to the target texture
		glBindTexture(GL_TEXTURE_2D, texture1); //Bind "texture1" (the crate texture) to it
		glActiveTexture(GL_TEXTURE1); //Repeat for the Ryu texture
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use(); //If we're gonna be bouncing between shaders a lot, then we need to switch the active shader to whatever is relevant, it all depends.
		

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}

		for (int i = 0; i < 2; i++) {
			if (player_info[i]->check_button_on(BUTTON_MENU_BACK)) {
				three_deeing = false;
			}
		}
		if (player_info[0]->check_button_on(BUTTON_UP)) { //Move the camera forward
			camera.add_pos(0.0, 0.0, -1.0);
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			camera.add_pos(1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			camera.add_pos(-1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			camera.add_pos(0.0, 0.0, 1.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LP)) { //Zoom the camera in
			camera.adjust_view(0.0, 0.0, -1.0);
		}
		if (player_info[0]->check_button_on(BUTTON_HP)) { //Out
			camera.adjust_view(0.0, 0.0, 1.0);
		}
		if (player_info[1]->check_button_on(BUTTON_UP)) { //Rotate the camera up
			camera.adjust_view(0.0, 1.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_RIGHT)) { //Right
			camera.adjust_view(1.0, 0.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_LEFT)) { //Left
			camera.adjust_view(-1.0, 0.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) { //Down
			camera.adjust_view(0.0, -1.0, 0.0);
		}

		camera.update_view(); //Update the camera view
		mat4 view = camera.get_view();
		mat4 projection = perspective(radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

		//setting variables within the vertex shaders 

		shader.set_mat4("view", view);
		shader.set_mat4("projection", projection);

		glBindVertexArray(VAO); //Setting the current vertex array
		for (unsigned int i = 0; i < 10; i++) {
			mat4 model = mat4(1.0f); //Setting up each of the cubes
			model = translate(model, cubePositions[i]); //Translating them based on those offsets from earlier
			float angle = 20.0f * i; //Cool rotation
			model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f)); //Rotate
			model = rotate(model, (float)(SDL_GetTicks() / 1000.0) * radians(50.0f), vec3(0.5f, 1.0f, 0.0f)); //ROTATE AGAIN!!
			shader.set_mat4("model", model); //Set another vertex shader var

			glDrawArrays(GL_TRIANGLES, 0, 36); //Draw vertices 0-36 from the current VAO and VBO (the faces)
		}

		SDL_GL_SwapWindow(g_window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}