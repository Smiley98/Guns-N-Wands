#include "matrix4x4.h"

#include <memory>
#include <math.h>

#include "vector3.h"

namespace spritelib
{
	namespace math
	{
		float Matrix4x4::s_identity[16] = 
		{ 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		Matrix4x4::Matrix4x4()
		{
			std::fill(std::begin(m_data), std::end(m_data), 0.0f);
		}

		Matrix4x4::Matrix4x4(float a_data[16])
		{
			memcpy(m_data, a_data, 16 * sizeof(float));
		}

		Matrix4x4& Matrix4x4::make_identity()
		{
			memcpy(m_data, s_identity, 16 * sizeof(float));
			return *this;
		}

		Matrix4x4 Matrix4x4::ortho(float a_left, float a_right, float a_bottom, float a_top, float a_near, float a_far)
		{
			Matrix4x4 ret = Matrix4x4(s_identity);

			ret.m_data[0 + 0 * 4] = 2.0f / (a_right - a_left);
			ret.m_data[1 + 1 * 4] = 2.0f / (a_top - a_bottom);
			ret.m_data[2 + 2 * 4] = 2.0f / (a_near - a_far);
			ret.m_data[0 + 3 * 4] = (a_left + a_right) / (a_left - a_right);
			ret.m_data[1 + 3 * 4] = (a_bottom + a_top) / (a_bottom - a_top);
			ret.m_data[2 + 3 * 4] = (a_far + a_near) / (a_far - a_near);

			return ret;
		}

		Matrix4x4 Matrix4x4::translate(Vector3 a_vec)
		{
			Matrix4x4 ret = Matrix4x4(s_identity);

			ret.m_data[0 + 3 * 4] = a_vec.x;
			ret.m_data[1 + 3 * 4] = a_vec.y;
			ret.m_data[2 + 3 * 4] = a_vec.z;

			return ret;
		}

		Matrix4x4 Matrix4x4::scale(Vector3 a_vec)
		{
			Matrix4x4 ret = Matrix4x4(s_identity);

			ret.m_data[0 + 0 * 4] *= a_vec.x;
			ret.m_data[1 + 1 * 4] *= a_vec.y;
			ret.m_data[2 + 2 * 4] *= a_vec.z;

			return ret;
		}

		Matrix4x4 Matrix4x4::rotate(float a_angle)
		{
			Matrix4x4 ret = Matrix4x4(s_identity);

			float r = (float)((PI / 180.0f) * a_angle);
			float c = (float)cos(r);
			float s = (float)sin(r);

			ret.m_data[0 + 0 * 4] = c;
			ret.m_data[1 + 0 * 4] = s;
			ret.m_data[0 + 1 * 4] = -s;
			ret.m_data[1 + 1 * 4] = c;

			return ret;
		}

		int e(int i, int j, int a, int b)
		{
			return ((j + b) % 4) * 4 + ((i + a) % 4);
		}

		float invf(int i, int j, float* m) {

			int o = 2 + (j - i);

			i += 4 + o;
			j += 4 - o;

			float inv =
				+ m[e(i,i,+1, -1)]*m[e(i,i,+0, +0)]*m[e(i,i,-1, +1)]
				+ m[e(i,i,+1, +1)]*m[e(i,i,+0, -1)]*m[e(i,i,-1, +0)]
				+ m[e(i,i,-1, -1)]*m[e(i,i,+1, +0)]*m[e(i,i,+0, +1)]
				- m[e(i,i,-1, -1)]*m[e(i,i,+0, +0)]*m[e(i,i,+1, +1)]
				- m[e(i,i,-1, +1)]*m[e(i,i,+0, -1)]*m[e(i,i,+1, +0)]
				- m[e(i,i,+1, -1)]*m[e(i,i,-1, +0)]*m[e(i,i,+0, +1)];

			return (o % 2) ? inv : -inv;
		}

		Matrix4x4 Matrix4x4::inverse(Matrix4x4 &a_matrix)
		{
			float inv[16];
			float det = 0.0f;

			inv[0] = a_matrix.m_data[5] * a_matrix.m_data[10] * a_matrix.m_data[15] -
				a_matrix.m_data[5] * a_matrix.m_data[11] * a_matrix.m_data[14] -
				a_matrix.m_data[9] * a_matrix.m_data[6] * a_matrix.m_data[15] +
				a_matrix.m_data[9] * a_matrix.m_data[7] * a_matrix.m_data[14] +
				a_matrix.m_data[13] * a_matrix.m_data[6] * a_matrix.m_data[11] -
				a_matrix.m_data[13] * a_matrix.m_data[7] * a_matrix.m_data[10];

			inv[4] = -a_matrix.m_data[4] * a_matrix.m_data[10] * a_matrix.m_data[15] +
				a_matrix.m_data[4] * a_matrix.m_data[11] * a_matrix.m_data[14] +
				a_matrix.m_data[8] * a_matrix.m_data[6] * a_matrix.m_data[15] -
				a_matrix.m_data[8] * a_matrix.m_data[7] * a_matrix.m_data[14] -
				a_matrix.m_data[12] * a_matrix.m_data[6] * a_matrix.m_data[11] +
				a_matrix.m_data[12] * a_matrix.m_data[7] * a_matrix.m_data[10];

			inv[8] = a_matrix.m_data[4] * a_matrix.m_data[9] * a_matrix.m_data[15] -
				a_matrix.m_data[4] * a_matrix.m_data[11] * a_matrix.m_data[13] -
				a_matrix.m_data[8] * a_matrix.m_data[5] * a_matrix.m_data[15] +
				a_matrix.m_data[8] * a_matrix.m_data[7] * a_matrix.m_data[13] +
				a_matrix.m_data[12] * a_matrix.m_data[5] * a_matrix.m_data[11] -
				a_matrix.m_data[12] * a_matrix.m_data[7] * a_matrix.m_data[9];

			inv[12] = -a_matrix.m_data[4] * a_matrix.m_data[9] * a_matrix.m_data[14] +
				a_matrix.m_data[4] * a_matrix.m_data[10] * a_matrix.m_data[13] +
				a_matrix.m_data[8] * a_matrix.m_data[5] * a_matrix.m_data[14] -
				a_matrix.m_data[8] * a_matrix.m_data[6] * a_matrix.m_data[13] -
				a_matrix.m_data[12] * a_matrix.m_data[5] * a_matrix.m_data[10] +
				a_matrix.m_data[12] * a_matrix.m_data[6] * a_matrix.m_data[9];

			det = a_matrix.m_data[0] * inv[0] + a_matrix.m_data[1] * inv[4] + a_matrix.m_data[2] * inv[8] + a_matrix.m_data[3] * inv[12];

			if (det == 0.0f)
			{
				// Avoid divide by zero error.
				return Matrix4x4(s_identity);
			}

			inv[1] = -a_matrix.m_data[1] * a_matrix.m_data[10] * a_matrix.m_data[15] +
				a_matrix.m_data[1] * a_matrix.m_data[11] * a_matrix.m_data[14] +
				a_matrix.m_data[9] * a_matrix.m_data[2] * a_matrix.m_data[15] -
				a_matrix.m_data[9] * a_matrix.m_data[3] * a_matrix.m_data[14] -
				a_matrix.m_data[13] * a_matrix.m_data[2] * a_matrix.m_data[11] +
				a_matrix.m_data[13] * a_matrix.m_data[3] * a_matrix.m_data[10];

			inv[5] = a_matrix.m_data[0] * a_matrix.m_data[10] * a_matrix.m_data[15] -
				a_matrix.m_data[0] * a_matrix.m_data[11] * a_matrix.m_data[14] -
				a_matrix.m_data[8] * a_matrix.m_data[2] * a_matrix.m_data[15] +
				a_matrix.m_data[8] * a_matrix.m_data[3] * a_matrix.m_data[14] +
				a_matrix.m_data[12] * a_matrix.m_data[2] * a_matrix.m_data[11] -
				a_matrix.m_data[12] * a_matrix.m_data[3] * a_matrix.m_data[10];

			inv[9] = -a_matrix.m_data[0] * a_matrix.m_data[9] * a_matrix.m_data[15] +
				a_matrix.m_data[0] * a_matrix.m_data[11] * a_matrix.m_data[13] +
				a_matrix.m_data[8] * a_matrix.m_data[1] * a_matrix.m_data[15] -
				a_matrix.m_data[8] * a_matrix.m_data[3] * a_matrix.m_data[13] -
				a_matrix.m_data[12] * a_matrix.m_data[1] * a_matrix.m_data[11] +
				a_matrix.m_data[12] * a_matrix.m_data[3] * a_matrix.m_data[9];

			inv[13] = a_matrix.m_data[0] * a_matrix.m_data[9] * a_matrix.m_data[14] -
				a_matrix.m_data[0] * a_matrix.m_data[10] * a_matrix.m_data[13] -
				a_matrix.m_data[8] * a_matrix.m_data[1] * a_matrix.m_data[14] +
				a_matrix.m_data[8] * a_matrix.m_data[2] * a_matrix.m_data[13] +
				a_matrix.m_data[12] * a_matrix.m_data[1] * a_matrix.m_data[10] -
				a_matrix.m_data[12] * a_matrix.m_data[2] * a_matrix.m_data[9];

			inv[2] = a_matrix.m_data[1] * a_matrix.m_data[6] * a_matrix.m_data[15] -
				a_matrix.m_data[1] * a_matrix.m_data[7] * a_matrix.m_data[14] -
				a_matrix.m_data[5] * a_matrix.m_data[2] * a_matrix.m_data[15] +
				a_matrix.m_data[5] * a_matrix.m_data[3] * a_matrix.m_data[14] +
				a_matrix.m_data[13] * a_matrix.m_data[2] * a_matrix.m_data[7] -
				a_matrix.m_data[13] * a_matrix.m_data[3] * a_matrix.m_data[6];

			inv[6] = -a_matrix.m_data[0] * a_matrix.m_data[6] * a_matrix.m_data[15] +
				a_matrix.m_data[0] * a_matrix.m_data[7] * a_matrix.m_data[14] +
				a_matrix.m_data[4] * a_matrix.m_data[2] * a_matrix.m_data[15] -
				a_matrix.m_data[4] * a_matrix.m_data[3] * a_matrix.m_data[14] -
				a_matrix.m_data[12] * a_matrix.m_data[2] * a_matrix.m_data[7] +
				a_matrix.m_data[12] * a_matrix.m_data[3] * a_matrix.m_data[6];

			inv[10] = a_matrix.m_data[0] * a_matrix.m_data[5] * a_matrix.m_data[15] -
				a_matrix.m_data[0] * a_matrix.m_data[7] * a_matrix.m_data[13] -
				a_matrix.m_data[4] * a_matrix.m_data[1] * a_matrix.m_data[15] +
				a_matrix.m_data[4] * a_matrix.m_data[3] * a_matrix.m_data[13] +
				a_matrix.m_data[12] * a_matrix.m_data[1] * a_matrix.m_data[7] -
				a_matrix.m_data[12] * a_matrix.m_data[3] * a_matrix.m_data[5];

			inv[14] = -a_matrix.m_data[0] * a_matrix.m_data[5] * a_matrix.m_data[14] +
				a_matrix.m_data[0] * a_matrix.m_data[6] * a_matrix.m_data[13] +
				a_matrix.m_data[4] * a_matrix.m_data[1] * a_matrix.m_data[14] -
				a_matrix.m_data[4] * a_matrix.m_data[2] * a_matrix.m_data[13] -
				a_matrix.m_data[12] * a_matrix.m_data[1] * a_matrix.m_data[6] +
				a_matrix.m_data[12] * a_matrix.m_data[2] * a_matrix.m_data[5];

			inv[3] = -a_matrix.m_data[1] * a_matrix.m_data[6] * a_matrix.m_data[11] +
				a_matrix.m_data[1] * a_matrix.m_data[7] * a_matrix.m_data[10] +
				a_matrix.m_data[5] * a_matrix.m_data[2] * a_matrix.m_data[11] -
				a_matrix.m_data[5] * a_matrix.m_data[3] * a_matrix.m_data[10] -
				a_matrix.m_data[9] * a_matrix.m_data[2] * a_matrix.m_data[7] +
				a_matrix.m_data[9] * a_matrix.m_data[3] * a_matrix.m_data[6];

			inv[7] = a_matrix.m_data[0] * a_matrix.m_data[6] * a_matrix.m_data[11] -
				a_matrix.m_data[0] * a_matrix.m_data[7] * a_matrix.m_data[10] -
				a_matrix.m_data[4] * a_matrix.m_data[2] * a_matrix.m_data[11] +
				a_matrix.m_data[4] * a_matrix.m_data[3] * a_matrix.m_data[10] +
				a_matrix.m_data[8] * a_matrix.m_data[2] * a_matrix.m_data[7] -
				a_matrix.m_data[8] * a_matrix.m_data[3] * a_matrix.m_data[6];

			inv[11] = -a_matrix.m_data[0] * a_matrix.m_data[5] * a_matrix.m_data[11] +
				a_matrix.m_data[0] * a_matrix.m_data[7] * a_matrix.m_data[9] +
				a_matrix.m_data[4] * a_matrix.m_data[1] * a_matrix.m_data[11] -
				a_matrix.m_data[4] * a_matrix.m_data[3] * a_matrix.m_data[9] -
				a_matrix.m_data[8] * a_matrix.m_data[1] * a_matrix.m_data[7] +
				a_matrix.m_data[8] * a_matrix.m_data[3] * a_matrix.m_data[5];

			inv[15] = a_matrix.m_data[0] * a_matrix.m_data[5] * a_matrix.m_data[10] -
				a_matrix.m_data[0] * a_matrix.m_data[6] * a_matrix.m_data[9] -
				a_matrix.m_data[4] * a_matrix.m_data[1] * a_matrix.m_data[10] +
				a_matrix.m_data[4] * a_matrix.m_data[2] * a_matrix.m_data[9] +
				a_matrix.m_data[8] * a_matrix.m_data[1] * a_matrix.m_data[6] -
				a_matrix.m_data[8] * a_matrix.m_data[2] * a_matrix.m_data[5];

			det = 1.0f / det;

			float ret[16];

			for (unsigned i = 0; i < 16u; i++)
			{
				ret[i] = inv[i] * det;
			}

			return Matrix4x4(ret);
		}

		Matrix4x4 Matrix4x4::multiply(Matrix4x4 a_matrix)
		{
			Matrix4x4 ret = Matrix4x4();

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					float sum = 0.0f;
					for (int k = 0; k < 4; k++)
					{
						sum += m_data[j + k * 4] * a_matrix.m_data[k + i * 4];
					}
					ret.m_data[j + i * 4] = sum;
				}
			}
			return ret;
		}
	}
}