#ifndef PROGRESS_BAR
# define PROGRESS_BAR

#include <iostream>

class ProgressBar {
public:
	ProgressBar(std::size_t w = 70): progress(0), width(w), pos(0) { printBar(); }
	~ProgressBar() { /*setProgress(100);*/ }

	void setProgress(int i, int n) { setProgress((float)i / (float)n); }

	void setProgress(float p) {
		progress = p;
		std::size_t newPos = p * width;
		if (newPos > pos) {
			pos = newPos;
			printBar();
		}
	}

private:
	float progress;
	const std::size_t width;
	std::size_t pos;

/*

//void progressBar(int width, int width * progress, int(int)progress * 100.0) {
//void progressBar(int barWidth, int pos, int prtg) {

*/

	void printBar() {
		std::cerr << " %\r" << "[";
		//int pos = barWidth * progress;
		for (int i = 0; i < width; ++i) {
			if (i < pos) std::cerr << "=";
			else if (i == pos) std::cerr << ">";
			else std::cerr << " ";
		}
		if (progress < 100) std::cerr << "] " << int(progress * 100.0);
		//std::cerr << "] " << int(progress * 100.0);
		std::cerr.flush();
	}
};

#endif