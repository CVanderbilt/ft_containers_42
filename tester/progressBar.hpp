#ifndef PROGRESS_BAR
# define PROGRESS_BAR

#include <iostream>

class ProgressBar {
public:
	ProgressBar();
	ProgressBar(std::size_t w);
	~ProgressBar();

	void setProgress(int i, int n);
	void setProgress(float p);

private:
	float progress;
	const std::size_t width;
	std::size_t pos;

	void printBar();
};

#endif