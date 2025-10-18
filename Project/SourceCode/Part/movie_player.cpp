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

void MoviePlayer::Draw() const
{
	m_result_screen->Draw();
}

void MoviePlayer::CreateMovieScreen() const
{
	const auto play_type = m_is_loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_NORMAL;

	m_result_screen->UseScreen();
	PlayMovieToGraph(m_movie_graphic->GetGraphicHandle(), play_type);
	m_movie_graphic->Draw();
	m_result_screen->UnuseScreen();

	switch (m_back_color_kind)
	{
	case MoviePlayer::BackColorKind::kBlack:
		GraphFilter(m_result_screen->GetScreenHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS,	  10, TRUE, GetColor(0, 0, 0), 0);
		break;

	case MoviePlayer::BackColorKind::kWhite:
		GraphFilter(m_result_screen->GetScreenHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_GREATER, 245, TRUE, GetColor(0, 0, 0), 0);
		break;

	default:
		break;
	}
}
