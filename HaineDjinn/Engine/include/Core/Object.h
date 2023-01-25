#pragma once

class Object
{
private:
	bool isEnable = true;

protected:

public:
	bool virtual IsActive();
	void virtual SetActive(bool value);

	void virtual OnDestroy() {}
	void virtual Destroy() {}
};