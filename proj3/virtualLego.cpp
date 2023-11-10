////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: ��â�� Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////
#define BALL_N 44

#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

const double PI = 3.14159265;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)
const float spherePos[BALL_N][2] = { {-2.2f,3.3f},{-1.76f,3.3f},{-1.32f,3.3f},{-0.88f,3.3f}, {-0.44f,3.3f},{0,3.3f},{0.44f,3.3f},{0.88f,3.3f},{1.32f,3.3f},{1.76f,3.3f},{2.2f,3.3f }, //11
									{-2.2f,2.8f},{-0.88f,2.8f},{0.88f,2.8f},{2.2f,2.8f }, //11
									{-2.2f,2.3f},{-1.32f,2.3f}, {-0.44f,2.3f},{0.44f,2.3f},{1.32f,2.3f},{2.2f,2.3f }, //11
									{-2.2f,1.8f},{2.2f,1.8f }, //11
									{-2.2f,1.3f},{-0.88f,1.3f},{0.88f,1.3f},{2.2f,1.3f }, //11
									{-2.2f,0.8f}, {-0.44f,0.8f},{0,0.8f},{0.44f,0.8f},{2.2f,0.8f },
									{-2.2f,0.3f},{-1.76f,0.3f},{-1.32f,0.3f},{-0.88f,0.3f}, {-0.44f,0.3f},{0,0.3f},{0.44f,0.3f},{0.88f,0.3f},{1.32f,0.3f},{1.76f,0.3f},{2.2f,0.3f }, //11
									{0, -2.0f} };
// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[BALL_N] = { d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED, d3d::RED, d3d::RED, d3d::RED,d3d::RED,
										d3d::RED,d3d::RED,d3d::RED,d3d::RED,
										d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,
										d3d::RED,d3d::RED,
										d3d::RED,d3d::RED,d3d::RED,d3d::RED,
										d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,
										d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED,d3d::RED, d3d::RED, d3d::RED, d3d::RED,d3d::RED,
										d3d::WHITE };

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private:
	int checker;
	float					center_x, center_y, center_z;
	float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;

public:
	CSphere(void)
	{
		checker = 1;
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}

public:
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		// Insert your code here.

		if (this->getCheck()&&ball.getCheck()&&sqrt(pow((ball.getCenter().x - this->center_x), 2) + pow((ball.getCenter().y - this->center_y), 2) + pow((ball.getCenter().z - this->center_z), 2)) <= (this->getRadius() + ball.getRadius()))
		{
			return true;
		}
		return false;
	}

	void hitBy(CSphere& ball)
	{
		// Insert your code here.
		if (this->hasIntersected(ball))
		{

			int checker_a = 1;
			int checker_b = 1;
			int checker_c = 1;
			int checker_d = 1;
			double sub_x, sub_z;
			sub_x = ball.getCenter().x - this->getCenter().x;
			sub_z = ball.getCenter().z - this->getCenter().z;
			double distance = sqrt(pow(sub_x, 2) + pow(sub_z, 2));
			double tx, tz, bx, bz;
			tx = this->getVelocity_X();
			tz = this->getVelocity_Z();
			bx = ball.getVelocity_X();
			bz = ball.getVelocity_Z();
			double vt, vb;
			vt = sqrt(pow(tx, 2) + pow(tz, 2));//this�� �ӷ�
			vb = sqrt(pow(bx, 2) + pow(bz, 2));//ball�� �ӷ�
			double x_temp, z_temp;
			double theta;

			theta = acos(sub_x / (ball.getRadius() * 2));//0~pi����

			this->setCenter(this->getCenter().x, this->getCenter().y, this->getCenter().z);
			x_temp = sub_x * 2 * ball.getRadius() / distance;
			z_temp = sub_z * 2 * ball.getRadius() / distance;
			ball.setCenter(this->getCenter().x + x_temp, this->getCenter().y, this->getCenter().z + z_temp);//��ġ�� �� ����
			//��Ƣ��� ����
			//���� ���
			double theta_temp;
			theta_temp = acos(ball.getVelocity_X()/ vb);
			if (ball.getVelocity_Z() < 0)
			{
				theta_temp = 2 * PI - theta_temp;
			}

			theta_temp = PI+2 * theta-theta_temp;

			//���� �⺻ �ӷ� 3
			ball.setPower(3 * cos(theta_temp), 3 * sin(theta_temp));
			this->setPower(0, 0);
			this->setChecker(0);
			/* 
			//4������ �ӵ� ���ϱ�
			//���� ���
			double thetab,thetat;
			if (ball.getVelocity_X() != 0) {
				thetab = acos(ball.getVelocity_X() / vb);
				if (ball.getVelocity_Z() < 0)
				{
					thetab = 2 * PI - thetab;
				} 
			}
			else {
				thetab = PI / 2;
			}
			if (this->getVelocity_X() != 0) {
				thetat = acos(this->getVelocity_X() / vt);
				if (this->getVelocity_Z() < 0)
				{
					thetat = 2 * PI - thetat;
				}
			}
			else {
				thetat = PI / 2;
			}
			
			//��ȣ����
			//theta�� ���� 0~PI/2�ϋ� ������ PI/2~PI������ ��쿡 ���� �����Ǵ� ��ȣ�� ���� �޶���
			//theta�� �����̴� ���� �ӵ� ������ ũ����迡 ���� ���̽��� ������,
			//�̶� ���� ��ü�Ǵ� ���Ϳ�ҿ� ���� ��ü���� �ʴ� ��Ҹ� ���� �����Ͽ� ����Ͽ��� �Ѵ�.
			//�� ���� �ε����µ� �� ���� �� ������ �� �����Ƿ� ���� ���̽��� thetab�� thetat 2���� ��ó �̷������.
			//���� ��ü ���̽��� 8�����̰�, ���� theta�� 0~PI/2�� ��찡 4, theta�� PI/2~PI�� ��찡 4���� ������
			
			if (theta >= 0 && theta <= (PI / 2)) {						//0<=theta<=PI/2
				if (thetab >= theta && thetab < (theta + PI))	//ball�� �ӵ����Ϳ� theta�� �̷�� ���� ����, ��ü���� �ʴ� ball�� �ӵ��� �����ħ
				{
					checker_a = 1;
				}
				else
				{
					checker_a = 2;
				}
				if (thetat >= theta && (thetat <= theta + PI))	//this�� �ӵ����Ϳ� theta�� �̷�� ���� ����, ��ü���� �ʴ� this�� �ӵ��� �����ħ
				{
					checker_b = 1;
				}
				else
				{
					checker_b = 2;
				}
				if (thetab >= (theta + PI / 2) && thetab <= theta + 3 * PI / 2)		//ball�� �ӵ����Ϳ� theta�� �̷�� ���� ����, ��ü���� �ʴ� this�� �ӵ��� �����ħ
				{
					checker_c = 1;
				}
				else
				{
					checker_c = 2;
				}
				if (thetat >= (theta + PI / 2) && thetat <= theta + 3 * PI / 2)		//this�� �ӵ����Ϳ� theta�� �̷�� ���� ����, ��ü���� �ʴ� ball�� �ӵ��� �����ħ
				{
					checker_d = 1;
				}
				else
				{
					checker_d = 2;
				}

				//�籸�� ź���浹 ���� �ӵ� 4�����ӿ��� �����ϰ� �� �ӵ�
				//�� ���� �ε����� �� �� �������� �� �� �ʿ��� ��

				bx = pow(-1, checker_a) * abs(vb * sin(thetab - theta) * sin(theta)) + pow(-1, checker_d) * abs(vt * cos(thetat - theta) * cos(theta));
				bz = pow(-1, checker_a + 1) * abs(vb * sin(thetab - theta) * cos(theta)) + pow(-1, checker_d) * abs(vt * cos(thetat - theta) * sin(theta));
				tx = pow(-1, checker_b) * abs(vt * sin(thetat - theta) * sin(theta)) + pow(-1, checker_c) * abs(vb * cos(thetab - theta) * cos(theta));
				tz = pow(-1, checker_b + 1) * abs(vt * sin(thetat - theta) * cos(theta)) + pow(-1, checker_c) * abs(vb * cos(thetab - theta) * sin(theta));

				
				ball.setPower(bx, bz);
				this->setPower(tx, tz);
			}
			else		//PI/2<=theta<=PI�� ���
			{//������ ������ �Ͱ� �����
				if (thetab >= theta && thetab < (theta + PI))
				{
					checker_a = 1;
				}
				else
				{
					checker_a = 2;
				}
				if (thetab >= theta && (thetab <= theta + PI))
				{
					checker_b = 1;
				}
				else
				{
					checker_b = 2;
				}
				if (thetab >= (theta - PI / 2) && thetab <= (theta + PI / 2))
				{
					checker_c = 1;
				}
				else
				{
					checker_c = 2;
				}
				if (thetat >= (theta - PI / 2) && thetat <= (theta + PI / 2))
				{
					checker_d = 1;
				}
				else
				{
					checker_d = 2;
				}
				//�籸�� ź���浹 ���� �ӵ� 4�����ӿ��� �����ϰ� �� �ӵ�
				//�� ���� �ε����� �� �� �������� �� �� �ʿ��� ��
				bx = pow(-1, checker_a) * abs(vb * sin(thetab - theta) * sin(theta)) + pow(-1, checker_d) * abs(vt * cos(thetat - theta) * cos(theta));
				bz = pow(-1, checker_a) * abs(vb * sin(thetab - theta) * cos(theta)) + pow(-1, checker_d + 1) * abs(vt * cos(thetat - theta) * sin(theta));
				tx = pow(-1, checker_b) * abs(vt * sin(thetat - theta) * sin(theta)) + pow(-1, checker_c) * abs(vb * cos(thetab - theta) * cos(theta));
				tz = pow(-1, checker_b) * abs(vt * sin(thetat - theta) * cos(theta)) + pow(-1, checker_c + 1) * abs(vb * cos(thetab - theta) * sin(theta));

				ball.setPower(bx, bz);
				this->setPower(tx, tz);
			}*/
		}
	}


	void ballUpdate(float timeDiff)
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if (vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
			float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

			//correction of position of ball
			// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
/*
			if (tX >= (4.5 - M_RADIUS))
				tX = 4.5 - M_RADIUS;
			else if (tX <= (-4.5 + M_RADIUS))
				tX = -4.5 + M_RADIUS;
			else if (tZ <= (-3 + M_RADIUS))
				tZ = -3 + M_RADIUS;
			else if (tZ >= (3 - M_RADIUS))
				tZ = 3 - M_RADIUS;
*/

			this->setCenter(tX, cord.y, tZ);
		}
		else { this->setPower(0, 0); }
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1;
		//		double rate = 1 - (1 - DECREASE_RATE) * timeDiff * 400;
		if (rate < 0)
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
	}

	double getVelocity_X() { return this->m_velocity_x; }
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x = x;	center_y = y;	center_z = z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getRadius(void)  const { return (float)(M_RADIUS); }
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}

	void setChecker(int n)
	{
		this->checker = n;
	}

	bool getCheck()
	{
		if (this->checker == 1)
			return true;
		else
			return false;
	}

private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pSphereMesh;

};



// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:

	float					m_x;
	float					m_z;
	float                   m_width;
	float                   m_depth;
	float					m_height;

public:
	CWall(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		m_width = iwidth;
		m_depth = idepth;

		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}
	void destroy(void)
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		// Insert your code here.
		if (ball.getCenter().z >= (4.5 - ball.getRadius())|| ball.getCenter().z <= (-4.5+ ball.getRadius())|| ball.getCenter().x >= (3 - ball.getRadius()) || ball.getCenter().x <= (-3 + ball.getRadius()))
			return true;
		return false;
	}

	void hitBy(CSphere& ball)
	{
		// Insert your code here.

			if (this->hasIntersected(ball) == true) {
				if (ball.getCenter().z >= (4.5 - ball.getRadius()))
				{
					ball.setCenter(ball.getCenter().x,ball.getCenter().y, 4.5 - ball.getRadius());
					ball.setPower(ball.getVelocity_X(), -1*ball.getVelocity_Z());
				}
				else if (ball.getCenter().z <= (-4.5 + ball.getRadius())) {
					ball.setCenter(0, ball.getCenter().y, -2);
					ball.setPower(0, 0);
				}

				if (ball.getCenter().x >= (3.0 - ball.getRadius()))
				{
					ball.setCenter(3.0 - ball.getRadius(), ball.getCenter().y, ball.getCenter().z);
					ball.setPower(-1 * ball.getVelocity_X(), ball.getVelocity_Z());
				}
				else if (ball.getCenter().x <= (-3.0 + ball.getRadius())) {
					ball.setCenter(-3.0 + ball.getRadius(), ball.getCenter().y, ball.getCenter().z );
					ball.setPower(-1 * ball.getVelocity_X(), ball.getVelocity_Z());
				}

			}// �ε����� �ʾҴٸ� ������� �ʴ´�.
			else {
				return;
			}

			// Insert your code here
	}

	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getHeight(void) const { return M_HEIGHT; }



private:
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pBoundMesh;
};

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
public:
	CLight(void)
	{
		static DWORD i = 0;
		m_index = i++;
		D3DXMatrixIdentity(&m_mLocal);
		::ZeroMemory(&m_lit, sizeof(m_lit));
		m_pMesh = NULL;
		m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bound._radius = 0.0f;
	}
	~CLight(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
	{
		if (NULL == pDevice)
			return false;
		if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
			return false;

		m_bound._center = lit.Position;
		m_bound._radius = radius;

		m_lit.Type = lit.Type;
		m_lit.Diffuse = lit.Diffuse;
		m_lit.Specular = lit.Specular;
		m_lit.Ambient = lit.Ambient;
		m_lit.Position = lit.Position;
		m_lit.Direction = lit.Direction;
		m_lit.Range = lit.Range;
		m_lit.Falloff = lit.Falloff;
		m_lit.Attenuation0 = lit.Attenuation0;
		m_lit.Attenuation1 = lit.Attenuation1;
		m_lit.Attenuation2 = lit.Attenuation2;
		m_lit.Theta = lit.Theta;
		m_lit.Phi = lit.Phi;
		return true;
	}
	void destroy(void)
	{
		if (m_pMesh != NULL) {
			m_pMesh->Release();
			m_pMesh = NULL;
		}
	}
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return false;

		D3DXVECTOR3 pos(m_bound._center);
		D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
		D3DXVec3TransformCoord(&pos, &pos, &mWorld);
		m_lit.Position = pos;

		pDevice->SetLight(m_index, &m_lit);
		pDevice->LightEnable(m_index, TRUE);
		return true;
	}

	void draw(IDirect3DDevice9* pDevice)
	{
		if (NULL == pDevice)
			return;
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
		pDevice->SetTransform(D3DTS_WORLD, &m);
		pDevice->SetMaterial(&d3d::WHITE_MTRL);
		m_pMesh->DrawSubset(0);
	}

	D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh* m_pMesh;
	d3d::BoundingSphere m_bound;
};


// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[4];
CSphere	g_sphere[BALL_N];
CSphere	g_target_blueball;
CLight	g_light;

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
	int i;

	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position
	if (false == g_legoPlane.create(Device, -1, -1, 6, 0.03f, 9, d3d::GREEN)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 0.12f, 0.3f, 9, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(3.06f, 0.12f, 0.0f);
	if (false == g_legowall[1].create(Device, -1, -1, 0.12f, 0.3f, 9, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(-3.06f, 0.12f, 0.0f);
	if (false == g_legowall[2].create(Device, -1, -1, 6.24f, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(0.0f, 0.12f, 4.56f);
	if (false == g_legowall[3].create(Device, -1, -1, 6.24f, 0.3f, 0.12f, d3d::MAGENTA)) return false;
	g_legowall[3].setPosition(0.0f, 0.12f, -3.56f);

	// create four balls and set the position
	for (i = 0; i < BALL_N; i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
		g_sphere[i].setPower(0, 0);
	}

	// create blue ball for set direction
	if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS, -3.5f);

	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < 4; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;


	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (i = 0; i < BALL_N; i++) {
			g_sphere[i].ballUpdate(timeDelta);
		}
		for (i = 0; i < 4;i++) 
		{
			for (j = 0; j < BALL_N; j++) { g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for (i = 0; i < BALL_N; i++) {
			for (j = 0; j < BALL_N; j++) {
				if (i >= j) { continue; }
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}
		g_target_blueball.hitBy(g_sphere[BALL_N-1]);
		g_target_blueball.setChecker(1);
		g_sphere[BALL_N - 1].setChecker(1);

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i < 4; i++) {
			g_legowall[i].draw(Device, g_mWorld);
		}
		for(i=0;i< BALL_N;i++)
		{
			if(g_sphere[i].getCheck())
				g_sphere[i].draw(Device, g_mWorld);
		}
		g_target_blueball.draw(Device, g_mWorld);
		g_light.draw(Device);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static int old_x = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;

	switch (msg) {
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_ESCAPE:
			::DestroyWindow(hwnd);
			break;
		case VK_RETURN:
			if (NULL != Device) {
				wire = !wire;
				Device->SetRenderState(D3DRS_FILLMODE,
					(wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
			}
			break;
		case VK_SPACE:

			D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
			D3DXVECTOR3	whitepos = g_sphere[BALL_N-1].getCenter();
			double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
				pow(targetpos.z - whitepos.z, 2)));		// �⺻ 1 ��и�

			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 ��и�
			if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 ��и�
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0) { theta = PI + theta; } // 3 ��и�

			double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
			g_sphere[BALL_N-1].setPower(3 * cos(theta), 3 * sin(theta));

			break;

		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		int new_x = LOWORD(lParam);
		int new_y = HIWORD(lParam);
		float dx;
		float dy;

		if (LOWORD(wParam) & MK_LBUTTON) {
			/*
			if (isReset) {
				isReset = false;
			}
			else {
				D3DXVECTOR3 vDist;
				D3DXVECTOR3 vTrans;
				D3DXMATRIX mTrans;
				D3DXMATRIX mX;
				D3DXMATRIX mY;

				switch (move) {
				case WORLD_MOVE:
					dx = (old_x - new_x) * 0.01f;
					dy = (old_y - new_y) * 0.01f;
					D3DXMatrixRotationY(&mX, dx);
					D3DXMatrixRotationX(&mY, dy);
					g_mWorld = g_mWorld * mX * mY;

					break;
				}
			}

			old_x = new_x;
			old_y = new_y;
			*/
		}
		else {
			isReset = true;

			if (LOWORD(wParam) & MK_RBUTTON) {
				dx = (old_x - new_x);// * 0.01f;
				dy = (old_y - new_y);// * 0.01f;

				D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
				g_target_blueball.setCenter(coord3d.x + dx * (-0.007f), coord3d.y, coord3d.z);
			}
			old_x = new_x;
			old_y = new_y;

			move = WORLD_MOVE;
		}
		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}