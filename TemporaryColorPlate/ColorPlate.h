#pragma once
#include <vector>
#include <iostream>
#include <windows.h>
#include "imgui.h"
#include <string>
using std::vector;

class ColorPlan
{
public:
	ColorPlan()
	{
		// 在构造函数中对 ColorVector 初始化
		ColorVector = std::vector<std::vector<ImVec4>>(4, std::vector<ImVec4>(3));

		for (int i = 0; i < ColorVector.size(); ++i) 
		{	// 遍历每一行
			for (int j = 0; j < ColorVector[i].size(); ++j) 
			{   // 遍历该行的每个元素
				ColorVector[i][j] = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // 给当前元素赋值
			}
		}
	}
	std::vector<std::vector<ImVec4>> ColorVector; 
	void CreateColorPlan(std::vector<std::vector<ImVec4>>& vec);

private:
	std::string ColorToHexString(const ImVec4& color);
	bool CopyStringToClipboard(const std::string& str);
	void CopyColorToClipboard(const ImVec4& color);
};

