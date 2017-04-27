#pragma once
class mbrInfo
{
public:
	mbrInfo();
	~mbrInfo();
	int getMbrX_dec();
	void setMbrX_dec(int);
	int getMbrX_int();
	void setMbrX_int(int);

	int getMbrY_dec();
	void setMbrY_dec(int);
	int getMbrY_int();
	void setMbrY_int(int);

private:
	int mbrX_dec;
	int mbrY_dec;
	int mbrX_int;
	int mbrY_int;
};

