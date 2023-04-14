#include "ColorPlate.h"
#include "imgui_internal.h"


void ColorPlan::CreateColorPlan(std::vector<std::vector<ImVec4>>& vec )
{
	static int rows = vec.size();
	static int columns = (rows > 0) ? vec[0].size() : 0;

	static bool isOpen = true;
	static bool isTop = false;
	static bool lastIsTop = false;

	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin("ColorPlate" , &isOpen, ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoResize);


	if (ImGui::Checkbox("Whether the window should be placed at the top level", &isTop))
	{
		if (isTop != lastIsTop)
		{
			if (isTop)
			{
				//::MessageBoxA(NULL, "The window will be placed at the top level", "Info", MB_OK);
				// 获取当前窗口句柄
				HWND hwnd = FindWindowA(NULL, "Temporary Color Plate");

				// 将其设置为顶层窗口并显示
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			else
			{
				//::MessageBoxA(NULL, "The window will not be placed at the top level", "Info", MB_OK);
				// 获取当前窗口句柄
				HWND hwnd = FindWindowA(NULL, "Temporary Color Plate");

				// 将其恢复到正常状态并显示
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			}
			lastIsTop = isTop;
		}
	}
	if (ImGui::BeginTable("table_padding", columns + 1, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX))
	{
		// 添加列标题
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

		// 第一格留空
		ImGui::TableSetColumnIndex(0);
		ImGui::TableNextColumn();

		for (int column = 0; column < columns; ++column)
		{
			ImGui::TableSetColumnIndex(column + 1);
			ImGui::Text("Column %d", column);
		}

		// 添加颜色值
		for (int row = 0; row < rows; ++row)
		{
			ImGui::TableNextRow();

			// 固定第一列
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Row %d", row);

			for (int column = 0; column < columns; ++column)
			{
				ImGui::TableSetColumnIndex(column + 1);

				ImVec4& color = vec[row][column];

				ImGui::PushID(row * columns + column);
				if (ImGui::ColorEdit3("color -->", (float*)&color, ImGuiColorEditFlags_InputRGB))
				{
					// 当颜色选择器的值发生变化时的处理
				}
				ImGui::PopID();

				ImGui::SameLine();

				// 为每个按钮设置单独的 ID
				ImGui::PushID(row * columns + column);
				if (ImGui::Button("Copy"))
				{
					ImVec4& color = vec[row][column];
					CopyColorToClipboard(color);
				}
				ImGui::PopID();
			}
		}

		ImGui::EndTable();
	}

	ImGui::End();

}

std::string ColorPlan::ColorToHexString(const ImVec4& color)
{
	char hexString[10] = { '\0' };
	int r = static_cast<int>(color.x * 255 + 0.5f); // 四舍五入
	int g = static_cast<int>(color.y * 255 + 0.5f);
	int b = static_cast<int>(color.z * 255 + 0.5f);
	sprintf_s(hexString, sizeof(hexString), "#%02X%02X%02X", r, g, b);
	return std::string(hexString);
}

bool ColorPlan::CopyStringToClipboard(const std::string& str)
{
	// 打开剪贴板
	if (!OpenClipboard(nullptr))
	{
		return false;
	}

	// 清除当前剪贴板内容
	EmptyClipboard();

	// 获取需要复制的字符串长度
	size_t strLen = str.size() + 1;

	// 分配全局内存作为剪贴板内容，并将字符串复制到其中
	HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, strLen);
	char* pchData = static_cast<char*>(GlobalLock(hClipboardData));
	if (pchData == nullptr)
	{
		CloseClipboard();
		return false;
	}

	memcpy(pchData, str.c_str(), strLen);

	GlobalUnlock(hClipboardData);

	// 将内存块标记为 Windows-Unicode 文本格式，并将其添加到剪贴板中
	SetClipboardData(CF_TEXT, hClipboardData);

	// 关闭剪贴板
	CloseClipboard();

	return true;
}

void ColorPlan::CopyColorToClipboard(const ImVec4& color)
{
	// 将颜色值转换为文本字符串
	std::string strColor = ColorToHexString(color);

	// 将字符串复制到剪贴板中
	CopyStringToClipboard(strColor);
}

