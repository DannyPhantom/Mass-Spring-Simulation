#pragma once
class Spring
{
private:
	double k;
	bool isVisible;
	double x0;
	double damping;
public:
	Spring(double k, double x0, double damping, bool isVisible) : k(k), x0(x0), damping(damping), isVisible(isVisible) {}
	~Spring();

	void setK(double k) { this->k = k; }
	double getK() { return k; }

	void setX0(double x0) { this->x0 = x0; }
	double getX0() { return x0; }

	void setDamping(double damping) { this->damping = damping; }
	double getDamping() { return damping; }

	bool getIsVisible() { return isVisible; }
	void setVisible() { isVisible = true; }
};

