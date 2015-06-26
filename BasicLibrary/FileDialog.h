#pragma once

#include <Windows.h>
#include <string>

namespace Yupei
{
	class FileDialog
	{
	public:
		enum class FileDialogType
		{
			Save,OpenAs
		};
		FileDialog() = default;
		FileDialog(HWND _parentWindow);
		void SetFiler(const wchar_t* _filer)
		{
			fileName.lpstrFilter = _filer;
		}
		std::wstring Show(FileDialogType _type,
			std::wstring _startString = std::wstring{})
		{
			auto res = std::move(_startString);
			res.resize(MAX_PATH);
			fileName.lpstrFile = &res[0];
			fileName.nMaxFile = res.size();
			switch (_type)
			{
			case FileDialog::FileDialogType::Save:
				::GetSaveFileName(&fileName);
				break;
			case FileDialog::FileDialogType::OpenAs:
				::GetOpenFileName(&fileName);
				break;
			default:
				break;
			}
			return res;
		}
	private:
		OPENFILENAME fileName = { sizeof(OPENFILENAME) };
	};
}
