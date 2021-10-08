#pragma once

#define SINGLE(type) private: type(); ~type();\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}

#define SAFE_RELEASE(p) if(nullptr != p) p->Release()
#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;

#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }

#define SAFE_DELETE_VECLIST(p) \
{\
auto	iter = p.begin();\
auto	iterEnd = p.end();\
for(; iter != iterEnd; ++iter)\
{\
	SAFE_DELETE((*iter));\
}\
p.clear();\
}




#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY(Key, State) (CKeyMgr::GetInst()->GetKeyState(Key) == State)
#define KEY_HOLD(Key) KEY(Key, KEY_STATE::STATE_HOLD)
#define KEY_TAB(Key) KEY(Key, KEY_STATE::STATE_TAB)
#define KEY_AWAY(Key) KEY(Key, KEY_STATE::STATE_AWAY)
#define KEY_NONE(Key) KEY(Key, KEY_STATE::STATE_NONE)

#define CLONE(type) public: type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) type* Clone() { assert(nullptr); return nullptr;} \
                            type(const type& _class){assert(nullptr);}


#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define DBG_WARNNING(pStr) CDebugMgr::GetInst()->AddDbgMsg(DBG_TYPE::DBG_WARNNING, pStr);
#define DBG_ERROR(pStr) CDebugMgr::GetInst()->AddDbgMsg(DBG_TYPE::DBG_ERROR, pStr);
#define DBG_MSG(pStr) CDebugMgr::GetInst()->AddDbgMsg(DBG_TYPE::DBG_MSG, pStr);


#define MAX_LAYER 32

#define _RADIAN 57.2957795130823f
#define _DEGREE 0.01745329251994f

#define _SHADOWSIZE_Y 512.f
#define _SHADOWSIZE_X _SHADOWSIZE_Y * 1.71421356f

#define COMPARED_INTEGER(_float) (int)((_float) * 100)

#define ObjectX 4000.f
//#define ObjectY -500.f
#define ObjectY 3450.f
#define ObjectZ 4000.f
#define	CliffY  800.f

#define INGAME_DISTANCE 0.05f

typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
using DirectX::SimpleMath::Matrix;

enum class PARTICLE_TYPE
{
	EXPLOSION,
	FLUTTER,
	FALL,
	END,
};

enum class CAMERA_MOD
{
	CLIENT_MOD,
	ENGINE_MOD,
	END,
};

enum class DISTANCE
{
	NEARBY,		// 3M ����
	MIDDLE,		// 3 ~ 8M
	AFAR,		// 9~ 16M
	END,
};

enum class BLADEMASTER_FSM
{
	// 1
	_1_STD_BLOCKING,		//����
	_1_STD_BLOCK1,
	_1_STD_BLOCK2,
	_1_STD_BLOCK3,

	_1_STD_LEGSLASH,		// ����

	// 2
	_2_STD_RUSHING,	// ����
	_2_STD_RUSHED,
	
	_2_HID_RUSHING,	// ����
	_2_HID_RUSHED,
	
	// 3
//	_3_HID_FIVE_COMBO_SLASH_CAST,	// ��������		CAST�� ���
	_3_HID_FIVE_COMBO_SLASH_1,
	_3_HID_FIVE_COMBO_SLASH_2,
	_3_HID_FIVE_COMBO_SLASH_3,
	_3_HID_FIVE_COMBO_SLASH_4,
	_3_HID_FIVE_COMBO_SLASH_5,
	
	_3_STD_RETURN_READY,				// ���� ĳ����
	_3_STD_RETURN_STANDBY,			// ���� �����
	_3_STD_RETURN_EXEC,				// ���� ����
	_3_STD_RETURN_FINISH,				// ���� ����

	_3_STD_LOWSLASH_CAST,		// �ϴܹ�� ����
	_3_STD_LOWSLASH_EXEC,		// �ϴܹ�� ����

	// 4
	_4_STD_LYING_RETURN_READY,		// ���� ĳ����
	_4_STD_LYING_RETURN_STANDBY,		// ���� �����
	_4_STD_LYING_RETURN_FINISH,		// ���� ����

	_4_STD_MACH_SLASH,				// ��������
	_4_HID_MACH_SLASH,

	// Z
	_Z_STD_FLYING_SWALLOW_SWORD_EXEC,		// �񿬰�
	_Z_STD_FLYING_SWALLOW_SWORD_FINISH,

	_Z_STD_FLYING_SWALLOW_SWORD_END,
	_Z_STD_FLYING_SWALLOW_SWORD_ENDTOIDLE,

	_Z_HID_LAND_CUTTING,					// ����������
	_Z_HID_LAND_CUT_END,					

	// X
	_X_STD_KICKKNOCKBACK_CAST,			// ������
	_X_STD_KICKKNOCKBACK_EXEC,
	_X_STD_KICKKNOCKBACK_END,

	// SS
	_S_BACKSTEP,			// �Ĺ��̵�

	// C
	_C_STD_SHOULDERCHARGE,		// ���ġ��
	
	_C_HID_WINDYMOONSLASH_01,		// ǳ����
	_C_HID_WINDYMOONSLASH_02,
	_C_HID_WINDYMOONSLASH_FINISH,

	// V
	_V_STD_DIFFUSIONSOUL_1,			// ��˹߻�
	_V_STD_DIFFUSIONSOUL_2,			// ��˹߻�

	_V_HID_PIERCE1,				// �ϼ�
	_V_HID_PIERCE2,				
	_V_HID_PIERCE3,				

	// Q
	_Q_COUNTERATTACK_Q,		// ����

	// E
	_E_COUNTERATTACK_E,		// ����

	// TAB
	_T_FULL_MOON_SLASH,		// ��������

	_T_ESCAPE,				// Ż��
	_T_ESCAPE_END,			// Ż�� �� ó��

	// LBTN
	_L_STD_HACK,						// ȸ������
	_L_STD_HACK_END,					// ȸ�� �� ó��

	_L_STD_RUSHING_WIND_SPHERE,		// �⺻ ����

	_L_STD_BODY_BLADE_UNITY_FIRE,		// �Ű�����	���
	_L_STD_BODY_BLADE_UNITY_EXEC,

	_L_STD_BODY_BLADE_SPRINT_FIRE,	// �Ű����� ����
	_L_STD_BODY_BLADE_SPRINT_EXEC,

	_L_STD_ASCENSION,					// ��õ

	_L_HID_BATTO,						// �ߵ�

	// RBTN
	_R_STD_RUSH_STAB_EXEC,			// ���

	_R_STD_MIDAIR_EXEC_01,			// õ��
	_R_STD_MIDAIR_EXEC_02,
	_R_STD_MIDAIR_EXEC_03,
	_R_STD_MIDAIR_EXEC_04,
	_R_STD_MIDAIR_EXEC_05,
	
	_R_STD_CRIPPLING_CUT_FIRE,		// �߶�
	
	_R_STD_LIGHTNINGSMASH_FIRE,		// �������
	_R_STD_LIGHTNINGSMASH_EXEC,		
	
	_R_HID_VITALSLASH_EXEC,			// �޼Һ���

	// F
	_F_STD_PHANTOMSLASH3_SWING,		// �÷�����
	_F_STD_PHANTOMSLASH3_EXEC,
	
	_F_BACKWARDROLL,				// �ڱ�����
	
	_F_HID_BATTO_BLOCKING_START,		// �ݰ�
	_F_HID_BATTO_BLOCKING,

	//====== ��� ======//
	// Z
	_Z_STD_SOULBLAST,					// ���˷� (�˼� �ڼ�)
	_Z_HID_SOULBLAST,					// ���˷� (�ߵ� �ڼ�)

	// X
	_X_STD_GUARDSOUL_BLADE,			// ��ȣ��
	_X_HID_THIRDBLADE_SLASH,			// ��� ���� ����

	// C
	_C_STD_SOULSTUNLIGHT,			// ���� (�˼� �ڼ�)
	_C_HID_SOULSTUNLIGHT,			// ���� (�ߵ� �ڼ�)

	// V
	_V_STD_ROTATESOULBLADE,		// ȸõ�� (�˼� �ڼ�)
	_V_HID_ROTATESOULBLADE,		// ȸõ�� (�ߵ� �ڼ�)


	//====== IDLEING ======//
	//NONBATTLE
	_NON_MOV_IDLE,
	//STD
	_STD_MOV_IDLE,
	//HID
	_HID_SKL_IDLE,

	// �������� ���ƿ���
	_NON_EQUIPX,

	//======== �����̻��========//
	// ������
	_CAPTURE_EXEC2_FIRE,	// ����
	_CAPTURE_EXEC2_END,	// ��

	// �˹�
	_KNOCKBACK_LONG_B_START,
	_DOWN_FRONT_END,		// ������ �Ѿ��� �� ���

	// ����
	_STUN_START,
	_STUN_LOOPING,

	// �׷α�
	_GROGGY_F_START,
	_GROGGY_LOOPING,
	_GROGGY_END,

	// �ٿ�
	_DOWN_MID_B_START,		// ������, �񿰰�, ��������
	_DOWN_LOW_B_R_START,	// �ϴܺ���, �ٸ��ɱ�
	_DOWN_LOOPING,
	_DOWN_END,				// �ڷ� �Ѿ��� �� ���

	// ����
	_MIDAIR1_UPPER,		// �÷�����, ��Ÿ
	_UPPER_SLASH,
	_MIDAIR2_UPPER,		// ��������
	_MIDAIR1_LOWER,
	_SKYKICK,
	_ASCENSION_ED,

	//==========FEEDBACK========//

	// ����Ʈ �迭
	_FLYTAKEDOWN,		// ����
	_MOUNTX_BAT_IDLE,				// ���� ��
	_MOUNTX_BAT_END,				// ���� ��
	_MOUNTX_SKL_ARMBAR,
	_MOUNTX_SKL_POUNDING_FIRST,
	_MOUNTX_SKL_POUNDING,

	// �⺻ �ǰ�
	_DAMAGEBACK,		// �ǹ� �ǰ�
	_DAMAGEFRONT,		// �⺻ �ǰ�

	// ���� ���� ��
	_BE_DOWNCOUNTER,

	// ���� ���� ��
	_VIOLENCEPUNCH,

	// ȭ����
	_TRIPLEKICK,

	//========== ���͸�� ==========// 
	_BATTO_SKL_END,	//�ߵ� ��ų �� �ߵ��� ���͸��


	//====== MOVING ======//
	//NONBATTLE
	_NON_MOV_IDLETOJUMP_FRONT,
	_NON_MOV_IDLETOJUMP_LEFT,
	_NON_MOV_IDLETOJUMP_RIGHT,
	_NON_MOV_JUMPFRONT,
	_NON_MOV_JUMPLEFT,
	_NON_MOV_JUMPRIGHT,
	_NON_MOV_JUMPTOIDLE,
	_NON_MOV_JUMPTOMOVE_BACK,
	_NON_MOV_JUMPTOMOVE_FRONT,
	_NON_MOV_JUMPTOMOVE_LEFT,
	_NON_MOV_JUMPTOMOVE_RIGHT,
	_NON_MOV_LEFTTORIGHT,
	_NON_MOV_MOVETOIDLE,
	_NON_MOV_RIGHTTOLEFT,
	_NON_MOV_RUNBACK,
	_NON_MOV_RUNFRONT,
	_NON_MOV_RUNLEFT,
	_NON_MOV_RUNLEFTBACK,
	_NON_MOV_RUNRIGHT,
	_NON_MOV_RUNRIGHTBACK,

	//STD
	_STD_MOV_IDLETOJUMP_FRONT,
	_STD_MOV_IDLETOJUMP_LEFT,
	_STD_MOV_IDLETOJUMP_RIGHT,
	_STD_MOV_JUMPFRONT,
	_STD_MOV_JUMPLEFT,
	_STD_MOV_JUMPRIGHT,
	_STD_MOV_JUMPTOIDLE,
	_STD_MOV_JUMPTOMOVE_BACK,
	_STD_MOV_JUMPTOMOVE_FRONT,
	_STD_MOV_JUMPTOMOVE_LEFT,
	_STD_MOV_JUMPTOMOVE_RIGHT,
	_STD_MOV_LEFTTORIGHT,
	_STD_MOV_MOVETOIDLE,
	_STD_MOV_RIGHTTOLEFT,
	_STD_MOV_RUNBACK,
	_STD_MOV_RUNFRONT,
	_STD_MOV_RUNLEFT,
	_STD_MOV_RUNLEFTBACK,
	_STD_MOV_RUNRIGHT,
	_STD_MOV_RUNRIGHTBACK,

	//HID
	_HID_MOV_IDLETOJUMP_FRONT,
	_HID_MOV_IDLETOJUMP_LEFT,
	_HID_MOV_IDLETOJUMP_RIGHT,
	_HID_MOV_JUMPFRONT,
	_HID_MOV_JUMPLEFT,
	_HID_MOV_JUMPRIGHT,
	_HID_MOV_JUMPTOIDLE,
	_HID_MOV_JUMPTOMOVE_BACK,
	_HID_MOV_JUMPTOMOVE_FRONT,
	_HID_MOV_JUMPTOMOVE_LEFT,
	_HID_MOV_JUMPTOMOVE_RIGHT,
	_HID_MOV_LEFTTORIGHT,
	_HID_MOV_MOVETOIDLE,
	_HID_MOV_RIGHTTOLEFT,
	_HID_MOV_RUNBACK,
	_HID_MOV_RUNFRONT,
	_HID_MOV_RUNLEFT,
	_HID_MOV_RUNLEFTBACK,
	_HID_MOV_RUNRIGHT,
	_HID_MOV_RUNRIGHTBACK,


	//========= ��� ==========//
	// Ȱ��
	_GLIDE_BOOST_FRONT,
	_GLIDE_BOOST_FRONT_CANCLE,
	_GLIDE_BOOST_FRONT_START,
	_GLIDE_DIVE_FRONT,
	_GLIDE_DIVE_FRONT_CANCLE,
	_GLIDE_IDLETOJUMP_FRONT_01,
	_GLIDE_JUMPFRONT,
	_GLIDE_JUMPLEFT,
	_GLIDE_JUMPRIGHT,

	// ����
	_SPRINT_FRONT,
	_SPRINT_FRONT_FALLSTART,
	_SPRINT_FRONT_ROCKETSTART,
	_SPRINT_IDLETOJUMP_FRONT_01,
	_SPRINT_IDLETOJUMP_FRONT_02,
	_SPRINT_IDLETOJUMP_FRONT_03,
	_SPRINT_JUMPFRONT,
	_SPRINT_JUMPTOIDLE,
	_SPRINT_JUMPTOMOVE_FRONT,
	_SPRINT_MOVETOIDLE,

	END,
};

enum class KUNGFUMASTER_FSM
{
	//============ 1
	// �ݰ�
	_1_COUNTERENEMY_READY,
	_1_COUNTERENEMY_STANDBY,
	_1_COUNTERENEMY_FINISH,
	_1_COUNTERENEMY_EXEC,
	// ����
	_1_WINDMILL,
	
	//============ 2
	// ���
	_2_FIRE_PUNCH1_END,
	_2_FIRE_PUNCH1_SWING,
	_2_FIRE_PUNCH2_END,
	_2_FIRE_PUNCH2_SWING,
	_2_FIRE_PUNCH3_END,
	_2_FIRE_PUNCH3_SWING,
	_2_FIRE_PUNCH4_END,
	_2_FIRE_PUNCH4_SWING,

	// �����ݰ�
	_2_DOWNCOUNTER_READY,
	_2_DOWNCOUNTER_STANDBY,
	_2_DOWNCOUNTER_EXEC,
	_2_DOWNCOUNTER_FINISH,

	// �����
	_2_1000HAND_SWING1,
	_2_1000HAND_EXEC1,
	_2_1000HAND_FINISH1,

	_2_1000HAND_SWING2,
	_2_1000HAND_EXEC2,
	_2_1000HAND_FINISH2,

	_2_WIND_KICK1_SWING,
	_2_WIND_KICK1_EXEC,
	_2_WIND_KICK1_END,


	// �����
	_2_POUNDING_FIRST,

	//============ 3
	// �ѱ��ĵ�
	_3_DRAGON_FLY_EXEC,

	// �ٸ��ɱ�
	_3_LOWKICK,

	//============ 4
	// �ر�
	_4_HEAVYPUNCH_SWING,

	// ��������
	_4_ICEBLAST_KICK,

	// �Ȳ���
	_4_ARMBAR,

	//============ Z
	// �·氢
	_Z_ASCENSION_KICK,
	
	//============ X
	// ����
	_X_BURSTABDOMEN,

	// ����
	_X_VIOLENCEPUNCH_CAST,
	_X_VIOLENCEPUNCH_EXEC,

	//============ C
	// �ǹ�
	_C_MARTIALDANCE1_EXEC,
	_C_MARTIALDANCE1_FIRE,
	_C_MARTIALDANCE2_EXEC,
	_C_MARTIALDANCE2_FIRE,
	_C_MARTIALDANCE3_EXEC,
	_C_MARTIALDANCE3_FIRE,
	_C_MARTIALDANCE4_EXEC,
	_C_MARTIALDANCE4_FIRE,

	//============ V
	// �����
	_V_CRUSHGROUND_SWING,

	//============ Q
	// Ⱦ�̵�
	_Q_LEFT_SIDE_FIRE,
	_Q_LEFT_SIDE_EXEC,
	
	//============ E
	// �İ���
	_E_RIGHT_SIDE_FIRE,
	_E_RIGHT_SIDE_EXEC,

	//============ TAB
	// Ż��
	_T_ESCAPE,

	// �پ����
	_T_JUMPPUNCH_FIRE,
	_T_JUMPPUNCH_EXEC,

	// �濰ȭ
	_T_WIND_MODE_SWING,
	_T_WIND_MODE_EXEC,
	_T_WIND_MODE_END,

	// ��������
	_T__FLYTAKEDOWN,

	//============ LBTN
	// �񿰰�
	_L_DRAGONKICK_FIRE,
	_L_DRAGONKICK_EXEC,
	
	//	��������
	_L_FLYING_DRAGONKICK_FIRE,
	_L_FLYING_DRAGONKICK_EXEC,

	// �鿭��
	_L_4HITKICK,
	
	// �������
	_L_SKYKICK,
	
	//============ RBTN
	// �÷�����
	_R_UPPERCUT,
	
	// �������� (����)
	_R_UPPERCUT2,
	
	// ���ݱ�
	_R_DASHUPPER_FIRE,
	_R_DASHUPPER_EXEC,

	// ������
	_R_LEFTELBOW,

	// ȭ����
	_R_TRIPLEKICK,

	//============ F
	// ��Ÿ
	_F_ABDOMENATTACK_FIRE,
	_F_ABDOMENATTACK_EXEC,

	// ����
	_F_FINALATTACK,

	// �ڱ�����
	_F_BACKWARDROLL,

	//============ SS
	// �Ĺ��̵�
	_S_BACKSTEP,

	// ���� ��
	_MOUNT_BAT_IDLE,
	// ���� ��
	_MOUNT_BAT_END,

	//============= FEED BACK =============//

	// �������
	_FB_CAPTURE_EXEC2_FIRE,
	_FB_CAPTURE_EXEC2_END,
	
	// �˹�
	_FB_KNOCKBACK_LONG_B_START,
	_FB_DOWN_FRONT_LOOPING,
	_FB_DOWN_FRONT_END,
	
	// ����
	_STUN_START,
	_STUN_LOOPING,
	
	// �׷α�
	_GROGGY_KNEE_F_START,
	_GROGGY_KNEE_LOOPING,
	_GROGGY_KNEE_END,
	
	// �ٿ�
	_DOWN_MID_B_START,		// ������, �񿰰�, ��������
	_DOWN_LOW_B_R_START,	// �ϴܺ���, �ٸ��ɱ�
	_DOWN_LOOPING,
	_DOWN_END,

	// ����
	_AIR_MIDAIR1_UPPER,					// ����, �پ����
	_AIR_UPPER_SLASH,					// �÷�����
	_AIR_MIDAIR2_UPPER,					// ���, õ��, ��������
	_AIR_MIDAIR1_LOWER,					// �ƹ��͵� ���� ���ؼ� ��������
	_AIR_BIGHITX_SKL_SKYKICK,			// ���� ���
	_AIR_STANDARDX_SKL_CRASH,			// �߶�

	_FB_ASCENSION,						// ��õ

	// �⺻ �ǰ�
	_DAMAGEBACK,		// �ǹ� �ǰ�
	_DAMAGEFRONT,		// �⺻ �ǰ�

	// ���											  
	_NON_MOV_IDLE,	// ������
	_BAT_MOV_IDLE,	// ����	
	

	// ���� ���										
	_BAT_EQUIPX,


	// MOVING											
	_COMBAT_JUMPTOIDLE,
	_COMBAT_MOVETOIDLE, // ����
	_BAT_IDLETOJUMP_FRONT,
	_NON_IDLETOJUMP_FRONT,
	_IDLETOJUMP_LEFT,
	_IDLETOJUMP_RIGHT,
	_JUMPFRONT,
	_JUMPLEFT,
	_JUMPRIGHT,
	_NONCOMBAT_JUMPTOIDLE,
	_JUMPTOMOVE_BACK,
	_JUMPTOMOVE_FRONT,
	_JUMPTOMOVE_LEFT,
	_JUMPTOMOVE_RIGHT,
	_LEFTTORIGHT,
	_NONCOMBAT_MOVETOIDLE,	// ������
	_RIGHTTOLEFT,
	_RUNBACK,
	_RUNFRONT,
	_RUNLEFT,
	_RUNLEFTBACK,
	_RUNRIGHT,
	_RUNRIGHTBACK,

	// Ȱ��											  
	_GLIDE_BOOST_FRONT,
	_GLIDE_BOOST_FRONT_CANCLE,
	_GLIDE_BOOST_FRONT_START,
	_GLIDE_DIVE_FRONT,
	_GLIDE_DIVE_FRONT_CANCLE,
	_GLIDE_IDLETOJUMP_FRONT_01,
	_GLIDE_JUMPFRONT,
	_GLIDE_JUMPLEFT,
	_GLIDE_JUMPRIGHT,

	// ���
	_SPRINT_FRONT,
	_SPRINT_FRONT_FALLSTART,
	_SPRINT_FRONT_ROCKETSTART,
	_SPRINT_IDLETOJUMP_FRONT_01,
	_SPRINT_IDLETOJUMP_FRONT_02,
	_SPRINT_IDLETOJUMP_FRONT_03,
	_SPRINT_JUMPFRONT,
	_SPRINT_JUMPTOIDLE,
	_SPRINT_JUMPTOMOVE_FRONT,
	_SPRINT_MOVETOIDLE,

	END,
};

enum class RENDER_ADD
{
	NONE,
	RENDER_ENGINE,
	RENDER_CLIENT,
	END,
};

enum class LANDSCAPE_MATERIAL
{
	DISABLE,
	TILE1,
	TILE2,
	TILE3,
	END,
};

enum class TERRAIN_OPTION
{
	DISABLE,
	ENABLE,
	END,
};

enum class BRUSH_TYPE
{
	DISABLE,
	INK,
	CIRCLE,
	END,
};

enum class BRUSH_STATE
{
	DISABLE,
	HEIGHT_MAP,
	WEIGHT_MAP,
	END,
};

enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	SHADER,
	END,
};

namespace RES_TYPE_STR
{
	extern const wchar_t* MATERIAL;
	extern const wchar_t* MESH;
	extern const wchar_t* TEXTURE;
	extern const wchar_t* SOUND;
	extern const wchar_t* SHADER;
}

extern const wchar_t* RES_TYPE_NAME[(UINT)RES_TYPE::END];


enum class SHADER_TYPE
{
	ST_VERTEX = 0x01,
	ST_HULL = 0x02,
	ST_DOMAIN = 0x04,
	ST_GEOMETRY = 0x08,
	ST_COMPUTE = 0x10,
	ST_PIXEL = 0x20,
	ST_END
};

#define ST_ALL (UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_HULL\
 | (UINT)SHADER_TYPE::ST_DOMAIN | (UINT)SHADER_TYPE::ST_GEOMETRY | (UINT)SHADER_TYPE::ST_COMPUTE | (UINT)SHADER_TYPE::ST_PIXEL

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESHRENDER,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	TERRAIN,
	PARTICLESYSTEM,
	END,
	SCRIPT,
};

enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class COLLIDER3D_TYPE
{
	MESH,
	CUBE,
	SPHERE,
};

enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,
	INT_END,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,
	FLOAT_END,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,
	VEC2_END,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,
	VEC4_END,

	MATRIX_0,
	MATRIX_1,
	MATRIX_2,
	MATRIX_3,
	MATRIX_END,

	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,
	TEXARR_0,
	TEXARR_1,
	TEXARR_2,
	TEXARR_3,
	TEX_END,

	RWTEX_0,
	RWTEX_1,
	RWTEX_2,
	RWTEX_3,
	RWTEX_END,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONEBLEND,
	END,
};

enum class DEPTH_STENCIL_STATE
{
	DEFAULT,	// LESS
	LESS_EQUAL,
	GRATER,
	GRATER_EQUAL,
	NO_TEST_NO_WIRTE,
	NO_WRITE,
	END,
};

enum class DBG_TYPE
{
	DBG_ERROR,
	DBG_WARNNING,
	DBG_MSG,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Idx
	DELETE_OBJECT,	// wParam : GameObject,
	ADD_CHILD,		// wParam : Parent Object, lParam : Child Object
	CLEAR_PARENT,	// wParam : Target Object
	TRANSFER_LAYER,	// wParam : Target Object, lParam : (HIWORD)Layer Index (LOWORD)bMoveAll
	TRANSFER_SCENE,
	ACTIVATE_GAMEOBJECT,	// wParam : GameObject Adress
	DEACTIVATE_GAMEOBJECT,	// wParam : GameObject Adress

	ACTIVATE_COMPONENT,		// wParam : Component Adress
	DEACTIVATE_COMPONENT,	// wParam : Component Adress

	END,
};

enum class LIGHT_TYPE
{
	DIR,
	POINT,
	SPOT,
	END,
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class FACE_TYPE
{
	FT_NEAR,
	FT_FAR,
	FT_UP,
	FT_DOWN,
	FT_LEFT,
	FT_RIGHT,
	FT_END,
};

enum class RT_TYPE
{
	SWAPCHAIN,

	DIFFUSE,
	NORMAL,
	POSITION,
	SPECULARMTRL,

	LIGHT,
	SPECULAR,

	SHADOWMAP,

	DEPTH_BUFFER,

	POSTEFFECT,

	END,
};

enum class MRT_TYPE
{
	// MRT			RT ����
	SWAPCHAIN,  // SWAPCHAIN
	DEFERRED,	// DIFFUSE, NORMAL, POSITION, DEPTHBUFFER
	LIGHT,		// LIGHT, SPECULAR
	SHADOWMAP,	// SHADOWMAP
	POSTEFFECT, // POSTEFFECT1, POSTEFFECT2
	END,
};

enum class SHADER_POV
{
	NONE_RENDER,
	LIGHTING,
	DEFERRED,
	FORWARD,
	PARTICLE,
	POSTEFFECT,
	COMPUTE,
};