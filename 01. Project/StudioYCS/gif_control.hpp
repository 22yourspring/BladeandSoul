/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// jsirm_generator.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

using namespace Gdiplus;

class gif_control : public CStatic {
	DECLARE_MESSAGE_MAP()

public:
	gif_control();
	~gif_control();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	bool verify() const;
	bool load(LPCTSTR file_name);
	void play();
	void stop();
	void clear();

private:
	Gdiplus::Image* _image;
	GUID* _dimension_ids;
	UINT _frame_count;
	UINT _current_frame;
	Gdiplus::PropertyItem* _property_item;
	bool _is_playing;
};