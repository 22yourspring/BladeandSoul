#include "pch.h"
#include "gif_control.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// end.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// jsirm_generator.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Gdiplus;


BEGIN_MESSAGE_MAP(gif_control, CStatic)
	ON_WM_TIMER()
END_MESSAGE_MAP()

gif_control::gif_control() {
	_image = nullptr;
	_current_frame = 0;
	_frame_count = 0;
	_is_playing = false;

	_dimension_ids = nullptr;
	_property_item = nullptr;
}

gif_control::~gif_control() {
	clear();
}

void gif_control::clear() {

	if (nullptr != _image) {
		delete _image;
		_image = nullptr;
	}

	if (nullptr != _dimension_ids) {
		delete _dimension_ids;
		_dimension_ids = nullptr;
	}

	if (nullptr != _property_item) {
		delete _property_item;
		_property_item = nullptr;
	}
}

bool gif_control::verify() const {
	bool result = false;

	do {
		if (nullptr == _image) {
			break;
		}

		result = true;

	} while (false);

	return result;
}

bool gif_control::load(LPCTSTR file_name) {
	bool result = false;

	do {
		if (nullptr != _image) {
			clear();
		}

		_image = Gdiplus::Image::FromFile(file_name);
		if (Gdiplus::Ok != _image->GetLastStatus()) {
			delete _image;
			_image = nullptr;
			break;
		}

		UINT frame_dimension_count = _image->GetFrameDimensionsCount();
		if (frame_dimension_count <= 0) {
			break;
		}

		_dimension_ids = (GUID*)new GUID[frame_dimension_count];
		if (nullptr == _dimension_ids) {
			break;
		}

		_image->GetFrameDimensionsList(_dimension_ids, frame_dimension_count);

		wchar_t guid_string[39] = L"";
		StringFromGUID2(_dimension_ids[0], guid_string, 39);

		_frame_count = _image->GetFrameCount(&_dimension_ids[0]);

		UINT total_buffer = _image->GetPropertyItemSize(PropertyTagFrameDelay);
		_property_item = (Gdiplus::PropertyItem *)new char[total_buffer];
		_image->GetPropertyItem(PropertyTagFrameDelay, total_buffer, _property_item);

		result = true;
	} while (false);

	return result;
}

void gif_control::play() {

	do {
		if (_is_playing) {
			break;
		}

		if (nullptr == _image) {
			break;
		}

		_current_frame = 0;
		GUID guid = Gdiplus::FrameDimensionTime;
		_image->SelectActiveFrame(&guid, _current_frame);

		UINT elapse = ((UINT*)(_property_item->value))[_current_frame] * 10;
		SetTimer(1, elapse, nullptr);
		_current_frame++;

		Invalidate(FALSE);

		_is_playing = true;

	} while (false);
}

void gif_control::stop() {

	do {
		if (false == _is_playing) {
			break;
		}

		KillTimer(1);

		_is_playing = false;

	} while (false);
}

void gif_control::PreSubclassWindow() {

	ModifyStyle(0, SS_OWNERDRAW | SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

void gif_control::OnTimer(UINT_PTR nIDEvent) {

	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);

		GUID guid = Gdiplus::FrameDimensionTime;
		_image->SelectActiveFrame(&guid, _current_frame);

		UINT elapse = ((UINT*)(_property_item->value))[_current_frame] * 10;
		SetTimer(1, elapse, nullptr);
		_current_frame = (++_current_frame) % _frame_count;

		Invalidate(FALSE);
	}

	CStatic::OnTimer(nIDEvent);
}

void gif_control::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {

	Gdiplus::Graphics grapics(lpDrawItemStruct->hDC);

	CRect rect;
	GetClientRect(&rect);

	grapics.DrawImage(_image, rect.left, rect.top, rect.Width(), rect.Height());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// end.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////