#include "movie_player.hpp"

MoviePlayer::MoviePlayer(const std::string& file_path, const BackColorKind back_color_kind, const bool is_loop) :
	m_back_color_kind	(back_color_kind),
	m_movie_graphic		(nullptr),
	m_result_screen		(nullptr),
	m_is_loop			(is_loop)
{
	m_movie_graphic	= std::make_shared<Graphicer>(file_path);
	m_result_screen	= std::make_shared<ScreenCreator>(m_movie_graphic->GetOriginSize());
	m_movie_graphic->SetCenterPos(m_result_screen->GetHalfScreenSize());
}

MoviePlayer::MoviePlayer(const int movie_handle, const BackColorKind back_color_kind, const bool is_loop) :
	m_back_color_kind	(back_color_kind),
	m_movie_graphic		(nullptr),
	m_result_screen		(nullptr),
	m_is_loop			(is_loop)
{
	m_movie_graphic	= std::make_shared<Graphicer>(movie_handle);
	m_result_screen	= std::make_shared<ScreenCreator>(m_movie_graphic->GetOriginSize());
	m_movie_graphic->SetCenterPos(m_result_screen->GetHalfScreenSize());
}

MoviePlayer::~MoviePlayer()
{

}

void MoviePlayer::Play(const bool is_draw_graphic_frame) const
{
	m_result_screen->Draw(is_draw_graphic_frame);
}

void MoviePlayer::CreateMovieScreen() const
{
	const auto play_type = m_is_loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_NORMAL;

	m_result_screen->UseScreen();
	PlayMovieToGraph(m_movie_graphic->GetGraphicHandle(), play_type);
	m_movie_graphic->Draw();
	m_result_screen->UnuseScreen();
}
