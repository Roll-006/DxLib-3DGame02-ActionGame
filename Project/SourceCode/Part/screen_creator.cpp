#include "screen_creator.hpp"

ScreenCreator::ScreenCreator(const Vector2D<int> screen_size) :
	m_screen_size			(screen_size),
	m_screen_graphic_handle	(MakeScreen(screen_size.x, screen_size.y, TRUE)),
	m_graphicer				(nullptr)
{
	m_graphicer = std::make_shared<Graphicer>(m_screen_graphic_handle);
}

ScreenCreator::~ScreenCreator()
{
	// MEMO : 画像の削除処理はGraphicerに委ねる
}

void ScreenCreator::UseScreen()
{
	// SetDrawScreenを使用した際に、カメラの設定が破棄されるため
	// 復元するためにカメラ情報を保存
	m_prev_near		= GetCameraNear();
	m_prev_far		= GetCameraFar();
	m_prev_fov		= GetCameraFov();
	m_prev_pos		= GetCameraPosition();
	m_prev_target	= GetCameraTarget();

	//// フォグ情報を保存
	//m_was_enabled	= GetFogEnable();
	//m_prev_mode		= GetFogMode();
	//GetFogColor(&m_prev_r, &m_prev_g, &m_prev_b);
	//GetFogStartEnd(&m_prev_start, &m_prev_end);
	//m_density		= GetFogDensity();

	SetDrawScreen(m_screen_graphic_handle);
}

void ScreenCreator::UnuseScreen()
{
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラ情報の復元
	SetCameraNearFar(m_prev_near, m_prev_far);
	SetupCamera_Perspective(m_prev_fov);
	SetCameraPositionAndTarget_UpVecY(m_prev_pos, m_prev_target);

	//// フォグ情報の復元
	//SetFogEnable(m_was_enabled);
	//SetFogMode(m_prev_mode);
	//SetFogColor(m_prev_r, m_prev_g, m_prev_b);
	//SetFogStartEnd(m_prev_start, m_prev_end);
	//SetFogDensity(m_density);
}

void ScreenCreator::Draw() const
{
	m_graphicer->Draw();
}
