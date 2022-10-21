#include "progressBar.hpp"

	ProgressBar::ProgressBar(): progress(0), width(70), pos(0) { printBar(); }
	ProgressBar::ProgressBar(std::size_t w): progress(0), width(w), pos(0) { printBar(); }
	ProgressBar::~ProgressBar() { /*setProgress(100);*/ }

	void ProgressBar::setProgress(int i, int n) { setProgress((float)i / (float)n); }

	void ProgressBar::setProgress(float p) {
		progress = p;
		std::size_t newPos = p * width;
		if (newPos > pos) {
			pos = newPos;
			printBar();
		}
	}

	void ProgressBar::printBar() {
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