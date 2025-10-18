#pragma once
#include "../Path/movie_path.hpp"
#include "../Part/graphicer.hpp"
#include "../Part/screen_creator.hpp"

class MoviePlayer final
{
public:
	enum class BackColorKind
	{
		kNone = -1,

		kBlack,
		kWhite,
	};

public:
	MoviePlayer(const std::string& file_path,	const BackColorKind back_color_kind, const bool is_loop);
	MoviePlayer(const int movie_handle,			const BackColorKind back_color_kind, const bool is_loop);
	~MoviePlayer();

	void CreateMovieScreen() const;
	void Play(const bool is_draw_graphic_frame = false) const;

	[[nodiscard]] std::shared_ptr<Graphicer>	 GetResourceMovieGraphicer() const { return m_movie_graphic; }
	[[nodiscard]] std::shared_ptr<Graphicer>	 GetResultGraphicer()		 const { return m_result_screen->GetGraphicer(); }
	[[nodiscard]] std::shared_ptr<ScreenCreator> GetResultScreen()			 const { return m_result_screen; }

private:
	BackColorKind					m_back_color_kind;
	std::shared_ptr<ScreenCreator>	m_result_screen;
	std::shared_ptr<Graphicer>		m_movie_graphic;
	bool							m_is_loop;
};
