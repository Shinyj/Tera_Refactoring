#pragma once
class ProgressBar
{
private:
	RECT _rcProgress; // ü�� �� ��Ʈ
	int _x, _y; // ��ǥ
	int _width; // ü�¹� ���� ���� ( �����Ͽ� HP ǥ�� )

	cSprite * _progressBarFront; // ü�� �� �� �̹���
	cSprite * _progressBarBack; // ü�� �� �� �̹���
public:
	ProgressBar();
	virtual~ProgressBar();

	// ü�� �� �ʱ�ȭ ( �� �̹���, �� �̹���, x, y, ���α���, ���α���)
	void Setup(char * frontImage, char * backImage, float x,
		float y, int width, int height);

	void Update();
	void Render();

	// Progress bar ������ ����
	void SetGauge(float currentGauge, float maxGauge);
	// Progress bar ��ġ
	void SetX(int x) { _x = x; }
	void SetY(int y) { _y = y; }
	
};

