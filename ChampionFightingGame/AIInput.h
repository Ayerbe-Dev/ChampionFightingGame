#pragma once

class AIInput {
public:
	AIInput();
	AIInput(int execution, int reaction);

	void init(int execution, int reaction);

	int execution;
	int reaction;
};