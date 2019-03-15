#ifndef _PIPE_SECTION_HPP_
#define _PIPE_SECTION_HPP_

#include "interface.h"

template <class PointType, class NetType, class SectionType>
class PipeSection : public IMesh<PointType, NetType> {
private:

	SectionType face;
	// ����� ��������� �� ������� ��������, ���-�� ���������� �����������
	int n, l;
	
	size_t start_ind;		//���-�� ������ � �����������
	
	size_t coor_on_layer; // ���-�� ������ � �������

	// �����  ����� ����������� ������, �������� ����� �������, � �����������
	void circle_point(real &res_x, real &res_y,
		const real x1,const real y1,
		const real x2,const real y2,
		const real x0,const real y0,const real r) {
		if (x1 == x2) {
			res_x = x1;
			res_y = y0 + r;
		}
		if (y1 == y2) {
			res_x = x0 + r;
			res_y = y1;
		}
		else {
			real dx = x1 - x2;
			real dy = y1 - y2;
			real l = sqrt(dx*dx + dy * dy);
			real c = dx / l;
			real s = dy / l;
			real x = x0 + c * r;
			real y = y0 + s * r;
			res_x = x;
			res_y = y;
		}
	};

	//������� �������� ��������� �� ����
	vector<NetType> nvtrTubeOnly() {
		//������� ������
		int i, j, material = 0;
		int a, b, c, d;

		vector<NetType> nv;
		//����� �� �����������
		for (int k = 0; k < 4 * n; k++)
			for (i = 0; i <= l; i++)
			{
				if (i == l) material = OIL; else material = IRON;
				//������� ����� � �������
				if (k == 4 * n - 1) {
					a = (l + 2)*k + i;
					b = (l + 2)*k + i + 1;
					c = i;
					d = i + 1;
					NetType A(c, a, d, b, material);
					nv.push_back(A);
				}
				else {
					a = (l + 2)*k + i;
					b = (l + 2)*k + i + 1;
					c = (l + 2)*(k + 1) + i;
					d = (l + 2)*(k + 1) + i + 1;
					if (k / n == 0 || k / n == 2)
					{
						NetType A(a, b, c, d, material); nv.push_back(A);
					}
					else
					{
						NetType A(b, d, a, c, material); nv.push_back(A);
					}

				}
			}
		//������� ������ ���������� �����
		for (i = 0; i < n - 1; i++) {
			a = (l + 2)*i + l + 1;
			b = (l + 2)*(i + 1) + l + 1;
			c = start_ind + i;
			d = start_ind + i + 1;
			NetType A(a, c, b, d, OIL);
			nv.push_back(A);
		}
		//������� ����� � ��������
		a = (l + 2)*n - 1;
		b = (l + 2)*n + l + 1;
		c = start_ind + n - 1;
		d = (l + 2)*(n + 1) + l + 1;
		NetType A1(a, c, b, d, OIL);
		nv.push_back(A1);
		//������ �����
		a = (l + 2)*(2 * n) - 1;
		b = coor_on_layer - 1;
		c = (l + 2)*(2 * n) + l + 1;
		d = (l + 2)*(2 * n + 2) - 1;
		NetType A2(b, d, a, c, OIL);
		nv.push_back(A2);
		//������ ������
		a = (l + 2)*(3 * n) - 1;
		b = coor_on_layer - n + 1;
		c = (l + 2)*(3 * n + 1) - 1;
		d = (l + 2)*(3 * n + 2) - 1;
		NetType A3(d, c, b, a, OIL);
		nv.push_back(A3);

		for (i = 1; i < n - 1; i++)
		{
			// �� ����� ������
			a = (l + 2)*(n + i) + l + 1;
			b = start_ind + i * (n - 1);
			c = (l + 2)*(n + i + 1) + l + 1;
			d = start_ind + (i + 1)*(n - 1);
			NetType A4(b, d, a, c, OIL);
			nv.push_back(A4);
			//�� ����
			a = (l + 2)*(2 * n + 1 + i) - 1;
			b = coor_on_layer - i;
			c = (l + 2)*(2 * n + 2 + i) - 1;
			d = coor_on_layer - i - 1;
			NetType A5(d, c, b, a, OIL);
			nv.push_back(A5);
			// �� ������ ������
			a = start_ind + (n - 1)*(i - 1) + 1;
			b = start_ind - (i - 1)*(l + 2);
			c = start_ind + (n - 1)*i + 1;
			d = start_ind - (i)*(l + 2);
			NetType A6(b, d, a, c, OIL);
			nv.push_back(A6);
		}
		//���������� ��
		for (i = 1; i < n - 1; i++)
			for (j = 1; j < n - 1; j++) {
				a = start_ind + (n - 1)*(i - 1) + j;
				b = start_ind + (n - 1)*(i - 1) + j + 1;
				c = start_ind + (n - 1)*(i)+j;
				d = start_ind + (n - 1)*i + j + 1;
				NetType A(a, c, b, d, OIL);
				nv.push_back(A);
			}
		return nv;
	};

	bool readFromFiles(const char *path) {
		FILE* file = fopen(path, "r");
		if (file == NULL) return false;

		fscanf(file, "n = %d l = %d\n", &n, &l);

		return true;
	};

public:
	PipeSection() {	
		SectionType A(0, 0, 0, 1, 0.8);
		face = A;
	};
	PipeSection(SectionType circle) {
		face = circle;
		n = 6;
		l = 5;
	};
	~PipeSection() {};
	//���������� ��������� ����� � �������
	vector<PointType> coordTubeOnly(SectionType c, int id) {

		PointType Temp(0, 0, 0, 0);
		vector<PointType> tmp;
		int i, j;
		real a = c.R*1.1;
		real b = c.r*0.6;
		real a_step = 2 * a / (int)n;
		real b_step = 2 * b / (int)n;
		real p = a / a_step;

		real step = (c.R - c.r) / (int)l;

		//����� �� ���������� ��������
		for (int k = 0; k < n; k++)
		{
			//������� �������
			Temp.x = b - k * b_step;
			Temp.z = b;
			Temp.id = (l + 1)*(k + 1) + k + id;
			tmp.push_back(Temp);

			//����� �������
			Temp.x = -b;
			Temp.z = b - k * b_step;
			Temp.id = (l + 2)*(n + k) + l + 1 + id ;
			tmp.push_back(Temp);

			//������ �������
			Temp.x = -b + k * b_step;
			Temp.z = -b;
			Temp.id = (k + 2 * n) * (l + 2) + l + 1 + id;
			tmp.push_back(Temp);

			//������ �������
			Temp.x = b;
			Temp.z = -b + k * b_step;
			Temp.id = (k + 3 * n) * (l + 2) + l + 1 + id;
			tmp.push_back(Temp);

		}
		//����� �� ��������
		for (int k = 0; k <= p; k++)
		{
			// ���� ������ ����� � �������� �������
			if (k == p && (a - p * a_step <= 2))
				for (i = 0; i <= l; i++)
				{
					//������������ ����
					Temp.x = 0;
					Temp.z = c.R - i * step;
					Temp.id = (l + 2)*k + i + id;
					tmp.push_back(Temp);
					//�������������� ����
					Temp.x = -c.R + i * step;
					Temp.z = 0;
					Temp.id = (l + 2) * n + (l + 2)*k + i + id;
					tmp.push_back(Temp);
					//������������ ���
					Temp.x = 0;
					Temp.z = -c.R + i * step;
					Temp.id = 2 * n *(l + 2) + (l + 2)*k + i + id;
					tmp.push_back(Temp);
					//�������������� �����
					Temp.x = c.R - i * step;
					Temp.z = 0;
					Temp.id = 3 * n*(l + 2) + (l + 2)*k + i + id;
					tmp.push_back(Temp);
				}
			else {

				real x1, y1, x2, y2;
				//����� � �������� ��������
				x1 = a - k * a_step;
				y1 = a;

				// ����� � ����������� ��������
				x2 = b - k * b_step;
				y2 = b;

				real r_x, r_z, R_x, R_z;
				circle_point(R_x, R_z, x1, y1, x2, y2, 0.0, 0.0, c.R);
				circle_point(r_x, r_z, x1, y1, x2, y2, 0.0, 0.0, c.r);

				real x_step = abs(R_x - r_x) / l;
				real z_step = abs(R_z - r_z) / l;

				//��������������� ����������
				for (i = 0; i <= l; i++) {

					//���� �����
					Temp.x = R_x - i * x_step;
					Temp.z = R_z - i * z_step;
					Temp.id = (l + 2)*k + i + id;
					tmp.push_back(Temp);

					real dx = Temp.x;
					real dy = Temp.z;
					Point Temp1(0, 0, 0, 0);

					//���� ����
					if (k != 0) {
						Temp1.x = -dx;
						Temp1.z = Temp.z;
						Temp1.id = (l + 2)*n - (l + 2)*k + i + id;
						tmp.push_back(Temp1);
					}

					//��� ����
					Temp1.x = -dx;
					Temp1.z = -dy;
					Temp1.id = (l + 2)*n * 2 + (l + 2)*k + i + id;
					tmp.push_back(Temp1);

					if (k != 0) {
						//��� �����
						Temp1.x = Temp.x;
						Temp1.z = -dy;
						Temp1.id = (l + 2)*n * 3 - (l + 2)*k + i + id;
						tmp.push_back(Temp1);
					}

					//����� ����
					if (k != 0) {
						Temp1.x = dy;
						Temp1.z = dx;
						Temp1.id = (l + 2)* n * 4 + i - (l + 2)*k + id;
						tmp.push_back(Temp1);
					}

					//����� ���
					Temp1.x = dy;
					Temp1.z = -dx;
					Temp1.id = (l + 2)*n * 3 + i + (l + 2)*k + id;
					tmp.push_back(Temp1);

					//���� ����
					Temp1.x = -dy;
					Temp1.z = dx;
					Temp1.id = (l + 2)*n + i + (l + 2)*k + id;
					tmp.push_back(Temp1);

					if (k != 0) {
						//���� ���
						Temp1.x = -dy;
						Temp1.z = -dx;
						Temp1.id = (l + 2)*n * 2 + i - (l + 2)*k + id;
						tmp.push_back(Temp1);
					}

				}
			}
		}
		start_ind = tmp.size() - 1;
		//��������� ��������� �����
		for (i = 1; i < n; i++)
			for (j = 1; j < n; j++) {
				Temp.x = b - j * b_step;
				Temp.z = b - i * b_step;
				Temp.id = start_ind + (n - 1)*(i - 1) + j + id;
				tmp.push_back(Temp);
			}
		coor_on_layer = tmp.size();
		return tmp;
	}
	void buildNet() {
		if (readFromFiles("./input-info/input_cut.txt")) {
				PipeSection<PointType, NetType,SectionType>::coord = coordTubeOnly(face,0);
				PipeSection<PointType, NetType, SectionType>::nvtr = nvtrTubeOnly();
				PipeSection<PointType, NetType, SectionType>::setNodesSize(coor_on_layer);
				PipeSection<PointType, NetType, SectionType>::setElemsSize(PipeSection<PointType, NetType, SectionType>::nvtr.size());
		}
	};
};

#endif //_PIPE-SECTION_HPP_